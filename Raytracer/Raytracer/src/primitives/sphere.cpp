#include "precomp.h"

sphere::sphere(vec3 aPosition, float aRadius)
{
	m_v3Position = aPosition;
	m_fRadius = aRadius;;
	m_maObjectMaterial = { {1.0f, 1.0f, 1.0f}, 0.9f };

	m_iPrimitiveType = spherical;
}
bool sphere::RayIntersection(ray& rPrimaryRay, intersectionData& isdData)
{
	// this function was taken (and subsequently corrected) from Jacco Bikkers first
	// lecture on ray tracing because it was far more effective than the one I employed in earlier versions

	// calculating the vector from the rays origin to the spheres center point
	const vec3 c = m_v3Position - rPrimaryRay.m_v3Origin;

	// calculating the length of the projection of c (the vector from the origin of
	// the ray to the spheres center point) on the rays direction
	float t = c.DotWith(rPrimaryRay.m_v3Direction);

	// calculating point on the rays trajectory on the same height as the
	// spheres center
	auto q = c - (rPrimaryRay.m_v3Direction * t);

	// calculating (squared) distance between spheres center and point on
	// rays trajectory
	const float pSquared = q.DotWith(q);

	// if the distance between the ray and the spheres center is
	// bigger than the radius, no intersection happens
	if (pSquared > (m_fRadius * m_fRadius))
	{
		return false;
	}
	
	// if the distance to the rays origin (magnitude of c) is bigger than the radius,
	// the ray starts outside of the sphere, else inside. Set t
	// and other variables accordingly
	vec3 v3IntersectionPoint;
	vec3 v3IntersectNormal;
	if (c.GetSquaredMagnitude() > (m_fRadius * m_fRadius))
	{
		t -= sqrtf((m_fRadius * m_fRadius) - pSquared);

		v3IntersectionPoint = rPrimaryRay.m_v3Origin + rPrimaryRay.m_v3Direction * t;
		v3IntersectNormal = GetIntersectionNormal(v3IntersectionPoint);

		rPrimaryRay.m_bInPrimitive = false;
		rPrimaryRay.m_fCurrentRefractionIndex = 1.10f;
	}
	else
	{
		t += sqrtf((m_fRadius * m_fRadius) - pSquared);

		v3IntersectionPoint = rPrimaryRay.m_v3Origin + rPrimaryRay.m_v3Direction * t;
		v3IntersectNormal = GetIntersectionNormal(v3IntersectionPoint) * (-1.0f);

		rPrimaryRay.m_bInPrimitive = true;
		rPrimaryRay.m_fCurrentRefractionIndex = m_maObjectMaterial.m_fRefractionIndex;
	}

	// set the distance of the primary ray to t if the current intersection is closer
	// than the former and is not 0, set isdData
	if ((t < rPrimaryRay.m_fDistance) && (t > 0)) {

		rPrimaryRay.m_fDistance = t;

		isdData.m_bIntersectionDetected = true;
		isdData.m_v3IntersectionPoint = v3IntersectionPoint;
		isdData.m_v3IntersectionNormal = v3IntersectNormal;
		isdData.m_maTransportedMaterial = m_maObjectMaterial;
	}

	return true;
}

vec3 sphere::GetIntersectionNormal(vec3& intersectionPoint)
{
	return ((intersectionPoint - m_v3Position).Normalize());
}

vec3 sphere::GetScreenspaceCoordinate(vec3& v3Origin, vec3& v3Left, vec3& v3Right, vec3& v3Up, vec3& v3Down, int iScrwidth, int iScrheight, float fViewportWidth, float& RadiusInPixels)
{
	int iBuffer = 20;
	
	vec3 v3ScreenPos;
	v3ScreenPos.z = 0;
	
	// assigning temporary vectors to not change the values of the
	// vectors passed by reference
	vec3 v3LeftTemp = v3Left;
	vec3 v3RightTemp = v3Right;

	vec3 v3TempPosition = m_v3Position;

	// setting the y value of left and right to 0 to get the
	// correct length
	v3TempPosition.y = 0;
	v3LeftTemp.y = 0;
	v3RightTemp.y = 0;

	v3LeftTemp.Normalize();
	v3RightTemp.Normalize();
	
	// calculate Vector from sphere center to origin
	vec3 v3SphereToOrigin = v3TempPosition - v3Origin;

	// calculate the length of the projection of the vector
	// onto the left and right vector
	float fLeftLength = v3SphereToOrigin.DotWith(v3LeftTemp);
	float fRightLength = v3SphereToOrigin.DotWith(v3RightTemp);

	// calculating the point on left or right closest to
	// the sphere's origin
	vec3 v3PointLeft = v3Origin + v3LeftTemp * fLeftLength;
	vec3 v3PointRight = v3Origin + v3RightTemp * fRightLength;

	// calculating the distance between the sphere's center and
	// the left and right point
	float fLeftDist = (v3TempPosition - v3PointLeft).GetMagnitude();
	float fRightDist = (v3TempPosition - v3PointRight).GetMagnitude();

	// calculate screenpos x
	float fTotalDist = fLeftDist + fRightDist;
	float fScale = fLeftDist / fTotalDist;

	v3ScreenPos.x = fScale * iScrwidth;


	// doing the same for the y position
	vec3 v3UpTemp = v3Up;
	vec3 v3DownTemp = v3Down;

	v3TempPosition = m_v3Position;

	// setting the x value of left and right to 0 to get the
	// correct length
	v3TempPosition.x = 0;
	v3UpTemp.x = 0;
	v3DownTemp.x = 0;

	v3UpTemp.Normalize();
	v3DownTemp.Normalize();

	// calculate Vector from sphere center to origin
	v3SphereToOrigin = v3TempPosition - v3Origin;

	// calculate the length of the projection of the vector
	// onto the up and down vector
	float fUpLength = v3SphereToOrigin.DotWith(v3UpTemp);
	float fDownLength = v3SphereToOrigin.DotWith(v3DownTemp);

	// calculating the point on up or down closest to
	// the sphere's origin
	vec3 v3PointUp = v3Origin + v3UpTemp * fUpLength;
	vec3 v3PointDown = v3Origin + v3DownTemp * fDownLength;

	// calculating the distance between the sphere's center and
	// the upper and down point
	float fUpDist = (v3TempPosition - v3PointUp).GetMagnitude();
	float fDownDist = (v3TempPosition - v3PointDown).GetMagnitude();

	// calculate screenpos y
	fTotalDist = fUpDist + fDownDist;
	fScale = fUpDist / fTotalDist;

	v3ScreenPos.y = fScale * iScrheight;

	

	// calculate on screen radius

	// calculating the line segment a1 between the viewport and the camera on
	// the ray towards the sphere's center
	v3TempPosition = m_v3Position;

	v3SphereToOrigin = v3TempPosition - v3Origin;

	// creating the vector c1 that connects the origin to
	// the center of the viewport
	vec3 v3C1 = {0, 0, 2};

	// calculating the legnth of the projection of the vector
	// from the Origin to the sphere on c1
	float cMagnitude = v3SphereToOrigin.DotWith(v3C1);

	// calculating the vector to point T at the end of the projection
	vec3 v3OriginToT = v3Origin + v3C1 * cMagnitude;

	// calculating the distance between the sphere's center and T
	vec3 v3B2 = v3SphereToOrigin - v3OriginToT;

	// calculating a1 by calculating the segment on the viewport b1
	// between v3SphereToOrigin and c1 
	vec3 v3B1 = v3B2 * ((v3C1.GetMagnitude()) / cMagnitude);
	vec3 v3A1 = v3C1 + v3B1;

	// calculating the radius (adding buffer to account for
	// inaccuracies)
	float rPrime = m_fRadius * v3A1.GetMagnitude() / v3SphereToOrigin.GetMagnitude();
	float fProportionOnScreen = rPrime / fViewportWidth;
	
	RadiusInPixels = fProportionOnScreen * iScrwidth + iBuffer;
	
	return v3ScreenPos;
}
