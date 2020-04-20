#include "Rectangle.h"
#include "CommonMath.h"

bool Rectangle::Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	float t = (origin - rayOrigin) * normal / (rayDirection * normal);

	if (t < EPSILON)
	{
		return false;
	}

	Point3 intersectionPoint = ray.GetPositionAtParam(t);
	Vector3 vectorAlongPlane = intersectionPoint - rayOrigin;

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

	if (t < tMin || t > tMax) {
		return false;
	}

	tMax = t;
	intersectionResult.SetIntersectionT(tMax);

	return true;
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

	Point3 intersectionPoint = ray.GetPositionAtParam(t);
	Vector3 vectorAlongPlane = intersectionPoint - rayOrigin;

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

	if (t < tMin || t > tMax) {
		return false;
	}

	return true;
}

void Rectangle::SamplePrimitive(Point3& resultingSample) {
	Point2 sampleOnSquare = sampler->GetSampleOnUnitSquare();
	resultingSample = origin + side1Vec * sampleOnSquare[0] +
		side2Vec * sampleOnSquare[1];
}

// each sample's probability is 1.0/inverseArea
float Rectangle::PDF(const IntersectionResult& intersectionResult) const {
	return inverseArea;
}

