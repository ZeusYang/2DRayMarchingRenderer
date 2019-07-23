#pragma once

#include "SDF.h"

class Scene
{
public:
	static Result oneEmissiveSphereScene(float x, float y);
	static Result threeEmissiveSphereScene(float x, float y);
	static Result moonEmissiveScene(float x, float y);
	static Result planeEmissiveScene(float x, float y);
	static Result capsuleEmissiveScene(float x, float y);
	static Result boxEmissiveScene(float x, float y);
	static Result triangleEmissiveScene(float x, float y);
	static Result reflectEmissiveScene(float x, float y);
	static Result causticEmissiveScene(float x, float y);
	static Result rectRefractEmissiveScene(float x, float y);
	static Result refractEmissiveScene(float x, float y);
	static Result beerLambertScene(float x, float y);
	static Result heartScene(float x, float y);
	static Result finalScene(float x, float y);
	static Result nameScene(float x, float y);
};

