#pragma once
#include "vec3.h"
#include "misc/material.h"


struct ray;
struct intersectionData;

class primitive
{
public:

	// functions
	virtual bool RayIntersection(ray& rPrimaryRay, intersectionData& isdData) = 0;
	virtual vec3 GetIntersectionNormal(vec3& intersectionPoint);
	virtual vec3 GetScreenspaceCoordinate(vec3& v3Origin, vec3& v3Left, vec3& v3Right, vec3& v3Up, vec3& v3Down, int iScrwidth, int iScrheight, float fViewportWidth, float& RadiusInPixels) = 0;

	// basic members defining a primitive
	vec3 m_v3Position = { 0,0,0 };
	material m_maObjectMaterial = {};

	int m_iPrimitiveType = 0;

	enum m_eTypes
	{
		planar = 1,
		spherical = 2
	};
};

