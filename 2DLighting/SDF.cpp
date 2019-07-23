#include "SDF.h"

#include <algorithm>

#define M_2PI 6.28318530718f

float SDF::circleSDF(float x, float y, float cx, float cy, float radius)
{
	float ux = x - cx, uy = y - cy;
	return sqrtf(ux * ux + uy * uy) - radius;
}

float SDF::planeSDF(float x, float y, float px, float py, float nx, float ny)
{
	return (x - px)*nx + (y - py)*ny;
}

float SDF::segmentSDF(float x, float y, float ax, float ay, float bx, float by)
{
	float vx = x - ax, vy = y - ay;
	float ux = bx - ax, uy = by - ay;
	float t = fmaxf(fminf((vx * ux + vy * uy) / (ux * ux + uy * uy), 1.0f), 0.0f);
	float dx = vx - ux * t, dy = vy - uy * t;
	return sqrtf(dx * dx + dy * dy);
}

float SDF::capsuleSDF(float x, float y, float ax, float ay, float bx, float by, float radius)
{
	return segmentSDF(x, y, ax, ay, bx, by) - radius;
}

float SDF::boxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy)
{
	float cosTheta = cosf(theta), sinTheta = sinf(theta);
	float dx = fabs((x - cx) * cosTheta + (y - cy)*sinTheta) - sx;
	float dy = fabs((y - cy) * cosTheta - (x - cx)*sinTheta) - sy;
	float ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
	return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
}

float SDF::triangleSDF(float x, float y, float ax, float ay, float bx, float by, float cx, float cy)
{
	float d = fminf(fminf(
		segmentSDF(x, y, ax, ay, bx, by),
		segmentSDF(x, y, bx, by, cx, cy)),
		segmentSDF(x, y, cx, cy, ax, ay));
	return 
		(bx - ax) * (y - ay) > (by - ay) * (x - ax) &&
		(cx - bx) * (y - by) > (cy - by) * (x - bx) &&
		(ax - cx) * (y - cy) > (ay - cy) * (x - cx) ? -d : d;
}

float SDF::ngonSDF(float x, float y, float cx, float cy, float r, float n) {
	float ux = x - cx, uy = y - cy, a = M_2PI / n;
	float t = fmodf(atan2f(uy, ux) + M_2PI, a), s = sqrtf(ux * ux + uy * uy);
	return planeSDF(s * cosf(t), s * sinf(t), r, 0.0f, cosf(a * 0.5f), sinf(a * 0.5f));
}

float SDF::YcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[4];
	point[0] = glm::vec2(-0.05, -0.05) + glm::vec2(cx, cy);
	point[1] = glm::vec2(0.0, 0.0) + glm::vec2(cx, cy);
	point[2] = glm::vec2(+0.05, -0.05) + glm::vec2(cx, cy);
	point[3] = glm::vec2(0.0, 0.08) + glm::vec2(cx, cy);
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
	float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[2].x, point[2].y, radius);
	float c3 = capsuleSDF(x, y, point[1].x, point[1].y, point[3].x, point[3].y, radius);
	return fminf(fminf(c1, c2), c3);
}

float SDF::AcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[5];
	point[0] = glm::vec2(+0.00f, -0.055f) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.05f, +0.072f) + glm::vec2(cx, cy);
	point[2] = glm::vec2(+0.05f, +0.072f) + glm::vec2(cx, cy);
	point[3] = point[0] + (point[1] - point[0]) * 0.9f;
	point[4] = point[0] + (point[2] - point[0]) * 0.9f;
	point[3].x += 0.03f;
	point[4].x -= 0.03f;
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
	float c2 = capsuleSDF(x, y, point[0].x, point[0].y, point[2].x, point[2].y, radius);
	float c3 = capsuleSDF(x, y, point[3].x, point[3].y, point[4].x, point[4].y, radius);
	float c4 = fminf(c1, c2);
	return fminf(c4, c3);
}

float SDF::NcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[4];
	point[0] = glm::vec2(+0.05, -0.06) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.05, -0.06) + glm::vec2(cx, cy);
	point[2] = glm::vec2(-0.05, +0.072) + glm::vec2(cx, cy);
	point[3] = glm::vec2(+0.05, +0.072) + glm::vec2(cx, cy);
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[3].x, point[3].y, radius);
	float c2 = capsuleSDF(x, y, point[3].x, point[3].y, point[1].x, point[1].y, radius);
	float c3 = capsuleSDF(x, y, point[1].x, point[1].y, point[2].x, point[2].y, radius);
	return fminf(fminf(c1, c2), c3);
}

float SDF::GcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[6];
	point[0] = glm::vec2(+0.05, -0.06) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.05, -0.06) + glm::vec2(cx, cy);
	point[2] = glm::vec2(-0.05, +0.072) + glm::vec2(cx, cy);
	point[3] = glm::vec2(+0.05, +0.072) + glm::vec2(cx, cy);
	point[4] = (point[0] + point[3]) * 0.5f;
	point[5] = glm::vec2((point[0].x + point[1].x) * 0.5f, (point[1].y + point[2].y)*0.5f);
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
	float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[2].x, point[2].y, radius);
	float c3 = capsuleSDF(x, y, point[2].x, point[2].y, point[3].x, point[3].y, radius);
	float c4 = capsuleSDF(x, y, point[3].x, point[3].y, point[4].x, point[4].y, radius);
	float c5 = capsuleSDF(x, y, point[4].x + 0.02f, point[4].y, point[5].x + 0.02f, point[5].y, radius);
	float c6 = fminf(c1, c2);
	float c7 = fminf(c3, c4);
	return fminf(fminf(c6, c7), c5);
}

float SDF::WcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[5];
	point[0] = glm::vec2(0.0, -0.06) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.03, +0.06) + glm::vec2(cx, cy);
	point[2] = glm::vec2(+0.03, +0.06) + glm::vec2(cx, cy);
	point[3] = glm::vec2(-0.06, -0.06) + glm::vec2(cx, cy);
	point[4] = glm::vec2(+0.06, -0.06) + glm::vec2(cx, cy);
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
	float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[3].x, point[3].y, radius);
	float c3 = capsuleSDF(x, y, point[0].x, point[0].y, point[2].x, point[2].y, radius);
	float c4 = capsuleSDF(x, y, point[2].x, point[2].y, point[4].x, point[4].y, radius);
	return fminf(fminf(c1, c2), fminf(c3, c4));
}

float SDF::CcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[4];
	point[0] = glm::vec2(+0.05, -0.06) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.05, -0.06) + glm::vec2(cx, cy);
	point[2] = glm::vec2(-0.05, +0.06) + glm::vec2(cx, cy);
	point[3] = glm::vec2(+0.05, +0.06) + glm::vec2(cx, cy);
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
	float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[2].x, point[2].y, radius);
	float c3 = capsuleSDF(x, y, point[2].x, point[2].y, point[3].x, point[3].y, radius);
	return fminf(fminf(c1, c2), c3);
}

float SDF::ScharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[6];
	point[0] = glm::vec2(+0.05, -0.06) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.05, -0.06) + glm::vec2(cx, cy);
	point[2] = glm::vec2(-0.05, +0.072) + glm::vec2(cx, cy);
	point[3] = glm::vec2(+0.05, +0.072) + glm::vec2(cx, cy);
	point[4] = (point[0] + point[3]) * 0.5f;
	point[5] = (point[1] + point[2]) * 0.5f;
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[1].x, point[1].y, radius);
	float c2 = capsuleSDF(x, y, point[1].x, point[1].y, point[5].x, point[5].y, radius);
	float c3 = capsuleSDF(x, y, point[5].x, point[5].y, point[4].x, point[4].y, radius);
	float c4 = capsuleSDF(x, y, point[4].x, point[4].y, point[3].x, point[3].y, radius);
	float c5 = capsuleSDF(x, y, point[3].x, point[3].y, point[2].x, point[2].y, radius);
	float c6 = fminf(c1, c2);
	float c7 = fminf(c3, c4);
	return fminf(fminf(c6, c7), c5);
}

float SDF::UcharSDF(float x, float y, float cx, float cy, float radius)
{
	glm::vec2 point[4];
	point[0] = glm::vec2(+0.05, -0.06) + glm::vec2(cx, cy);
	point[1] = glm::vec2(-0.05, -0.06) + glm::vec2(cx, cy);
	point[2] = glm::vec2(-0.05, +0.072) + glm::vec2(cx, cy);
	point[3] = glm::vec2(+0.05, +0.072) + glm::vec2(cx, cy);
	float c1 = capsuleSDF(x, y, point[0].x, point[0].y, point[3].x, point[3].y, radius);
	float c2 = capsuleSDF(x, y, point[3].x, point[3].y, point[2].x, point[2].y, radius);
	float c3 = capsuleSDF(x, y, point[1].x, point[1].y, point[2].x, point[2].y, radius);
	return fminf(fminf(c1, c2), c3);
}

Result SDF::unionOperation(Result a, Result b)
{
	return a.sdf < b.sdf ? a : b;
}

Result SDF::intersectOperation(Result a, Result b)
{
	Result ret = a.sdf < b.sdf ? b : a;
	//ret.sdf = a.sdf > b.sdf ? a.sdf : b.sdf;
	return ret;
}

Result SDF::subtractOperation(Result a, Result b)
{
	Result ret = a;
	ret.sdf = (a.sdf > -b.sdf) ? a.sdf : -b.sdf;
	return ret;
}
