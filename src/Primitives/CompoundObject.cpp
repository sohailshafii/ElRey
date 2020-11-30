#include "CompoundObject.h"
#include "OpenCylinder.h"

bool CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
							   IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	Primitive* closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		if (currPrimitive->Intersect(ray, tMin, tMax, intersectionResult)) {
			closestPrimSoFar = currPrimitive;
		}
	}
	
	// since the name is set at the top level, only supports one level of compound objects
	// TODO: support recursion
	if (closestPrimSoFar != nullptr) {
		intersectionResult.SetPrimitiveName(closestPrimSoFar->GetName());
		return true;
	}
	
	return false;
}

bool CompoundObject::IntersectShadow(const Ray &ray, float tMin,
									 float tMax) {
	unsigned int numElements = primitives.size();
	bool hitSomething = false;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		hitSomething =
			currPrimitive->IntersectShadow(ray, tMin, tMax);
		if (hitSomething) {
			break;
		}
	}
	
	return hitSomething;
}

Vector3 CompoundObject::GetNormal(ParamsForNormal const &paramsForNormal) const {
	Primitive* foundPrim = GetPrimitiveByName(paramsForNormal.GetPrimitiveName());
	Vector3 normalVec = foundPrim != nullptr ? foundPrim->GetNormal(paramsForNormal)
		: Vector3();
	return normalVec;
}

/*Vector3 CompoundObject::GetNormalAtPosition(Point3 const &position) const {
	
	Primitive* foundPrim = GetPrimitiveByName(paramsForNormal.GetPrimitiveName());
	return closestPrimSoFar != nullptr ? closestPrimSoFar->GetNormalAtPosition(position)
		: Vector3(0.0f, 0.0f, 0.0f);
}*/

Primitive* CompoundObject::GetPrimitiveByName(std::string const & intersecPrimName) const {
	for (Primitive* currPrim : primitives) {
		if (currPrim->GetName() == intersecPrimName) {
			return currPrim;
		}
	}
	return nullptr;
}

void CompoundObject::SamplePrimitive(Point3& resultingSample,
									 IntersectionResult const & intersectionResult) {
	Primitive* foundPrim = GetPrimitiveByName(intersectionResult.GetPrimitiveName());
	if (foundPrim != nullptr) {
		foundPrim->SamplePrimitive(resultingSample, intersectionResult);
	}
}

float CompoundObject::PDF(ParamsForNormal const &paramsForNormal) const {
	Primitive* foundPrim = GetPrimitiveByName(paramsForNormal.GetPrimitiveName());
	return foundPrim != nullptr ? foundPrim->PDF(paramsForNormal) : 0.0f;
}

AABBox CompoundObject::GetBoundingBox() const {
	return boundingBox;
}

void CompoundObject::AddPrimitive(Primitive * primitive) {
	primitives.push_back(primitive);
	RecomputeBoundingBox();
}

void CompoundObject::RemovePrimitiveAtIndex(unsigned int index) {
	primitives.erase(primitives.begin() + index);
	RecomputeBoundingBox();
}

void CompoundObject::RemovePrimitiveWithName(std::string const & name) {
	unsigned int indexToRemove;
	bool foundObject;
	unsigned int numElements = primitives.size();
	for(unsigned int index = 0; index < numElements; index++) {
		if (primitives[index]->GetName() == name) {
			foundObject = true;
			indexToRemove = index;
		}
	}
	
	if (foundObject) {
		RemovePrimitiveAtIndex(indexToRemove);
		RecomputeBoundingBox();
	}
}

Material const * CompoundObject::GetMaterial(IntersectionResult const & intersectionResult) {
	Primitive* foundPrim = GetPrimitiveByName(intersectionResult.GetPrimitiveName());
	return foundPrim != nullptr ? foundPrim->GetMaterial(intersectionResult) : nullptr;
}

const GenericSampler* CompoundObject::GetSampler(IntersectionResult const & intersectionResult) {
	Primitive* foundPrim = GetPrimitiveByName(intersectionResult.GetPrimitiveName());
	return foundPrim != nullptr ?
	foundPrim->GetSampler(intersectionResult) : nullptr;
}

void CompoundObject::RecomputeBoundingBox() {
	boundingBox.Reset();
	unsigned int numElements = primitives.size();
	for(unsigned int index = 0; index < numElements; index++) {
		auto primitiveBounds = primitives[index]->GetBoundingBox();
		std::cout << "name: " << primitives[index]->GetName() << std::endl;
		primitiveBounds.PrintBounds();
		boundingBox.Superset(primitiveBounds);
		std::cout << "bounds is now\n";
		boundingBox.PrintBounds();
		std::cout << "---\n";
	}
}

void CompoundObject::PrintBounds() {
	boundingBox.PrintBounds();
}
