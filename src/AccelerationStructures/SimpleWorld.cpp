#include "SimpleWorld.h"
#include "IntersectionResult.h"
#include "Primitive.h"

SimpleWorld::SimpleWorld() : BaseAccelerator() {
}

SimpleWorld::SimpleWorld(Primitive **primitives,
						 unsigned int numPrimitives)
	: BaseAccelerator(primitives, numPrimitives) {	
}

void SimpleWorld::SetUpAccelerator(nlohmann::json const & jsonObj) {
	// nothing to see here for simple world
}

Primitive* SimpleWorld::Intersect(const Ray &ray, float tMin, float& tMax,
							IntersectionResult &intersectionResult) {
	Primitive* closestPrimitive = nullptr;
	for (auto currentPrimitive : primitives) {
		auto hitPrimitive = currentPrimitive->Intersect(ray, tMin, tMax,
														intersectionResult);
		if (hitPrimitive != nullptr) {
			closestPrimitive = hitPrimitive;
		}
	}

	return closestPrimitive;
}

Primitive* SimpleWorld::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
													   float tMax) {
	Ray rayToCast = ray;
	Vector3 originalDir = ray.GetDirection();
	Point3 originalOrigin = ray.GetOrigin();
	for (auto currentPrimitive : primitives) {
		auto hitPrimitive = currentPrimitive->IntersectShadow(ray, tMin, tMax);
		if (hitPrimitive != nullptr)
		{
			return hitPrimitive;
		}
	}
	return nullptr;
}
