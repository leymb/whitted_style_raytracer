#include "precomp.h"
intersectionData::intersectionData(bool aBool, vec3 aIntersectionPoint, vec3 aIntersectionNormal, material maAMaterial)
	: m_bIntersectionDetected(aBool), m_v3IntersectionPoint(aIntersectionPoint),
	  m_v3IntersectionNormal(aIntersectionNormal), m_maTransportedMaterial(maAMaterial)
{
	
}