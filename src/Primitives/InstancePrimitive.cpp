#include "InstancePrimitive.h"

InstancePrimitive::InstancePrimitive(std::shared_ptr<Material> const& iMaterial,
									 const std::string& iName,
									 std::shared_ptr<Primitive> & primitive) :
	Primitive(iMaterial, iName), instancePrimitive(primitive) {
	
}

InstancePrimitive::InstancePrimitive(std::shared_ptr<Material> const&
									 iMaterial,
									 std::shared_ptr<GenericSampler> const& iSampler,
									 const std::string& iName,
									 std::shared_ptr<Primitive> & primitive) :
									Primitive(iMaterial, iSampler, iName), instancePrimitive(primitive) {
	
}

InstancePrimitive::InstancePrimitive(std::shared_ptr<Material> && material,
									 const std::string& iName,
									 std::shared_ptr<Primitive> & primitive) :
	Primitive(std::move(material), iName), instancePrimitive(primitive) {
					  
}

InstancePrimitive::InstancePrimitive(std::shared_ptr<Material> && material,
									 std::shared_ptr<GenericSampler> && sampler,
									 const std::string& iName,
									 std::shared_ptr<Primitive> & primitive)  :
	Primitive(std::move(material), std::move(sampler), iName), instancePrimitive(primitive) {
					  
}

void InstancePrimitive::SamplePrimitiveLocal(Point3& resultingSample) {
	instancePrimitive->SamplePrimitiveLocal(resultingSample);
}

void InstancePrimitive::SamplePrimitiveWorld(Point3& resultingSample) {
	instancePrimitive->SamplePrimitiveWorld(resultingSample);
}

bool InstancePrimitive::HasBoundingBox() const {
	return instancePrimitive->HasBoundingBox();
}

AABBox InstancePrimitive::GetBoundingBoxLocal() const {
	return instancePrimitive->GetBoundingBoxLocal();
}

AABBox InstancePrimitive::GetBoundingBoxWorld() const {
	return instancePrimitive->GetBoundingBoxWorld();
}

bool InstancePrimitive::Intersect(const Ray &rayWorld, float tMin,
								  float& tMax,
								  IntersectionResult &intersectionResult) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}
	return Intersect(rayToCast, tMin, tMax, intersectionResult);
}

bool InstancePrimitive::IntersectShadow(const Ray &rayWorld,
										float tMin, float tMax) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}
	return IntersectShadow(rayToCast, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormal(IntersectionResult const &intersectionResult)
	const {
	Vector3 normalLocal = GetNormal(intersectionResult);
	return isTransformed ? GetWorldToLocalTransposeDir(normalLocal).Normalized() :
		normalLocal;
}

// TODO: re-write to use transforms in one place when ready
bool InstancePrimitive::IntersectLocal(const Ray &rayLocal,
									   float tMin, float& tMax,
									   IntersectionResult &intersectionResult) {
	return instancePrimitive->Intersect(rayLocal, tMin, tMax, intersectionResult);
}

bool InstancePrimitive::IntersectShadowLocal(const Ray &rayLocal,
											 float tMin, float tMax)  {
	return instancePrimitive->IntersectShadow(rayLocal, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormalLocal(IntersectionResult const
							   &intersectionResult) const {
	return instancePrimitive->GetNormal(intersectionResult);
}
