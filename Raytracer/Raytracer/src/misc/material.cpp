#include "precomp.h"

material::material(vec3 aColor, float aReflectionIndex, float aRefractionIndex, float aDiffusionValue)
	: m_v3Color(aColor), m_fReflectionIndex(aReflectionIndex), m_fRefractionIndex(aRefractionIndex), fDiffusion(aDiffusionValue)
{
	
}
