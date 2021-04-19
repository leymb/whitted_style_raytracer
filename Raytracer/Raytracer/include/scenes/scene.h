#pragma once
#include "misc/lightsource.h"

class scene
{
public:
	scene();
	~scene();
	void AddPrimitive(primitive& aPrimitive);
	intersectionData GetIntersection(ray& aRay);

	int m_iPrimitiveCounter = 0;
	primitive* m_prPrimitives[100]{nullptr};

	int m_iLightCounter = 1;
	lightSource* m_lsLightSources[100]{ nullptr };
	
	lightSource m_lsLight{{0.0, 6.0, 5.0}};
}; 
