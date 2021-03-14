#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

#include <vector>

class GridPrimitive : public Primitive {
public:
	GridPrimitive(const std::string& iName) :
		Primitive(iName) {
	}
	
	GridPrimitive(const std::string& iName,
				  const std::vector<std::shared_ptr<Primitive>> & primitives,
				  float multipier) :
		Primitive(iName) {
		SetUpAccelerator(multipier, primitives);
	}
	
	~GridPrimitive();
	
	void SetUpAccelerator(float multipier,
						  const std::vector<std::shared_ptr<Primitive>> & primitives);
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(const ShadingInfo& shadingInfo) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	virtual Material const * GetMaterial(const ShadingInfo& shadingInfo) override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo& shadingInfo) override;
	
	// a compound object might have a different sampler per-subbject
	virtual const GenericSampler* GetSampler(const ShadingInfo& shadingInfo) override;
	
	virtual float PDF(const ShadingInfo& shadingInfo) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
	virtual std::shared_ptr<Primitive> GetSubPrimitiveByName(std::string const & intersecPrimName) const override;
	
private:
	class PrimitiveCollection {
	public:
		void ComputeBoundingBox() {
			boundingBox.Reset();
			for(auto primitive : primitives) {
				boundingBox.Superset(primitive->GetBoundingBox());
			}
		}
		
		AABBox const & GetBoundingBox() {
			return boundingBox;
		}
		
		AABBox boundingBox;
		std::vector<std::shared_ptr<Primitive>> primitives;
	};
	
	class RayParameters {
	public:
		int ix, iy, iz;
		float dtx, dty, dtz;
		float txNext, tyNext, tzNext;
		int ixStep, iyStep, izStep;
		int ixStop, iyStop, izStop;
		bool txInvalid, tyInvalid, tzInvalid;
	};
	
	void SetupCells(float multipier,
					const std::vector<std::shared_ptr<Primitive>> & primitives);
	
	Point3 ComputeMinCoordinates(std::vector<std::shared_ptr<Primitive>> const & primitives);
	Point3 ComputeMaxCoordinates(std::vector<std::shared_ptr<Primitive>> const & primitives);
	
	bool CheckBoundsOfRay(Ray const& ray, float tMin, float tMax,
		RayParameters& rayParams);
	
	Primitive* EvaluatePrimitiveCollectionCell(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult, float tNext);
	
	Primitive* IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult);
	
	bool IntersectAgainstPrimitiveCollectionShadow(PrimitiveCollection & primitiveCollection,
												   const Ray &ray, float tMin, float tMax);
	
	Primitive* BruteForceIntersect(const Ray &ray, float tMin, float& tMax,
								   IntersectionResult &intersectionResult);
	bool BruteForceShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
												  float tMax);
	
	AABBox boundingBox;
	// primitives that are not in cells, because they don't have
	// bounding boxes, like planes
	std::vector<std::shared_ptr<Primitive>> primitivesNotInCells;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
