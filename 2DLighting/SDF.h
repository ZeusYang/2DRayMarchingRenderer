#pragma once

#include <glm/glm.hpp>

struct Result
{
	float sdf;
	glm::vec3 emissive;
	float reflectivity;
	float eta;
	glm::vec3 absorption;

	Result() {}

	Result(float s, glm::vec3 e, float r, float et = 0.0f, glm::vec3 absor = glm::vec3(0.0f))
	{
		sdf = s;
		emissive = e;
		reflectivity = r;
		eta = et;
		absorption = absor;
	}
};

class SDF
{
public:
	// several shape sdf.
	static float circleSDF(float x, float y, float cx, float cy, float radius);
	static float planeSDF(float x, float y, float px, float py, float nx, float ny);
	static float segmentSDF(float x, float y, float ax, float ay, float bx, float by);
	static float capsuleSDF(float x, float y, float ax, float ay, float bx, float by, float radius);
	static float boxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy);
	static float triangleSDF(float x, float y, float ax, float ay, float bx, float by, float cx, float cy);
	static float ngonSDF(float x, float y, float cx, float cy, float r, float n);
	static float YcharSDF(float x, float y, float cx, float cy, float radius);
	static float AcharSDF(float x, float y, float cx, float cy, float radius);
	static float NcharSDF(float x, float y, float cx, float cy, float radius);
	static float GcharSDF(float x, float y, float cx, float cy, float radius);
	static float WcharSDF(float x, float y, float cx, float cy, float radius);
	static float CcharSDF(float x, float y, float cx, float cy, float radius);
	static float ScharSDF(float x, float y, float cx, float cy, float radius);
	static float UcharSDF(float x, float y, float cx, float cy, float radius);

	// sdf operation.
	static Result unionOperation(Result a, Result b);
	static Result intersectOperation(Result a, Result b);
	static Result subtractOperation(Result a, Result b);
};

