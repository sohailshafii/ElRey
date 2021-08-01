
#pragma once

#include "Primitive.h"

class TriangleMesh;

class TriangleMeshPrimitive : public Primitive {
public:
	TriangleMeshPrimitive(std::shared_ptr<Material> const& iMaterial,
			const std::string& iName) : Primitive(iMaterial, iName), isSmooth(false),
			reverseNormal(reverseNormal) {
	}
	
	TriangleMeshPrimitive(std::shared_ptr<Material> const& iMaterial,
			const std::string& iName,
			std::shared_ptr<TriangleMesh> const &meshPtr,
			size_t index0, size_t index1, size_t index2, bool isSmooth, bool reverseNormal) : Primitive(iMaterial, iName),
				triangleMesh(meshPtr), index0(index0), index1(index1), index2(index2), isSmooth(isSmooth), reverseNormal(reverseNormal) {
			ComputeNormal();
			ComputeBoundingBox();
	}
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	virtual Vector3 GetNormal(ShadingInfo const & shadingInfo) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo &shadingInfo) override;
	
	virtual float PDF(const ShadingInfo &shadingInfo) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
	std::shared_ptr<TriangleMesh> triangleMesh;
	
private:
	AABBox boundingBoxLocal;
	size_t index0, index1, index2;
	class Vector3 normal;
	bool isSmooth;
	bool reverseNormal;
	
	void ComputeBoundingBox();
	void ComputeNormal();
};
