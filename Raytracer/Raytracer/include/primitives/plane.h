#pragma once
#include "primitive.h"

struct ray;

class plane :
    public primitive
{
public:

	// functions
    plane(vec3 aPosition = { 0,0,0 }, vec3 aNormal = {0,1,0}, float aDistance = 0);
    bool RayIntersection(ray& rPrimaryRay, intersectionData& isdData) override;
    vec3 GetIntersectionNormal(vec3& intersectionPoint) override;
    vec3 GetScreenspaceCoordinate(vec3& v3Origin, vec3& v3Left, vec3& v3Right, vec3& v3Up, vec3& v3Down, int iScrwidth, int iScrheight, float fViewportWidth, float& RadiusInPixels) override;

	// basic members defining a plane
    vec3 m_v3Normal{0.0f, 0.0f, 0.0f};
    float m_fDistanceFromOrigin = 0;
};

