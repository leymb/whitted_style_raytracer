#include "precomp.h"


plane::plane(vec3 aPosition, vec3 aNormal, float aDistance)
{
	m_v3Position = aPosition;
	m_v3Normal = aNormal;
	m_fDistanceFromOrigin = aDistance;
	m_maObjectMaterial = { {1.0f,0.05f,0.8f}, 0.5 };

	m_iPrimitiveType = planar;
};

bool plane::RayIntersection(ray& rPrimaryRay, intersectionData& isdData)
{
	// calculating cosine of the angle between the normal & the ray direction
	const auto denom = m_v3Normal.DotWith(rPrimaryRay.m_v3Direction);

	//if the ray direction and the planes normal are not orthogonal
	if (fabsf(denom) > 0.0001f)
	{
		// calculating t by dotting the vector between the position of the plane and
		// the origin of the ray with the object normal and dividing it by the
		// denominator

		const auto t = -(rPrimaryRay.m_v3Origin.DotWith(m_v3Normal) + m_fDistanceFromOrigin) / rPrimaryRay.m_v3Direction.DotWith(m_v3Normal);
		
		//const auto t = (m_v3Position - rPrimaryRay.m_v3Origin).DotWith(m_v3Normal) / denom;
		if (t > 0)
		{

			
			// calculating the point of intersection by calculating the displacement from
			// the origin by multiplying t (a scalar) with the (normalized) direction
			vec3 pos = rPrimaryRay.m_v3Origin + rPrimaryRay.m_v3Direction * t;
			

			// check if distance from origin of ray is smaller than the one already stored
			// in the ray. If yes, set distance of ray to tempDistance && set intersection
			// data to new intersection data
			const float fTempDistance = (pos - rPrimaryRay.m_v3Origin).GetMagnitude();
			if (fTempDistance < rPrimaryRay.m_fDistance)
			{
				rPrimaryRay.m_fDistance = fTempDistance;

				const vec3 v3IntersectNormal = GetIntersectionNormal(pos);
				
				isdData = {true, pos, v3IntersectNormal, m_maObjectMaterial};
				
				// change color depending on coordinates of intersection
				if (1 & (static_cast<int>((pos.x + 100000)) + static_cast<int>(pos.z + 100000)))
				{
					isdData.m_maTransportedMaterial.m_v3Color = vec3{ 0.5f,0.5f,0.5f };
				}
				else isdData.m_maTransportedMaterial.m_v3Color = vec3{ 1.0f,1.0f,1.0f };
				
			}

			isdData.m_bIntersectionDetected = true;
			return true;
		}
	}

	isdData.m_bIntersectionDetected = false;
	return false;
}

vec3 plane::GetIntersectionNormal(vec3& )
{
	return m_v3Normal;
}

vec3 plane::GetScreenspaceCoordinate(vec3& , vec3& , vec3& , vec3& , vec3& , int , int , float , float& )
{
	return {};
}
