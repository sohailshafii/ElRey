#pragma once

#include "Primitive.h"
#include <vector>
#include <string>

class CompoundObject : public Primitive {
public:
	CompoundObject(std::shared_ptr<Material> & iMaterial,
			const std::string& iName) :
		Primitive(iMaterial, iName) {
	}

	CompoundObject(std::shared_ptr<Material> && iMaterial,
		const std::string& iName) :
		Primitive(iMaterial, iName) {
	}

	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
					IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalWorld(IntersectionResult const &intersectionResult) const override;
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return false;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override {
		return AABBox();
	}
	
	virtual AABBox GetBoundingBoxWorld() const override {
		return AABBox();
	}
	
	virtual std::shared_ptr<Material> GetMaterial() override;
	virtual const GenericSampler* GetSampler() override;
	
	void AddPrimitive(Primitive * primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);
	
	void RecomputeTransformsForChildren();

	virtual Vector3 GetLocalToWorldDir(Vector3 const & inDir)
		const override;
	virtual Vector3 GetWorldToLocalDir(Vector3 const & inDir)
		const override;
	virtual Vector3 GetLocalToWorldTransposeDir(Vector3 const & inDir)
		const override;
	virtual Vector3 GetWorldToLocalTransposeDir(Vector3 const & inDir)
		const override;
	
	virtual Point3 GetLocalToWorldPos(Point3 const & inPos)
		const override;
	virtual Point3 GetWorldToLocalPos(Point3 const & inPos)
		const override;

private:
	std::vector<Primitive*> primitives;
	Primitive *closestPrimSoFar;
	
	Matrix4x4 worldToLocalChildren, localToWorldChildren;
	Matrix4x4 localToWorldTransposeChildren;
	Matrix4x4 worldToLocalTransposeChildren;
	
	Primitive* GetPrimitiveByIntersectionResult(IntersectionResult const &intersectionResult) const;
};

