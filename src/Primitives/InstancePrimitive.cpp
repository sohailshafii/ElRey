#include "InstancePrimitive.h"
#include <iostream>

InstancePrimitive::InstancePrimitive(std::string const & iName,
									 std::shared_ptr<Primitive> primitive) :
	Primitive(iName), instancePrimitive(primitive) {
}

InstancePrimitive::~InstancePrimitive() {
	//delete instancePrimitive;
}

void InstancePrimitive::SamplePrimitive(Point3& resultingSample, const ShadingInfo& shadingInfo) {
	// if our child is an instance primitive, then that one will apply its
	// own transform too
	instancePrimitive->SamplePrimitive(resultingSample, shadingInfo);
	resultingSample = GetLocalToWorldPos(resultingSample);
}

bool InstancePrimitive::HasBoundingBox() const {
	return instancePrimitive->HasBoundingBox();
}

AABBox InstancePrimitive::GetBoundingBox() const {
	AABBox localBoundingBox = instancePrimitive->GetBoundingBox();
	// transform corners of bounding box, then compute bounding box
	// of those corners
	Point3 boxPoints[8];
	boxPoints[0][0] = localBoundingBox.x0; boxPoints[0][1] = localBoundingBox.y0; boxPoints[0][2] = localBoundingBox.z0;
	boxPoints[1][0] = localBoundingBox.x1; boxPoints[1][1] = localBoundingBox.y0; boxPoints[1][2] = localBoundingBox.z0;
	boxPoints[2][0] = localBoundingBox.x1; boxPoints[2][1] = localBoundingBox.y1; boxPoints[2][2] = localBoundingBox.z0;
	boxPoints[3][0] = localBoundingBox.x0; boxPoints[3][1] = localBoundingBox.y1; boxPoints[3][2] = localBoundingBox.z0;

	boxPoints[4][0] = localBoundingBox.x0; boxPoints[4][1] = localBoundingBox.y0; boxPoints[4][2] = localBoundingBox.z1;
	boxPoints[5][0] = localBoundingBox.x1; boxPoints[5][1] = localBoundingBox.y0; boxPoints[5][2] = localBoundingBox.z1;
	boxPoints[6][0] = localBoundingBox.x1; boxPoints[6][1] = localBoundingBox.y1; boxPoints[6][2] = localBoundingBox.z1;
	boxPoints[7][0] = localBoundingBox.x0; boxPoints[7][1] = localBoundingBox.y1; boxPoints[7][2] = localBoundingBox.z1;
	
	// transform using forward transform
	boxPoints[0] = GetLocalToWorldPos(boxPoints[0]);
	boxPoints[1] = GetLocalToWorldPos(boxPoints[1]);
	boxPoints[2] = GetLocalToWorldPos(boxPoints[2]);
	boxPoints[3] = GetLocalToWorldPos(boxPoints[3]);
	boxPoints[4] = GetLocalToWorldPos(boxPoints[4]);
	boxPoints[5] = GetLocalToWorldPos(boxPoints[5]);
	boxPoints[6] = GetLocalToWorldPos(boxPoints[6]);
	boxPoints[7] = GetLocalToWorldPos(boxPoints[7]);
	
	float x0, y0, z0;
	bool x0Set = false, y0Set = false, z0Set = false;
	float x1, y1, z1;
	bool x1Set = false, y1Set = false, z1Set = false;
	
	for (int i = 0; i < 8; i++) {
		if (!x0Set || boxPoints[i][0] < x0) {
			x0Set = true;
			x0 = boxPoints[i][0];
		}
		if (!y0Set || boxPoints[i][1] < y0) {
			y0Set = true;
			y0 = boxPoints[i][1];
		}
		if (!z0Set || boxPoints[i][2] < z0) {
			z0Set = true;
			z0 = boxPoints[i][2];
		}
		
		if (!x1Set || boxPoints[i][0] > x1) {
			x1Set = true;
			x1 = boxPoints[i][0];
		}
		if (!y1Set || boxPoints[i][1] > y1) {
			y1Set = true;
			y1 = boxPoints[i][1];
		}
		if (!z1Set || boxPoints[i][2] > z1) {
			z1Set = true;
			z1 = boxPoints[i][2];
		}
	}
	
	return AABBox(x0, y0, z0, x1, y1, z1);
}

Primitive* InstancePrimitive::Intersect(const Ray &rayWorld, float tMin,
										float& tMax,
										IntersectionResult &intersectionResult) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	// NOTE: do not normalize ray direction after transformation!
	// the local-to-world's t value equals the world-to-local's t
	// and that relationship is ruined if the vector is normalized
	// see https://cs.brown.edu/courses/cs123/lectures/CS123_18_Raytracing_10.24.19.pdf
	// https://graphicscompendium.com/raytracing/12-transformations
	// ray-object intersections don't assume normal vector anyway and should
	// not
	rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
	rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	auto hitPrim = instancePrimitive->Intersect(rayToCast, tMin, tMax, intersectionResult);
	// if we hit something, return US as we need to apply transformations for
	// further processing (like calculating normals, etc)
	return hitPrim != nullptr ? this : nullptr;
}

bool InstancePrimitive::IntersectShadow(const Ray &rayWorld,
										float tMin, float tMax) {
	if (ignoreShadowTest) {
		return false;
	}
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
	rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	// if our child is an instance primitive, then that one will apply its
	// own transform too
	return instancePrimitive->IntersectShadow(rayToCast, tMin, tMax) ?
		true : false;
}

Vector3 InstancePrimitive::GetNormal(const ShadingInfo& shadingInfo) const {
	// hack; modify intersection position so that primitive thinks it's in local space
	ShadingInfo resModified = shadingInfo;
	resModified.intersectionPosition = GetWorldToLocalPos(resModified.intersectionPosition);
	resModified.eyeDir = GetWorldToLocalDir(resModified.eyeDir);
	// if our child is an instance primitive, then that one will apply its
	// own transform too
	Vector3 normalLocal = instancePrimitive->GetNormal(resModified);
	
	return GetWorldToLocalTransposeDir(normalLocal).Normalized();
}

Vector3 InstancePrimitive::ComputeHardNormal(Point3 const &position) const {
	// if our child is an instance primitive, then that one will apply its
	// own transform too
	Vector3 normalLocal = instancePrimitive->ComputeHardNormal(GetWorldToLocalPos(position));
	return GetWorldToLocalTransposeDir(normalLocal).Normalized();
}

void InstancePrimitive::SetLocalToWorld(Matrix4x4 const & localToWorld) {
	this->localToWorld = localToWorld;
	this->localToWorldTranspose = localToWorld.Transpose();
}

void InstancePrimitive::SetWorldToLocal(Matrix4x4 const & worldToLocal) {
	this->worldToLocal = worldToLocal;
	this->worldToLocalTranspose = worldToLocal.Transpose();
}

void InstancePrimitive::SetTransformAndInverse(Matrix4x4 const & localToWorld,
									   Matrix4x4 const & worldToLocal) {
	this->localToWorld = localToWorld;
	this->localToWorldTranspose = localToWorld.Transpose();
	this->worldToLocal = worldToLocal;
	this->worldToLocalTranspose = worldToLocal.Transpose();
}

Vector3 InstancePrimitive::GetLocalToWorldDir(Vector3 const & inDir) const {
	return localToWorld*inDir;
}

Vector3 InstancePrimitive::GetWorldToLocalDir(Vector3 const & inDir) const {
	return worldToLocal*inDir;
}

Vector3 InstancePrimitive::GetLocalToWorldTransposeDir(Vector3 const & inDir) const {
	return localToWorldTranspose*inDir;
}

Vector3 InstancePrimitive::GetWorldToLocalTransposeDir(Vector3 const & inDir) const {
	return worldToLocalTranspose*inDir;
}

Point3 InstancePrimitive::GetLocalToWorldPos(Point3 const & inPos) const {
	return localToWorld*inPos;
}

Point3 InstancePrimitive::GetWorldToLocalPos(Point3 const & inPos) const {
	return worldToLocal*inPos;
}
