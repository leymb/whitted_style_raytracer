#pragma once
struct intersectionData
{
	intersectionData(bool aBool = false, vec3 aIntersectionPoint = {}, vec3 aIntersectionNormal = {}, material maAMaterial = {});
	
	bool m_bIntersectionDetected{false};

	//I
	vec3 m_v3IntersectionPoint{0,0,0};
	//N
	vec3 m_v3IntersectionNormal{};
	//material
	material m_maTransportedMaterial = {};
};
