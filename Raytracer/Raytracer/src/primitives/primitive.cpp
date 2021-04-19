#include "precomp.h"
#include "primitives/primitive.h"


vec3 primitive::GetIntersectionNormal(vec3& intersectionPoint)
{
	return intersectionPoint - m_v3Position;
}
