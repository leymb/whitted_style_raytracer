#pragma once
#include "misc/cell.h"
#include "primitives/sphere.h"
#include "scenes/scene.h"

struct lightSource;
struct camera;
struct rgbColor;

class renderer
{
public:
	renderer();
	~renderer();

	void Update();

	const int SCRWIDTH = 640;
	const int SCRHEIGHT = 640;

	const int CELLAMOUNT_WIDTH = 10;
	const int CELLAMOUNT_HEIGHT = 10;

private:

	void CheckForPrimaryIntersections(ray& rAPrimaryRay, intersectionData& isdIntersectionData) const;
	void CheckForIntersection(ray& rAPrimaryRay, intersectionData& isdIntersectionData) const;
	ray CastShadowRay(vec3& aOrigin, vec3& aLightPosition, vec3& aCollideNormal, intersectionData& isdIntersectionData) const;
	vec3 CalculateShadows(intersectionData& isdData) const;
	
	vec3 CalculateDiffuseColor(bool bShadowRayOccluded, ray& rAShadowRay, lightSource& lsALightSource, vec3& v3AIntersectionNormal, material& maObjectMaterial) const;

	static float Fresnel(vec3& v3IncomingDirection, vec3& v3OutgoingDirection, vec3& v3IntersectionNormal, float fRefractIndexOne, float fRefractIndex2);
	
	vec3 Trace(ray& rPrimaryRay, int iRecursionCap);
	static vec3 ReflectRay(vec3& v3Direction, vec3& v3IntersectionNormal);
	static vec3 RefractRay(vec3& v3Direction, vec3& v3IntersectionNormal, float fInsideRefraction, float fOutsideRefraction = 1.0f);

	void ConstructGrid() const;

	// members used for acceleration
	const float CELLWIDTH = static_cast<float>(SCRWIDTH / CELLAMOUNT_WIDTH);
	const float CELLHEIGHT = static_cast<float>(SCRHEIGHT / CELLAMOUNT_HEIGHT);
	cell* m_p_cScreenCells = new cell[static_cast<__int64>(CELLAMOUNT_HEIGHT) * CELLAMOUNT_WIDTH];
	
	int m_iInterlaceStride = 64;
	int m_iCurrentInterlace = 0;

	// members needed for SDL2
	SDL_Renderer* m_p_reMainRenderer = nullptr;
	SDL_Texture* m_p_tMainTexture = nullptr;
	SDL_Window* m_p_wMainWindow = nullptr;

	// members needed directly in rendering
	camera* m_p_cMainCamera = nullptr;
	
	Uint32* m_piPixels = new Uint32[static_cast<__int64>(SCRWIDTH) * SCRHEIGHT];

	int iSceneCounter = 0;
	scene* m_p_AllScenes[10]{nullptr};
	scene* m_p_scActiveScene = m_p_AllScenes[0];
};

