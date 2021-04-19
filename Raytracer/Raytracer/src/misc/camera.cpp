#include "precomp.h"


camera::camera(const vec3 aPosition, const vec3 aViewDirection)
{
	m_v3Direction = aViewDirection;
	m_v3Direction.Normalize();

	m_v3Position = aPosition;

	// normally c = cameraPos + distance * direction, but in this case, distance = 1
	vec3 c = m_v3Position + m_v3Direction;

	m_v3ViewportP0 = c + vec3(-1, 1, -2);
	m_v3ViewportP1 = c + vec3(1, 1, -2);
	m_v3ViewportP2 = c + vec3(-1, -1, -2);


}

ray camera::CreateRay(const float u, const float v) const
{
	// creating ray
	const vec3 v3Direction = { m_v3ViewportP0 + ((m_v3ViewportP1 - m_v3ViewportP0) * u) + ((m_v3ViewportP2 - m_v3ViewportP0) * v) };
	ray rPrimaryRay{ m_v3Position, v3Direction};

	// filling in variables used for screenspace grid
	rPrimaryRay.m_bIsPrimaryRay = true;
	
	return rPrimaryRay;
}

