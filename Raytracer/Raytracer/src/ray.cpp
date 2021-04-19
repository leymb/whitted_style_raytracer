#include "precomp.h"


ray::ray(vec3 aOrigin, vec3 aDirection)
{
	m_v3Origin = aOrigin;
	m_v3Direction = aDirection.Normalize();
}
