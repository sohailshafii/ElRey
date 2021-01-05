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
		if (currentPrimitive->UsedForInstancing()) {
			continue;
		}
		
		auto hitPrimitive = currentPrimitive->Intersect(ray, tMin, tMax,
														intersectionResult);
		if (hitPrimitive) {
			closestPrimitive = currentPrimitive;
		}
	}

	return closestPrimitive;
}

bool SimpleWorld::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
												 float tMax,
												 const Primitive* primitiveToExclude) {
	Ray rayToCast = ray;
	Vector3 originalDir = ray.GetDirection();
	Point3 originalOrigin = ray.GetOrigin();
	for (auto currentPrimitive : primitives) {
		if (currentPrimitive == primitiveToExclude ||
			currentPrimitive->UsedForInstancing()) {
			continue;
		}
		
		if (currentPrimitive->IntersectShadow(ray, tMin, tMax))
		{
			return true;
		}
	}
	return false;
}
