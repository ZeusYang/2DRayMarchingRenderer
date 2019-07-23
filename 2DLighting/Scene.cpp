#include "Scene.h"

#define M_2PI 6.28318530718f

Result Scene::oneEmissiveSphereScene(float x, float y)
{
	Result ret = Result(SDF::circleSDF(x, y, 0.5f, 0.5f, 0.1f), glm::vec3(2.0f), 0.0f);
	return ret;
}

Result Scene::threeEmissiveSphereScene(float x, float y)
{
	Result ret1 = Result(SDF::circleSDF(x, y, 0.3f, 0.3f, 0.1f), glm::vec3(2.0f), 0.0f);
	Result ret2 = Result(SDF::circleSDF(x, y, 0.3f, 0.7f, 0.05f), glm::vec3(0.8f), 0.0f);
	Result ret3 = Result(SDF::circleSDF(x, y, 0.7f, 0.5f, 0.1f), glm::vec3(0.0f), 0.0f);
	return SDF::unionOperation(SDF::unionOperation(ret1, ret2), ret3);
}

Result Scene::moonEmissiveScene(float x, float y)
{
	Result ret1 = Result(SDF::circleSDF(x, y, 0.4f, 0.5f, 0.20f), glm::vec3(1.0f), 0.0f);
	Result ret2 = Result(SDF::circleSDF(x, y, 0.6f, 0.5f, 0.20f), glm::vec3(0.8f), 0.0f);
	//return SDF::unionOperation(ret1, ret2);
	//return SDF::intersectOperation(ret1, ret2);
	return SDF::subtractOperation(ret1, ret2);
}

Result Scene::planeEmissiveScene(float x, float y)
{
	Result ret1 = Result(SDF::circleSDF(x, y, 0.5f, 0.5f, 0.2f), glm::vec3(1.0f), 0.0f);
	Result ret2 = Result(SDF::planeSDF(x, y, 0.0f, 0.5f, 0.0f, 1.0f), glm::vec3(0.8f), 0.0f);
	return SDF::intersectOperation(ret1, ret2);
}

Result Scene::capsuleEmissiveScene(float x, float y)
{
	Result ret = Result(SDF::capsuleSDF(x, y, 0.4f, 0.4f, 0.6f, 0.6f, 0.1f), glm::vec3(1.0f), 0.0f);
	return ret;
}

Result Scene::boxEmissiveScene(float x, float y)
{
	Result ret = Result(SDF::boxSDF(x, y, 0.5f, 0.5f, M_2PI / 16.0f, 0.3f, 0.1f), glm::vec3(1.0f), 0.0f);
	return ret;
}

Result Scene::triangleEmissiveScene(float x, float y)
{
	Result ret = Result(SDF::triangleSDF(x, y, 0.5f, 0.2f, 0.8f, 0.8f, 0.3f, 0.6f), glm::vec3(1.0f), 0.0f);
	return ret;
}

Result Scene::reflectEmissiveScene(float x, float y)
{
	Result ret1 = Result(SDF::circleSDF(x, y, 0.4f, 0.2f, 0.1f), glm::vec3(2.0f), 0.0f);
	Result ret2 = Result(SDF::boxSDF(x, y, 0.5f, 0.8f, M_2PI / 16.0f, 0.1f, 0.1f), glm::vec3(0.0f), 0.9f);
	Result ret3 = Result(SDF::boxSDF(x, y, 0.8f, 0.5f, M_2PI / 16.0f, 0.1f, 0.1f), glm::vec3(0.0f), 0.9f);
	return SDF::unionOperation(SDF::unionOperation(ret1, ret2), ret3);
}

Result Scene::causticEmissiveScene(float x, float y)
{
	Result a = Result(SDF::circleSDF(x, y, 0.4f, 0.2f, 0.1f), glm::vec3(2.0f), 0.0f);
	Result d = Result(SDF::planeSDF(x, y, 0.0f, 0.5f, 0.0f, -1.0f), glm::vec3(0.0f), 0.9f);
	Result e = Result(SDF::circleSDF(x, y, 0.5f, 0.5f, 0.4f), glm::vec3(0.0f), 0.9f);
	return SDF::unionOperation(a, SDF::subtractOperation(d, e));
}

Result Scene::rectRefractEmissiveScene(float x, float y)
{
	Result ret1 = Result(SDF::boxSDF(x, y, 0.5f, 0.5f, 0.0f, 0.3f, 0.2f), glm::vec3(0.0f), 0.2f, 1.5f, glm::vec3(4.0f, 4.0f, 1.0f));
	Result ret2 = Result(SDF::circleSDF(x, y, 0.1f, 0.1f, 0.1f), glm::vec3(7.0f), 0.0f, 0.0f);
	return SDF::unionOperation(ret1, ret2);
}

Result Scene::refractEmissiveScene(float x, float y)
{
	x = fabsf(x - 0.5f) + 0.5f;
	Result a = { SDF::capsuleSDF(x, y, 0.75f, 0.25f, 0.75f, 0.75f, 0.05f), glm::vec3(0.0f), 0.2f, 1.5f };
	Result b = { SDF::capsuleSDF(x, y, 0.75f, 0.25f, 0.50f, 0.75f, 0.05f), glm::vec3(0.0f), 0.2f, 1.5f };
	y = fabsf(y - 0.5f) + 0.5f;
	Result c = { SDF::circleSDF(x, y, 1.05f, 1.05f, 0.05f), glm::vec3(5.0f, 0.0f, 0.0f), 0.0f, 0.0f };
	return SDF::unionOperation(a, SDF::unionOperation(b, c));
}

Result Scene::beerLambertScene(float x, float y)
{
	Result a = { SDF::circleSDF(x, y, 0.3f, -0.2f, 0.1f), glm::vec3(10.0f),
		0.0f, 0.0f, glm::vec3(0.0f) };
	Result b = { SDF::ngonSDF(x, y, 0.5f, 0.5f, 0.25f, 8.0f), glm::vec3(0.0f),
		0.0f, 1.5f, glm::vec3(4.0f, 1.0f, 4.0f)};
	return SDF::unionOperation(a, b);
}

Result Scene::heartScene(float x, float y)
{
	float u = x - 0.5f, v = y - 0.5f, 
		t = fmodf(atan2f(v, u) + M_2PI, M_2PI / 16),
		s = sqrtf(u * u + v * v);
	x = fabsf(x - 0.5f) + 0.5f;
	glm::vec3 m(1.0f, 4.0f, 4.0f);
	Result a = { SDF::ngonSDF(x, y, 0.7f, 0.35f, 0.2f, 16), glm::vec3(0.0f), 0.0f, 1.77f, m };
	Result b = { SDF::ngonSDF(x, y, 0.35f, 0.35f, 0.55f, 32), glm::vec3(0.0f), 0.0f, 1.77f, m };
	Result c = { SDF::planeSDF(x, y, 0.5f, 0.35f, 0.0f, -1.0f), glm::vec3(0.0f), 0.0f, 1.77f, m };
	Result d = { 
		SDF::circleSDF(s * cosf(t), s * sinf(t),
		0.6f * cosf(M_2PI / 32), 0.5f * sinf(M_2PI / 32), 0.05f),
		glm::vec3(2.0f), 0.0f, 0.0f, glm::vec3(0.0f) };
	return SDF::unionOperation(SDF::unionOperation(a, SDF::intersectOperation(b, c)), d);
}

Result Scene::finalScene(float x, float y)
{
	Result ret1 = Result(SDF::circleSDF(x, y, 0.15f, 0.15f, 0.1f), glm::vec3(7.0), 0.0f);
	Result ret2 = Result(SDF::boxSDF(x, y, 0.2f, 0.8f, M_2PI / 16.0f, 0.1f, 0.1f), glm::vec3(0.0f), 0.5f, 1.5f,
		glm::vec3(4.0, 1.0, 1.0));
	Result ret3 = Result(SDF::boxSDF(x, y, 0.2f, 0.5f, M_2PI / 8.0f, 0.1f, 0.1f), glm::vec3(0.0f), 0.0f, 1.5f,
		glm::vec3(1.0, 4.0, 4.0));
	Result ret4 = Result(SDF::circleSDF(x, y, 0.4f, 0.1f, 0.1f), glm::vec3(0.0f), 0.1f, 1.5f,
		glm::vec3(4.0, 1.0, 4.0));
	Result ret5 = { SDF::ngonSDF(x, y, 0.8f, 0.8f, 0.10f, 8.0f), glm::vec3(0.0f),
		0.0f, 1.5f, glm::vec3(4.0f, 4.0f, 1.0f) };
	Result ret6 = { SDF::triangleSDF(x, y, 0.9f, 0.2f, 0.7f, 0.4f, 0.8f, 0.3f), glm::vec3(0.0f),
	0.2f, 1.5f, glm::vec3(4.0f, 4.0f, 1.0f) };
	Result ret7 = Result(SDF::capsuleSDF(x, y, 0.3f, 0.3f, 0.5f, 0.5f, 0.05f), glm::vec3(0.0f), 0.5f,
		1.5f, glm::vec3(1.0, 1.0, 4.0));
	Result ret8 = Result(SDF::YcharSDF(x, y, 0.6, 0.3, 0.02f), glm::vec3(3.0f, 0.0f, 0.0f), 0.0f);
	Result ret9 = Result(SDF::WcharSDF(x, y, 0.8, 0.55, 0.02f), glm::vec3(0.0f, 3.0f, 0.0f), 0.0f);
	Result ret10 = Result(SDF::CcharSDF(x, y, 0.5, 0.7, 0.02f), glm::vec3(0.0f, 0.0f, 3.0f), 0.0f);
	Result tmp = SDF::unionOperation(SDF::unionOperation(ret1, ret2), ret3);
	Result tmp1 = SDF::unionOperation(tmp, SDF::unionOperation(ret4, ret5));
	Result tmp2 = SDF::unionOperation(tmp1, SDF::unionOperation(ret6, ret7));
	Result tmp3 = SDF::unionOperation(tmp2, SDF::unionOperation(ret8, ret9));
	Result tmp4 = SDF::unionOperation(tmp3, ret10);
	return tmp4;
}

Result Scene::nameScene(float x, float y)
{
	Result yc = Result(SDF::YcharSDF(x, y, 0.2 + 0.05, 0.3, 0.01f), glm::vec3(3.5f, 0.0f, 0.0f), 0.0f);
	Result ac = Result(SDF::AcharSDF(x, y, 0.35 + 0.05, 0.3, 0.01f), glm::vec3(3.5f, 3.5f, 0.0f), 0.0f);
	Result nc = Result(SDF::NcharSDF(x, y, 0.50 + 0.05, 0.3, 0.01f), glm::vec3(0.0f, 3.5f, 3.5f), 0.0f);
	Result gc = Result(SDF::GcharSDF(x, y, 0.65 + 0.05, 0.3, 0.01f), glm::vec3(3.4f, 1.64f, 0.76f), 0.0f);
	Result wc = Result(SDF::WcharSDF(x, y, 0.35 + 0.05, 0.55, 0.01f), glm::vec3(0.0f, 3.5f, 0.0f), 0.0f);
	Result cc = Result(SDF::CcharSDF(x, y, 0.50 + 0.05, 0.55, 0.01f), glm::vec3(3.5f, 3.5f, 3.5f), 0.0f);

	Result s1c = Result(SDF::ScharSDF(x, y, 0.2 + 0.05, 0.80, 0.01f), glm::vec3(0.0f),
		0.1f, 1.77f, glm::vec3(1.0f, 4.0, 4.0));
	Result s2c = Result(SDF::YcharSDF(x, y, 0.35 + 0.05, 0.80, 0.01f), glm::vec3(0.0f),
		0.1f, 1.77f, glm::vec3(1.0f, 4.0, 4.0));
	Result s3c = Result(SDF::ScharSDF(x, y, 0.50 + 0.05, 0.80, 0.01f), glm::vec3(0.0f),
		0.1f, 1.77f, glm::vec3(1.0f, 4.0, 4.0));
	Result s4c = Result(SDF::UcharSDF(x, y, 0.65 + 0.05, 0.80, 0.01f), glm::vec3(0.0f),
		0.1f, 1.77f, glm::vec3(1.0f, 4.0, 4.0));

	Result logo = { SDF::ngonSDF(x, y, 0.15f, 0.15f, 0.10f, 8.0f), glm::vec3(0.0f),
		0.0f, 1.5f, glm::vec3(4.0f, 4.0f, 1.0f) };
	Result light = Result(SDF::circleSDF(x, y, 0.8f, 0.85f, 0.025f), glm::vec3(8.0), 0.0f);
	Result ret1 = SDF::unionOperation(yc, ac);
	Result ret2 = SDF::unionOperation(wc, cc);
	Result ret3 = SDF::unionOperation(nc, gc);
	Result ret4 = SDF::unionOperation(s1c, s2c);
	Result ret5 = SDF::unionOperation(s3c, s4c);
	Result f1 = SDF::unionOperation(ret1, ret2);
	Result f2 = SDF::unionOperation(ret3, ret4);
	Result f3 = SDF::unionOperation(ret5, logo);
	return SDF::unionOperation(SDF::unionOperation(f1, f2), SDF::unionOperation(f3, light));
}
