#pragma once
struct lightSource
{
	vec3 m_v3Position{};
	vec3 m_v3Direction{};
	vec3 m_v3LightColor{};
	
	float m_fLightStrength{1.0f};
};

