#pragma once
#include "primitive.h"

struct intersectionData;

class sphere :
    public primitive
{
public:

	// functions
    sphere(vec3 aPosition, float aRadius);
    bool RayIntersection(ray& rPrimaryRay, intersectionData& isdData);
    vec3 GetIntersectionNormal(vec3& intersectionPoint) override;
    vec3 GetScreenspaceCoordinate(vec3& v3Origin, vec3& v3Left, vec3& v3Right, vec3& v3Up, vec3& v3Down, int iScrwidth, int iScrheight, float fViewportWidth, float& RadiusInPixels);
	
    // basic members defining a plane
    float m_fRadius = 0.0f;
};

