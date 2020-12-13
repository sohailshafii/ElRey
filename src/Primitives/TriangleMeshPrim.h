
#pragma once

#include "Primitive.h"

class TriangleMesh;

class TriangleMeshPrimitive : public Primitive {
public:
	TriangleMeshPrimitive(std::shared_ptr<Material> const& iMaterial,
			const std::string& iName) : Primitive(iMaterial, iName) {
	}
	
	TriangleMeshPrimitive(std::shared_ptr<Material> const& iMaterial,
			const std::string& iName,
			std::shared_ptr<TriangleMesh> const &meshPtr,
			size_t index0, size_t index1, size_t index2) : Primitive(iMaterial, iName), triangleMesh(meshPtr),
				index0(index0), index1(index1), index2(index2)
		{
			ComputeNormal();
			ComputeBoundingBox();
	}
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) override;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
private:
	AABBox boundingBoxLocal;
	std::shared_ptr<TriangleMesh> triangleMesh;
	size_t index0, index1, index2;
	class Vector3 normal;
	
	void ComputeNormal();
	void ComputeBoundingBox();
};
