#include "AABBoxPrim.h"
#include "Point3.h"
#include "Point4.h"
#include "Ray.h"
#include "CommonMath.h"

AABBoxPrim::AABBoxPrim(std::shared_ptr<Material> const & iMaterial,
					   std::shared_ptr<GenericSampler> const & iSampler,
					   const std::string& iName) :
						Primitive(iMaterial, iSampler, iName),
						x0(-1.0f), y0(-1.0f), z0(-1.0f),
						x1(1.0f), y1(1.0f), z1(1.0f) {
	CalculateInvVolume();
	CalculateCenter();
}

AABBoxPrim::AABBoxPrim(const float x0, const float y0, const float z0,
					   const float x1, const float y1, const float z1,
					   std::shared_ptr<Material> const & iMaterial,
					   std::shared_ptr<GenericSampler> const & iSampler,
					   const std::string& iName) :
						Primitive(iMaterial, iSampler, iName), x0(x0),
	y0(y0), z0(z0), x1(x1), y1(y1), z1(z1) {
	CalculateInvVolume();
	CalculateCenter();
}

AABBoxPrim::AABBoxPrim(Point3 const & min, Point3 const & max,
					   std::shared_ptr<Material> const & iMaterial,
					   std::shared_ptr<GenericSampler> const & iSampler,
					   const std::string& iName) :
						Primitive(iMaterial, iSampler, iName),
						x0(min[0]), y0(min[1]), z0(min[2]),
						x1(max[0]), y1(max[1]), z1(max[2]) {
	CalculateInvVolume();
	CalculateCenter();
}

AABBoxPrim::AABBoxPrim(Point4 const & min, Point4 const & max,
					   std::shared_ptr<Material> const & iMaterial,
					   std::shared_ptr<GenericSampler> const & iSampler,
					   const std::string& iName) :
					   Primitive(iMaterial, iSampler, iName),
					   x0(min[0]), y0(min[1]), z0(min[2]),
					   x1(max[0]), y1(max[1]), z1(max[2]) {
	CalculateInvVolume();
	CalculateCenter();
}

bool AABBoxPrim::IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
								IntersectionResult &intersectionResult) {
	auto const & rayOrigin = rayLocal.GetOrigin();
	auto const & rayDirection = rayLocal.GetDirection();
	
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
	FaceHit faceIn, faceOut;
	// find entering t (biggest)
	if (tMinX > tMinY) {
		t0 = tMinX;
		faceIn = invDirX >= 0.0f ? NegativeX : PositiveX;
	}
	else {
		t0 = tMinY;
		faceIn = invDirY >= 0.0f ? NegativeY : PositiveY;
	}
	if (tMinZ > t0) {
		t0 = tMinZ;
		faceIn = invDirZ >= 0.0f ? NegativeZ : PositiveZ;
	}
	
	// exit t (smallest)
	if (tMaxX < tMaxY) {
		t1 = tMaxX;
		faceOut = invDirX >= 0.0f ? PositiveX : NegativeX;
	}
	else {
		t1 = tMaxY;
		faceOut = invDirY >= 0.0f ? PositiveY : NegativeY;
	}
	if (tMaxZ < t1) {
		t1 = tMaxZ;
		faceOut = invDirZ >= 0.0f ? PositiveZ : NegativeZ;
	}
	
	Vector3 tempNorm;
	// passes if (biggest) entering t is smaller than (smallest) exit t
	// and that exit point is greater than ray's start
	if (t0 < t1 && t1 > tMin) {
		if (t0 > tMin) {
			if (t0 > tMax) {
				return false;
			}
			tMax = t0;
			switch (faceIn) {
				case NegativeX:
					tempNorm = Vector3(-1.0f, 0.0f, 0.0f);
					break;
				case NegativeY:
					tempNorm = Vector3(0.0f,-1.0f, 0.0f);
					break;
				case NegativeZ:
					tempNorm = Vector3(0.0f, 0.0f,-1.0f);
					break;
				case PositiveX:
					tempNorm = Vector3(1.0f, 0.0f, 0.0f);
					break;
				case PositiveY:
					tempNorm = Vector3(0.0f, 1.0f, 0.0f);
					break;
				default:
					tempNorm = Vector3(0.0f, 0.0f, 1.0f);
			}
		}
		else {
			if (t1 > tMax) {
				return false;
			}
			tMax = t1;
			switch (faceOut) {
				case NegativeX:
					tempNorm = Vector3(-1.0f, 0.0f, 0.0f);
					break;
				case NegativeY:
					tempNorm = Vector3(0.0f,-1.0f, 0.0f);
					break;
				case NegativeZ:
					tempNorm = Vector3(0.0f, 0.0f,-1.0f);
					break;
				case PositiveX:
					tempNorm = Vector3(1.0f, 0.0f, 0.0f);
					break;
				case PositiveY:
					tempNorm = Vector3(0.0f, 1.0f, 0.0f);
					break;
				default:
					tempNorm = Vector3(0.0f, 0.0f, 1.0f);
			}
			tempNorm = isTransformed ?
				GetWorldToLocalTransposeDir(tempNorm).Normalized() : tempNorm;
		}
		intersectionResult.SetIntersectionT(tMax);
		intersectionResult.SetGenericMetadata(tempNorm[0], tempNorm[1],
											  tempNorm[2]);
		return true;
	}
	else {
		return false;
	}
}

bool AABBoxPrim::IntersectShadowLocal(const Ray &rayLocal, float tMin,
									  float tMax) {
	auto const & rayOrigin = rayLocal.GetOrigin();
	auto const & rayDirection = rayLocal.GetDirection();
	
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
	
	if (t0 < t1 && t1 > tMin) {
		bool faceIn = t0 > tMin;
		if (faceIn) {
			return t0 < tMax;
		}
		else {
			return t1 < tMax;
		}
	}
	
	return false;
}

bool AABBoxPrim::PointInsideLocal(Point3 const& point) const {
	return ((point[0] > x0 && point[0] < x1) &&
			(point[1] > y0 && point[1] < y1) &&
			(point[2] > z0 && point[2] < z1));
}

bool AABBoxPrim::PointInsideLocal(Point4 const& point) const {
	return ((point[0] > x0 && point[0] < x1) &&
			(point[1] > y0 && point[1] < y1) &&
			(point[2] > z0 && point[2] < z1));
}

Vector3 AABBoxPrim::GetNormalWorld(IntersectionResult const &intersectionResult) const {
	float meta1 = intersectionResult.GetGenericMetadata1();
	float meta2 = intersectionResult.GetGenericMetadata2();
	float meta3 = intersectionResult.GetGenericMetadata3();
	return Vector3(meta1, meta2, meta3);
}

void AABBoxPrim::SamplePrimitiveLocal(Point3& resultingSample) {
	// we have to sample to unit cube; leave out unless we need it
}

void AABBoxPrim::SamplePrimitiveWorld(Point3& resultingSample) {
	// we have to sample to unit cube; leave out unless we need it
}

float AABBoxPrim::PDF(const IntersectionResult& intersectionResult) const {
	return invVolume;
}


AABBox AABBoxPrim::GetBoundingBoxLocal() const {
	return AABBox(x0, y0, z0, x1, y1, z1);
}

AABBox AABBoxPrim::GetBoundingBoxWorld() const {
	Point3 worldSpaceMin(x0, y0, z0);
	Point3 worldSpaceMax(x1, y1, z1);
	GetLocalToWorldPos(worldSpaceMin);
	GetLocalToWorldPos(worldSpaceMax);
	return AABBox(worldSpaceMin[0], worldSpaceMin[1],
				  worldSpaceMin[2], worldSpaceMax[0],
				  worldSpaceMax[1], worldSpaceMax[2]);
}
