#include "precomp.h"


scene::scene()
{
	
}

scene::~scene()
{
	delete [] &m_prPrimitives;
	delete [] &m_lsLightSources;
}

void scene::AddPrimitive(primitive& aPrimitive)
{
	
	m_iPrimitiveCounter += 1;
	
	if (m_prPrimitives[m_iPrimitiveCounter] == nullptr)
	{
		
		m_prPrimitives[m_iPrimitiveCounter] = &aPrimitive;
		
	} else
	{
		AddPrimitive(aPrimitive);
	}
	
}

intersectionData scene::GetIntersection(ray& aRay)
{
	intersectionData isdTempData{};
	
	for (int i = 0; i < m_iPrimitiveCounter; i++)
	{
		const float fTempDistance = aRay.m_fDistance;
		m_prPrimitives[i]->RayIntersection(aRay, isdTempData);
		
		if ((isdTempData.m_bIntersectionDetected)
			&& (aRay.m_fDistance < fTempDistance))
		{

			aRay.m_iNearestPrimitiveIndex = i;

		}
	}

	return isdTempData;
}
