#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "SDF.h"

class Renderer
{
private:
	int m_samples;
	int m_width, m_height;
	unsigned char *m_image;

public:
	typedef std::shared_ptr<Renderer> ptr;

	Renderer();
	~Renderer();

	void initialize(int width, int height);

	unsigned char *render();

private:
	glm::vec3 sample(float x, float y);
	Result scene(float x, float y);
	glm::vec3 trace(float ox, float oy, float dx, float dy, int depth);
	void drawPixel(unsigned int row, unsigned int col, const glm::vec3 &color);
	float pseudoRandFloat();

	glm::vec3 beerLambert(glm::vec3 a, float d);
	void gradient(float x, float y, float &nx, float &ny);
	float fresnel(float cosi, float cost, float etai, float etat);
	void reflect(float ix, float iy, float nx, float ny, float &rx, float &ry);
	int refract(float ix, float iy, float nx, float ny, float eta, float &rx, float &ry);

	glm::vec3 deNan(const glm::vec3 &target)
	{
		glm::vec3 temp = target;
		if (!(temp[0] == temp[0]))temp[0] = 0;
		if (!(temp[1] == temp[1]))temp[1] = 0;
		if (!(temp[2] == temp[2]))temp[2] = 0;
		return temp;
	}
};

