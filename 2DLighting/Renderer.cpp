#include "Renderer.h"

#include "Scene.h"

#include <algorithm>
#include "tbb/parallel_for.h"

#define MAX_DEPTH 6
#define BIAS 1e-4f
#define M_2PI 6.28318530718f
#define EPSILON 1e-6f
#define MAXITER 32
#define MAXDIST 5.0f

using namespace tbb;

Renderer::Renderer()
	:m_width(0), m_height(0), m_image(nullptr) {}


Renderer::~Renderer()
{
	if (m_image != nullptr)
		delete m_image;
	m_image = nullptr;
}

void Renderer::initialize(int width, int height)
{
	m_width = width;
	m_height = height;
	if (m_image != nullptr)
		delete m_image;
	m_samples = 256;
	m_image = new unsigned char[width * height * 4];
}

unsigned char * Renderer::render()
{
	parallel_for(blocked_range<size_t>(0, m_height * m_width, 10000),
		[&](blocked_range<size_t>& r)
	{
		for (size_t i = r.begin(); i != r.end(); ++i)
		{
			size_t col = i % m_width;
			size_t row = i / m_width;
			glm::vec3 color(0.0f);
			// sampling and lighting.
			color = sample(static_cast<float>(col) / m_width, static_cast<float>(row) / m_height);
			
			// normal visualization.
			//gradient(static_cast<float>(col) / m_width, static_cast<float>(row) / m_height,
			//	color.x, color.y);
			//color.x = fmaxf(fminf(color.x, 1.0f), -1.0f) * 0.5f + 0.5f;
			//color.y = fmaxf(fminf(color.y, 1.0f), -1.0f) * 0.5f + 0.5f;

			// save to pixel.
			color.x /= (color.x + 1.0f);
			color.y /= (color.y + 1.0f);
			color.z /= (color.z + 1.0f);
			drawPixel(row, col, color);
		}
	});

	return m_image;
}

glm::vec3 Renderer::sample(float x, float y)
{
	glm::vec3 radiance = glm::vec3(0.0f);
	for (int i = 0; i < m_samples; ++i)
	{
		// randome sampling.
		// float angle = M_2PI * rand() / RAND_MAX;
		
		// stratified samping.
		//float angle = M_2PI * i / m_samples;

		// jittered sampling.
		float angle = M_2PI * (i + pseudoRandFloat()) / m_samples;

		radiance += deNan(trace(x, y, cosf(angle), sinf(angle), 0));
	}
	radiance.x /= m_samples;
	radiance.y /= m_samples;
	radiance.z /= m_samples;
	return  radiance;
}

Result Renderer::scene(float x, float y)
{
	// scene 1.
	//Result ret = Scene::oneEmissiveSphereScene(ox + dx * step, oy + dy * step);
	// scene 2.
	//Result ret = Scene::threeEmissiveSphereScene(x, y);
	// scene 3.
	//Result ret = Scene::moonEmissiveScene(ox + dx * step, oy + dy * step);
	// scene 4.
	//Result ret = Scene::planeEmissiveScene(ox + dx * step, oy + dy * step);
	// scene 5.
	//Result ret = Scene::capsuleEmissiveScene(ox + dx * step, oy + dy * step);
	// scene 6.
	//Result ret = Scene::boxEmissiveScene(ox + dx * step, oy + dy * step);
	// scene 7.
	//Result ret = Scene::triangleEmissiveScene(x, y);
	// scene 8.
	//Result ret = Scene::reflectEmissiveScene(x, y);
	// scene 9.
	//Result ret = Scene::causticEmissiveScene(x, y);
	// scene 10.
	Result ret = Scene::rectRefractEmissiveScene(x, y);
	// scene 11.
	//Result ret = Scene::refractEmissiveScene(x, y);
	// scene 12.
	//Result ret = Scene::beerLambertScene(x, y);
	// scene 13.
	//Result ret = Scene::heartScene(x, y);
	// scene 14.
	//Result ret = Scene::finalScene(x, y);
	// scene 15.
	//Result ret = Scene::nameScene(x, y);
	return ret;
}

glm::vec3 Renderer::trace(float ox, float oy, float dx, float dy, int depth)
{
	float step = 1e-3f;
	float sign = scene(ox, oy).sdf > 0.0f ? 1.0f : -1.0f;
	for (int i = 0; i < MAXITER && step < MAXDIST; ++i)
	{
		float x = ox + dx * step;
		float y = oy + dy * step;
		Result ret = scene(x, y);
		// reach the egde(==0) or inside(<0)
		if (ret.sdf * sign < EPSILON)
		{
			glm::vec3 sum = ret.emissive;
			if (depth < MAX_DEPTH && (ret.reflectivity > 0.0f || ret.eta > 0.0f))
			{
				// get normal and reflect.
				float nx = 0, ny = 0, rx = 0, ry = 0;
				float reflectivity = ret.reflectivity;
				gradient(x, y, nx, ny);
				// inside, need to reverse normal.
				nx *= sign;
				ny *= sign;
				// refraction.
				if (ret.eta > 0.0f)
				{
					if (refract(dx, dy, nx, ny, sign < 0.0f ? ret.eta : 1.0f / ret.eta, rx, ry))
					{
						float cosi = -(dx * nx + dy * ny);
						float cost = -(rx * nx + ry * ny);
						reflectivity = sign < 0.0f ? fresnel(cosi, cost, ret.eta, 1.0f)
							: fresnel(cosi, cost, 1.0f, ret.eta);
						sum += (1.0f - reflectivity) * trace(x - nx * BIAS, y - ny * BIAS, rx, ry, depth + 1);
					}
					else
						reflectivity = 1.0f;// full reflect.
				}
				// reflection.
				if (reflectivity > 0.0f)
				{
					reflect(dx, dy, nx, ny, rx, ry);
					sum += reflectivity * trace(x + nx * BIAS, y + ny * BIAS, rx, ry, depth + 1);
				}
			}
			return sum * beerLambert(ret.absorption, step);
		}
		step += ret.sdf * sign;
	}
	return glm::vec3(0.0f);
}

void Renderer::drawPixel(unsigned int row, unsigned int col, const glm::vec3 & color)
{
	if (row < 0 || row >= m_height || col < 0 || col >= m_width)
		return;
	unsigned int index = (row * m_width + col) * 4;
	m_image[index + 0] = static_cast<char>(std::min(color.x, 1.0f) * 255.99);
	m_image[index + 1] = static_cast<char>(std::min(color.y, 1.0f) * 255.99);
	m_image[index + 2] = static_cast<char>(std::min(color.z, 1.0f) * 255.99);
	m_image[index + 3] = static_cast<char>(1.0f * 255.99);
}

float Renderer::pseudoRandFloat()
{
	// return pseudo random float number in [0, 1].
	return static_cast<float>(rand()) / RAND_MAX;
}

glm::vec3 Renderer::beerLambert(glm::vec3 a, float d)
{
	return glm::vec3(expf(-a.x * d), expf(-a.y * d), expf(-a.z * d));
}

void Renderer::gradient(float x, float y, float & nx, float &ny)
{
	nx = (scene(x + EPSILON, y).sdf - scene(x - EPSILON, y).sdf) * (0.5f / EPSILON);
	ny = (scene(x, y + EPSILON).sdf - scene(x, y - EPSILON).sdf) * (0.5f / EPSILON);
}

float Renderer::fresnel(float cosi, float cost, float etai, float etat)
{
	float rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
	float rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
	return (rs * rs + rp * rp) * 0.5f;
}

void Renderer::reflect(float ix, float iy, float nx, float ny, float & rx, float & ry)
{
	float idotn_2 = (ix * nx + iy * ny) * 2.0f;
	rx = ix - idotn_2 * nx;
	ry = iy - idotn_2 * ny;
}

int Renderer::refract(float ix, float iy, float nx, float ny, float eta, float & rx, float & ry)
{
	float idotn = ix * nx + iy * ny;
	float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
	if (k < 0.0f)
		return 0;
	float a = eta * idotn + sqrtf(k);
	rx = eta * ix - a * nx;
	ry = eta * iy - a * ny;
	return 1;
}
