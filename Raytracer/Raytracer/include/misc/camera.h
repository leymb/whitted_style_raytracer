#pragma once
#include "vec3.h"


struct ray;

struct camera
{
	camera(vec3 aPosition = { 0,0,0 }, vec3 aViewDirection = { 0,0,1 });

	vec3 m_v3Position;
	vec3 m_v3Direction;

	vec3 m_v3ViewportP0{};
	vec3 m_v3ViewportP1{};
	vec3 m_v3ViewportP2{};

	ray CreateRay(float u, float v) const;
};

