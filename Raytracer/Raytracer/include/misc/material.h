#pragma once
struct material
{
	material(vec3 aColor = { 0,0,0 }, float aReflectionIndex = 0.0, float aRefractionIndex = 1.1, float aDiffusionValue = 0.9);
	vec3 m_v3Color{};
	float m_fReflectionIndex;
	float m_fRefractionIndex;
	float fDiffusion;
};
