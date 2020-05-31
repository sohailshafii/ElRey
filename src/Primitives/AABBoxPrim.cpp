#include "AABBoxPrim.h"
#include "Point3.h"
#include "Point4.h"
#include "Ray.h"
#include "CommonMath.h"

AABBoxPrim::AABBoxPrim(std::shared_ptr<Material> const & iMaterial,
					   const std::string& iName) : Primitive(iMaterial, iName),
						x0(-1.0f), y0(-1.0f), z0(-1.0f),
						x1(1.0f), y1(1.0f), z1(1.0f) {
}

AABBoxPrim::AABBoxPrim(const float x0, const float y0, const float z0,
	   const float x1, const float y1, const float z1,
	   std::shared_ptr<Material> const & iMaterial,
	   const std::string& iName) : Primitive(iMaterial, iName), x0(x0),
	y0(y0), z0(z0), x1(x1), y1(y1), z1(z1) {
}

AABBoxPrim::AABBoxPrim(Point3 const & min, Point3 const & max,
					   std::shared_ptr<Material> const & iMaterial,
					   const std::string& iName) :
						Primitive(iMaterial, iName),
						x0(min[0]), y0(min[1]), z0(min[2]),
						x1(max[0]), y1(max[1]), z1(max[2]) {
}

AABBoxPrim::AABBoxPrim(Point4 const & min, Point4 const & max,
					   std::shared_ptr<Material> const & iMaterial,
					   const std::string& iName) :
					   Primitive(iMaterial, iName),
					   x0(min[0]), y0(min[1]), z0(min[2]),
					   x1(max[0]), y1(max[1]), z1(max[2]) {
}

bool AABBoxPrim::Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) {
	auto const & rayOrigin = ray.GetOrigin();
	auto const & rayDirection = ray.GetDirection();
	
	float rayOriginX = rayOrigin[0];
	float rayOriginY = rayOrigin[1];
	float rayOriginZ = rayOrigin[2];
	float rayDirX = rayDirection[0];
	float rayDirY = rayDirection[1];
	float rayDirZ = rayDirection[2];
	
	float tMinX, tMinY, tMinZ;
	float tMaxX, tMaxY, tMaxZ;
	
	float invDirX = 1.0f/rayDirX;
	if (invDirX >= 0) {
		tMinX = (x0 - rayOriginX) * invDirX;
		tMaxX = (x1 - rayOriginX) * invDirX;
	}
	else {
		tMinX = (x1 - rayOriginX) * invDirX;
		tMaxX = (x0 - rayOriginX) * invDirX;
	}
	
	float invDirY = 1.0f/rayDirY;
	if (invDirY >= 0) {
		tMinY = (y0 - rayOriginY) * invDirY;
		tMaxY = (y1 - rayOriginY) * invDirY;
	}
	else {
		tMinY = (y1 - rayOriginY) * invDirY;
		tMaxY = (y0 - rayOriginY) * invDirY;
	}
	
	float invDirZ = 1.0f/rayDirZ;
	if (invDirZ >= 0) {
		tMinZ = (z0 - rayOriginZ) * invDirZ;
		tMaxZ = (z1 - rayOriginZ) * invDirZ;
	}
	else {
		tMinZ = (z1 - rayOriginZ) * invDirZ;
		tMaxZ = (z0 - rayOriginZ) * invDirZ;
	}
	
	float t0, t1;
	AABoxFace EnteringFace, ExitingFace;
	// find entering t (biggest)
	if (tMinX > tMinY) {
		t0 = tMinX;
		EnteringFace = invDirX >= 0.0f ? MinX : MaxX;
	}
	else {
		t0 = tMinY;
		EnteringFace = invDirY >= 0.0f ? MinY : MaxY;
	}
	if (tMinZ > t0) {
		t0 = tMinZ;
		EnteringFace = invDirZ >= 0.0f ? MinZ : MaxZ;
	}
	
	// exit t (smallest)
	if (tMaxX < tMaxY) {
		t1 = tMaxX;
		ExitingFace = invDirX >= 0.0f ? MaxX : MinX;
	}
	else {
		t1 = tMaxY;
		ExitingFace = invDirY >= 0.0f ? MaxY : MinY;
	}
	if (tMaxZ < t1) {
		t1 = tMaxZ;
		ExitingFace = invDirZ >= 0.0f ? MaxZ : MinZ;
	}
	
	// passes if (biggest) entering t is smaller than (smallest) exit t
	// and that exit point is greater than ray's start
	if (t0 < t1 && t1 > tMin) {
		if (t0 > tMin) {
			tMax = t0;
		}
		else {
			tMax = t1;
		}
		intersectionResult.SetIntersectionT(tMax);
		return true;
	}
	else {
		return false;
	}
}

bool AABBoxPrim::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	auto const & rayOrigin = ray.GetOrigin();
	auto const & rayDirection = ray.GetDirection();
	
	float rayOriginX = rayOrigin[0];
	float rayOriginY = rayOrigin[1];
	float rayOriginZ = rayOrigin[2];
	float rayDirX = rayDirection[0];
	float rayDirY = rayDirection[1];
	float rayDirZ = rayDirection[2];
	
	float tMinX, tMinY, tMinZ;
	float tMaxX, tMaxY, tMaxZ;
	
	float invDirX = 1.0f/rayDirX;
	if (invDirX >= 0) {
		tMinX = (x0 - rayOriginX) * invDirX;
		tMaxX = (x1 - rayOriginX) * invDirX;
	}
	else {
		tMinX = (x1 - rayOriginX) * invDirX;
		tMaxX = (x0 - rayOriginX) * invDirX;
	}
	
	float invDirY = 1.0f/rayDirY;
	if (invDirY >= 0) {
		tMinY = (y0 - rayOriginY) * invDirY;
		tMaxY = (y1 - rayOriginY) * invDirY;
	}
	else {
		tMinY = (y1 - rayOriginY) * invDirY;
		tMaxY = (y0 - rayOriginY) * invDirY;
	}
	
	float invDirZ = 1.0f/rayDirZ;
	if (invDirZ >= 0) {
		tMinZ = (z0 - rayOriginZ) * invDirZ;
		tMaxZ = (z1 - rayOriginZ) * invDirZ;
	}
	else {
		tMinZ = (z1 - rayOriginZ) * invDirZ;
		tMaxZ = (z0 - rayOriginZ) * invDirZ;
	}
	
	float t0, t1;
	// find entering t (biggest)
	if (tMinX > tMinY) {
		t0 = tMinX;
	}
	else {
		t0 = tMinY;
	}
	if (tMinZ > t0) {
		t0 = tMinZ;
	}
	
	// exit t (smallest)
	if (tMaxX < tMaxY) {
		t1 = tMaxX;
	}
	else {
		t1 = tMaxY;
	}
	if (tMaxZ < t1) {
		t1 = tMaxZ;
	}
	
	// passes if (biggest) entering t is smaller than (smallest) exit t
	// and that exit point is greater than ray's start
	return (t0 < t1 && t1 > tMin);
}

bool AABBoxPrim::PointInside(Point3 const& point) const {
	return ((point[0] > x0 && point[0] < x1) &&
			(point[1] > y0 && point[1] < y1) &&
			(point[2] > z0 && point[2] < z1));
}

bool AABBoxPrim::PointInside(Point4 const& point) const {
	return ((point[0] > x0 && point[0] < x1) &&
			(point[1] > y0 && point[1] < y1) &&
			(point[2] > z0 && point[2] < z1));
}

Vector3 AABBoxPrim::GetNormalAtPosition(const Point3& position) const {
	return Vector3();
}

void AABBoxPrim::SamplePrimitive(Point3& resultingSample) {
	// TODO if needed for area lights
}

float AABBoxPrim::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f; // TODO if needed for area lights
}
