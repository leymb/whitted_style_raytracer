#include "precomp.h"

#include "mathm.h"

renderer::renderer()
{
    SDL_Init(SDL_INIT_VIDEO);
    m_p_wMainWindow = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCRWIDTH, SCRHEIGHT, SDL_WINDOW_SHOWN);
    m_p_reMainRenderer = SDL_CreateRenderer(m_p_wMainWindow, -1, SDL_RENDERER_ACCELERATED);
    m_p_tMainTexture = SDL_CreateTexture(m_p_reMainRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCRWIDTH, SCRHEIGHT);
    m_p_cMainCamera = new camera{};

	m_p_AllScenes[0] = new testscene{};
	m_p_scActiveScene = m_p_AllScenes[0];
	iSceneCounter = 1;

	ConstructGrid();
}

renderer::~renderer()
{
	delete [] &m_p_AllScenes;
	delete [] &m_p_cScreenCells;
    SDL_DestroyTexture(m_p_tMainTexture);
    SDL_DestroyWindow(m_p_wMainWindow);
    SDL_DestroyRenderer(m_p_reMainRenderer);
}

void renderer::Update()
{
	// The way to access the pixel was taken from https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/  
	// because it was the least complex way of coloring single pixels I could find
	
	#pragma omp parallel for
	for (int y = m_iCurrentInterlace; y < SCRHEIGHT; y += m_iInterlaceStride)
	{
		
		for (int x = 0; x < SCRWIDTH; x++)
		{
			// Creating primary ray
			ray rPrimaryRay = m_p_cMainCamera->CreateRay((static_cast<float>(x) / (SCRWIDTH - 1)),
			                                                   (static_cast<float>(y) / (SCRHEIGHT - 1)));
			rPrimaryRay.m_iPixelX = x;
			rPrimaryRay.m_iPixelY = y;

			// Tracing primary ray to get the pixels color. Since all my colors are float values 
			// between 0-1 I convert them to int with a range of 0-255. Doing this only once also
			// results in me only clamping the values once
			const rgbColor rgbTempColor(Trace(rPrimaryRay, 5));

			// This is taken from Jacco Bikkers "Hitting the ground running lecture"
			m_piPixels[y * SCRWIDTH + x] = rgbTempColor.b + (rgbTempColor.g << 8) + (rgbTempColor.r << 16);

		}
	}

	// increment stride so that interlacing renders next part of screen
	m_iCurrentInterlace++;
	m_iCurrentInterlace %= m_iInterlaceStride;
	
	// Updates the texture with the pixels array 
	SDL_UpdateTexture(m_p_tMainTexture, nullptr, m_piPixels, (SCRWIDTH * sizeof(Uint32)));
	
	// Rendering created texture to screen
	SDL_RenderCopy(m_p_reMainRenderer, m_p_tMainTexture, nullptr, nullptr);
	SDL_RenderPresent(m_p_reMainRenderer);
}


void renderer::CheckForPrimaryIntersections(ray& rAPrimaryRay, intersectionData& isdIntersectionData) const
{
	const int iPixelX = rAPrimaryRay.m_iPixelX;
	const int iPixelY = rAPrimaryRay.m_iPixelY;

	// calculating which cell the current pixel is in
	// by dividing the 
	const int iCellX = static_cast<int>(iPixelX / CELLWIDTH);
	const int iCellY = static_cast<int>(iPixelY / CELLHEIGHT);
	
	// get current cells primitive counters
	const int iPrimitiveCounter = m_p_cScreenCells[iCellX + iCellY * CELLAMOUNT_WIDTH].m_iPrimCounter;

	for (int i = 0; i <= iPrimitiveCounter; i++)
	{
		const int iPrimitiveIndex = m_p_cScreenCells[iCellX + iCellY * CELLAMOUNT_WIDTH].m_iPrimitiveIndices[i];

		const float fTempDistance = rAPrimaryRay.m_fDistance;

		if (m_p_scActiveScene->m_prPrimitives[iPrimitiveIndex]->RayIntersection(rAPrimaryRay, isdIntersectionData)
			&& (rAPrimaryRay.m_fDistance < fTempDistance))
		{

			rAPrimaryRay.m_iNearestPrimitiveIndex = i;

		}
	}
}

void renderer::CheckForIntersection(ray& rARay, intersectionData& isdIntersectionData) const
{
	// Gets the current scenes primitive counter
	const int iObjectCounter = m_p_scActiveScene->m_iPrimitiveCounter;

	// Iterates over all primitives in the active scene and checks if
	// ray intersects with them

		for (int i = 0; i < iObjectCounter; i++)
		{
			const float fTempDistance = rARay.m_fDistance;

			if (m_p_scActiveScene->m_prPrimitives[i]->RayIntersection(rARay, isdIntersectionData)
				&& (rARay.m_fDistance < fTempDistance))
			{

				rARay.m_iNearestPrimitiveIndex = i;

			}
		}
}

ray renderer::CastShadowRay(vec3& aOrigin, vec3& aLightPosition, vec3& aCollideNormal, intersectionData& isdIntersectionData) const
{
    vec3 v3Direction = aLightPosition - aOrigin;
    ray rShadowRay{};

	// If IntersectionNormal.DotWith(vector towards light) < 0, then light source is 
	// behind th object the ray intersected with -> do not send out shadow ray
	
    if (aCollideNormal.DotWith(v3Direction) < 0)
    {
        isdIntersectionData.m_bIntersectionDetected = true;
        return rShadowRay;
    }

	// Send out shadow ray and check for intersection
	rShadowRay = {aOrigin + aCollideNormal * 0.0001f, v3Direction};

    CheckForIntersection(rShadowRay, isdIntersectionData);

    if (isdIntersectionData.m_bIntersectionDetected == true)
    {
	    if (rShadowRay.m_fDistance > (aLightPosition - aOrigin).GetMagnitude())
	    {
			isdIntersectionData.m_bIntersectionDetected = false;
			return rShadowRay;
	    }
    }
	
    return rShadowRay;
}

vec3 renderer::CalculateShadows(intersectionData& isdData) const
{
	vec3 v3Origin = isdData.m_v3IntersectionPoint;
	
	// Cast shadow rays
	ray rShadowRay = CastShadowRay(v3Origin, m_p_scActiveScene->m_lsLight.m_v3Position, isdData.m_v3IntersectionNormal, isdData);


	bool bOccluded = isdData.m_bIntersectionDetected;
	return CalculateDiffuseColor(bOccluded, rShadowRay, m_p_scActiveScene->m_lsLight, isdData.m_v3IntersectionNormal, isdData.m_maTransportedMaterial);
}

vec3 renderer::CalculateDiffuseColor(bool bShadowRayOccluded, ray& rAShadowRay, lightSource& lsALightSource, vec3& v3AIntersectionNormal, material& maObjectMaterial) const
{	
    if (bShadowRayOccluded) return {};

    //TODO rename LightStrength to distanceAttenuation && add diffuse shading
    // Calculate distance attenuation by dividing 1 by the length of the vector
    // between the light source and the shadow rays origin squared
    const vec3 v3VecToLight = lsALightSource.m_v3Position - rAShadowRay.m_v3Origin;
    const float fTempDistanceToLight = v3VecToLight.GetSquaredMagnitude();
    const float fLightStrength = 1.0f / (fTempDistanceToLight);
   
    const float fDiffusion = maObjectMaterial.fDiffusion;

	// Lambertian shading
    // Calculating illumination by factoring in the angle of the light hitting the
    // object & multiplying that with the diffusion of the material
    float fIllumination = rAShadowRay.m_v3Direction.DotWith(v3AIntersectionNormal);
    mathm::Clamp(fIllumination, 1.0f, 0.0f);
    //fIllumination *= fDiffusion;

    vec3 v3LightAccumulation{};
    v3LightAccumulation += lsALightSource.m_v3LightColor * fLightStrength * fIllumination;
	
    return maObjectMaterial.m_v3Color * v3LightAccumulation;
}

float renderer::Fresnel(vec3& v3IncomingDirection, vec3& v3OutgoingDirection, vec3& v3IntersectionNormal, float fRefractIndex1, float fRefractIndex2)
{
	// calculating cos(θi) && cos(θt) 
	const float fIncomingCosine = v3IncomingDirection.DotWith(v3IntersectionNormal);
	const float fTransmittedCosine = v3OutgoingDirection.DotWith(v3IntersectionNormal);

	// calculating reflectance for s-polarized light
	float fTempA = fRefractIndex2 * fIncomingCosine;
	float fTempB = fRefractIndex1 * fTransmittedCosine;
	float fReflectanceS = (fTempA - fTempB) / (fTempA + fTempB);
	fReflectanceS *= fReflectanceS;

	// calculating reflectance for p-polarized light
	fTempA = fRefractIndex2 * fTransmittedCosine;
	fTempB = fRefractIndex1 * fIncomingCosine;
	float fReflectanceP = (fTempA - fTempB) / (fTempA + fTempB);
	fReflectanceP *= fReflectanceP;

	// returning reflectance for unpolarized light
	return (fReflectanceS + fReflectanceP) * 0.5f;
}

vec3 renderer::Trace(ray& rPrimaryRay, int iRecursionCap)
{
	
#ifdef _DEBUG

	//test for refraction function, result should be (2.6674, -2.76958, 0)
	vec3 normal{ 0,1,0 };
	vec3 direction{ 4,-4,0 };

	vec3 test = RefractRay(direction, normal, 1.5f, 1.000293f);
#endif

	
	if (iRecursionCap > 0)
	{
		iRecursionCap--;

		// intersecting primary ray with primitives in a scene
		intersectionData isdTempData;

		if (!rPrimaryRay.m_bIsPrimaryRay)
		{
			isdTempData = m_p_scActiveScene->GetIntersection(rPrimaryRay);
		}
		else CheckForPrimaryIntersections(rPrimaryRay, isdTempData);


		// assigning frequently used 
		float fReflectionIndex = isdTempData.m_maTransportedMaterial.m_fReflectionIndex;
		float fRefractionIndex = isdTempData.m_maTransportedMaterial.m_fRefractionIndex;

		vec3 v3IntersectionNormal = isdTempData.m_v3IntersectionNormal;
		vec3 v3Origin = isdTempData.m_v3IntersectionPoint + isdTempData.m_v3IntersectionNormal * 0.0001f;

		vec3 v3Direction = rPrimaryRay.m_v3Direction;

		//TODO abstract if statements, put things inside in its own function
		if (fReflectionIndex == 0)
		{
			// calculating shadows
			return CalculateShadows(isdTempData);
			
		}
		else if (fReflectionIndex == 1)
		{
			// calculating total reflection
			ray rSecondaryRay = { v3Origin, ReflectRay(v3Direction, v3IntersectionNormal) };
			return Trace(rSecondaryRay, iRecursionCap);

		}
		else if (fReflectionIndex > 0 && fReflectionIndex < 1)
		{

			// constructing secondary ray && Tracing Reflected ray
			ray rSecondaryRay = { v3Origin, ReflectRay(v3Direction, v3IntersectionNormal) };
			vec3 v3TempColor = isdTempData.m_maTransportedMaterial.m_v3Color;

			vec3 v3ReflectedColor = Trace(rSecondaryRay, iRecursionCap) * fReflectionIndex;

			// calculating shadows & shadow color
			vec3 v3ShadowRayColor = CalculateShadows(isdTempData);
			v3ShadowRayColor *= v3TempColor * (1 - fReflectionIndex);

			return (v3ReflectedColor + v3ShadowRayColor);
			
		} else if (fRefractionIndex >= 1)
		{
			
			// this approach to refraction was taken from Jacco Bikkers 5th Lecture
			float fN1 = rPrimaryRay.m_fCurrentRefractionIndex;
			float fN2 = isdTempData.m_maTransportedMaterial.m_fRefractionIndex;


			// TODO resolve
			// this will cause an issue if there is a glass sphere in another glass sphere

			// if the current medium the ray is in is not air, make the medium he is leaving
			// into to air
			if (fN1 > 1.10f && rPrimaryRay.m_bInPrimitive)
			{
				fN2 = 1.10f;
			}

			// calculating refracted ray origin offset
			vec3 v3RefractedOrigin = isdTempData.m_v3IntersectionPoint - isdTempData.m_v3IntersectionNormal * 0.0001f;

			// refract primary ray & constructing refracted ray
			vec3 v3RefractedDirection = RefractRay(v3Direction, v3IntersectionNormal, fN2, fN1);
			ray rRefractedRay{v3RefractedOrigin, v3RefractedDirection};

			// calculating reflectance of unpolarized light
			float fReflectance = Fresnel(v3Direction, v3RefractedDirection, 
											v3IntersectionNormal ,fN1, fN2);

			// calculating color of refracted ray
			vec3& v3MaterialColor = isdTempData.m_maTransportedMaterial.m_v3Color;
			vec3 v3RefractionColor = Trace(rRefractedRay, iRecursionCap) * (1 - fReflectance) * v3MaterialColor;

			
			// reflecting primary ray & constructing secondary ray
			vec3 v3SecondaryDirection = ReflectRay(v3Direction, v3IntersectionNormal);
			ray rSecondaryRay{v3Origin, v3SecondaryDirection};

			// calculating reflection color
			vec3 v3ReflectionColor = Trace(rSecondaryRay, iRecursionCap) * fReflectance;

			// returning color
			return v3ReflectionColor + v3RefractionColor;
			
		}
	}
	
	return {};
}

vec3 renderer::ReflectRay(vec3& v3Direction, vec3& v3IntersectionNormal)
{
	//the idea for this function was taken from Jacco Bikker's 4th lecture
	// Returns the new direction of a reflected vector
	return { v3Direction - v3IntersectionNormal * (v3Direction.DotWith(v3IntersectionNormal)) * 2 };
}

vec3 renderer::RefractRay(vec3& v3Direction, vec3& v3IntersectionNormal, float fInsideRefraction, float fOutsideRefraction)
{
	// n1 == refractionIndex of the outside medium
	// n2 == refractionIndex of the inside medium
	const float n1 = fOutsideRefraction;
	const float n2 = fInsideRefraction;

	const float j = (n1 / n2);

	// calculating the cosine of the angle between the rays direction & the normal
	// of the intersection point by dotting the Normal with -(Direction)
	vec3 v3InverseDirection = v3Direction * (-1.0);
	const float cosTheta = v3IntersectionNormal.DotWith(v3InverseDirection);

	const float k = 1.0f - ((j * j) * (1 - cosTheta * cosTheta));
	
	// through Snell's Law, we know that if k < 0, there is no refraction occurring, only 
	// reflection (because of total internal reflection) return reflectRay()
	if (k < 0.f)
	{
		return ReflectRay(v3Direction, v3IntersectionNormal);
	}
	
	// if k >= 0, refraction occurs -> calculate T (transmitted vector)
	// and return it
	return {v3Direction * j + v3IntersectionNormal * (j * cosTheta - sqrt(k)) };
}

void renderer::ConstructGrid() const
{

	// loop over all of the primitives in the active scene
	const int iCounter = m_p_scActiveScene->m_iPrimitiveCounter;
	for (int i = 0; i < iCounter; i++)
	{
		
		primitive* p_prCurrentPrimitive = m_p_scActiveScene->m_prPrimitives[i];
		
		// if the primitive is a sphere
		if (p_prCurrentPrimitive->m_iPrimitiveType == 2)
		{

			// get the spheres on-screen center point and radius
			float fRadiusInPixels = 0;
			float ViewPortWidth = (m_p_cMainCamera->m_v3ViewportP1 - m_p_cMainCamera->m_v3ViewportP0).GetMagnitude();
			vec3 v3OnScreenPos = m_p_scActiveScene->m_prPrimitives[i]->GetScreenspaceCoordinate(
				m_p_cMainCamera->m_v3Position, m_p_cMainCamera->m_v3ViewportP0, m_p_cMainCamera->m_v3ViewportP1,
				m_p_cMainCamera->m_v3ViewportP0, m_p_cMainCamera->m_v3ViewportP2, SCRWIDTH, SCRHEIGHT, ViewPortWidth, fRadiusInPixels);


			// calculating the minimum and maximum x/y
			float fMinX = v3OnScreenPos.x - fRadiusInPixels;
			float fMaxX = v3OnScreenPos.x + fRadiusInPixels;

			float fMinY = v3OnScreenPos.y - fRadiusInPixels;
			float fMaxY = v3OnScreenPos.y + fRadiusInPixels;

			// calculating the minimum and maximum x/y in the Grid
			// to find the upper leftmost cell and the
			// lower rightmost cell
			int iMaxCellX = static_cast<int>(fMaxX / CELLWIDTH);
			int iMaxCellY = static_cast<int>(fMaxY / CELLHEIGHT);

			int iMinCellX = static_cast<int>(fMinX / CELLWIDTH);
			int iMinCellY = static_cast<int>(fMinY / CELLHEIGHT);

			mathm::Clamp(iMinCellX, CELLAMOUNT_WIDTH, 0);
			mathm::Clamp(iMinCellY, CELLAMOUNT_HEIGHT, 0);
			mathm::Clamp(iMaxCellX, CELLAMOUNT_WIDTH, 0);
			mathm::Clamp(iMaxCellY, CELLAMOUNT_HEIGHT, 0);

			// looping over the square of cells a given sphere overlaps
			for (int j = iMinCellX; j <= iMaxCellX; j++)
			{
				for (int k = iMinCellY; k <= iMaxCellY; k++)
				{
					// adding the sphere to the cell && increasing its primitive counter
					int& iIndex = m_p_cScreenCells[j + k * CELLAMOUNT_WIDTH].m_iPrimCounter;
					m_p_cScreenCells[j + k * CELLAMOUNT_WIDTH].m_iPrimitiveIndices[iIndex] = i;
					iIndex++;
				}
			}
		}	
		else if (p_prCurrentPrimitive->m_iPrimitiveType == 1)
		{
			// if primitive is a plane, add it to all cells
			for (int j = 0; j < CELLAMOUNT_WIDTH * CELLAMOUNT_HEIGHT; j++)
			{
				m_p_cScreenCells[j].m_iPrimCounter++;
				m_p_cScreenCells[j].m_iPrimitiveIndices[m_p_cScreenCells[j].m_iPrimCounter] = i;
			}
		}
	}
#ifdef _DEBUG
	//use this to turn of rendering in a given cell

	//m_p_cScreenCells[5 + 7 * CELLAMOUNT_WIDTH].m_iPrimCounter = 0;
	
#endif
}
