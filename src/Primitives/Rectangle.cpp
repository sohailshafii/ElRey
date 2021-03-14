#include "Rectangle.h"
#include "CommonMath.h"
#include <algorithm>

Primitive* Rectangle::Intersect(const Ray &ray, float tMin, float& tMax,
								IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	float t = (origin - rayOrigin) * normal / (rayDirection * normal);

	if (t < EPSILON)
	{
		return nullptr;
	}

	if (t < tMin || t > tMax) {
		return nullptr;
	}

	Point3 intersectionPoint = ray.GetPositionAtParam(t);
	Vector3 vectorAlongPlane = intersectionPoint - origin;

	float projectionSide1 = vectorAlongPlane * side1Vec;
	if (projectionSide1 < 0.0 || projectionSide1 > side1LengthSqr)
	{
		return nullptr;
	}

	float projectionSide2 = vectorAlongPlane * side2Vec;
	if (projectionSide2 < 0.0 || projectionSide2 > side2LengthSqr)
	{
		return nullptr;
	}

	tMax = t;
	intersectionResult.rayIntersectT = tMax;

	return this;
}

bool Rectangle::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	float t = (origin - rayOrigin) * normal / (rayDirection * normal);

	if (t < EPSILON)
	{
		return false;
	}

	if (t < tMin || t > tMax) {
		return false;
	}

	Point3 intersectionPoint = ray.GetPositionAtParam(t);
	Vector3 vectorAlongPlane = intersectionPoint - origin;

	float projectionSide1 = vectorAlongPlane * side1Vec;
	if (projectionSide1 < 0.0 || projectionSide1 > side1LengthSqr)
	{
		return false;
	}

	float projectionSide2 = vectorAlongPlane * side2Vec;
	if (projectionSide2 < 0.0 || projectionSide2 > side2LengthSqr)
	{
		return false;
	}

	return true;
}

void Rectangle::SamplePrimitive(Point3& resultingSample,
								const ShadingInfo &shadingInfo) {
	Point2 sampleOnSquare = sampler->GetSampleOnUnitSquare();
	resultingSample = origin + side1Vec * sampleOnSquare[0] +
		side2Vec * sampleOnSquare[1];
}

// each sample's probability is 1.0/inverseArea
float Rectangle::PDF(const ShadingInfo &shadingInfo) const {
	return inverseArea;
}

AABBox Rectangle::GetBoundingBox() const {
	return boundingBoxLocal;
}

void Rectangle::Initialize(const Vector3& iSide1Vec, const Vector3& iSide2Vec) {
	side1Vec = iSide1Vec;
	side2Vec = iSide2Vec;
	side1LengthSqr = side1Vec.NormSqr();
	side2LengthSqr = side2Vec.NormSqr();
	area = sqrt(side1LengthSqr) * sqrt(side2LengthSqr);
	inverseArea = 1.0f / area;
	// assume left-handed
	normal = side2Vec ^ side1Vec;
	normal.Normalize();
	
	boundingBoxLocal = ComputeBoundingBox();
}

AABBox Rectangle::ComputeBoundingBox() const {
	Point3 minPoint;
	Point3 maxPoint;
	
	Point3 side1Point = origin + side1Vec;
	Point3 side2Point = origin + side2Vec;
	
	minPoint[0] = std::min(side1Point[0], side2Point[0]);
	minPoint[1] = std::min(side1Point[1], side2Point[1]);
	minPoint[2] = std::min(side1Point[2], side2Point[2]);
	
	maxPoint[0] = std::max(side1Point[0], side2Point[0]);
	maxPoint[1] = std::max(side1Point[1], side2Point[1]);
	maxPoint[2] = std::max(side1Point[2], side2Point[2]);
	
	// in case any dimension is too thin, add some thickness
	if (fabs(maxPoint[2] - minPoint[2]) < EPSILON) {
		maxPoint[2] += 0.1f;
	}
	if (fabs(maxPoint[1] - minPoint[1]) < EPSILON) {
		maxPoint[1] += 0.1f;
	}
	if (fabs(maxPoint[0] - minPoint[0]) < EPSILON) {
		maxPoint[0] += 0.1f;
	}
	
	return AABBox(minPoint, maxPoint);
}

