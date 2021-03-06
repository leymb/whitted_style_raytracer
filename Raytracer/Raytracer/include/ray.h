#pragma once


struct ray
{
	ray(vec3 aOrigin = {0,0,0}, vec3 aDirection = {0,0,1});

	// basic members defining a ray
	vec3 m_v3Origin{};
	vec3 m_v3Direction{ 0,0,1 };

	// tracks if ray is a primary ray && the pixel it is sent through
	// used in determining which intersect to use
	bool m_bIsPrimaryRay = false;
	int m_iPixelX = -1;
	int m_iPixelY = -1;
	

	// members used in determining intersections with primitives
	int m_iNearestPrimitiveIndex = -404;
	float m_fDistance = 10000000;

	// refraction index of material the ray is in
	float m_fCurrentRefractionIndex = 1.10f;

	bool m_bInPrimitive = false;
};

