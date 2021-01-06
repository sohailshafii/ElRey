
#include "GridPrimitive.h"

 bool GridPrimitive::Intersect(const Ray &ray, float tMin, float& tMax,
							   IntersectionResult &intersectionResult) {
	 // TODO
	 return false;
 }

 bool GridPrimitive::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	 // TODO
	 return false;
 }

 Vector3 GridPrimitive::GetNormal(ParamsForNormal const &paramsForNormal) const {
	 // TODO
	 return Vector3();
 }

 Vector3 GridPrimitive::ComputeHardNormal(Point3 const &position) const {
	 // TODO
	 return Vector3();
 }

 void GridPrimitive::SamplePrimitive(Point3& resultingSample,
									 IntersectionResult const & intersectionResult) {
	 // TODO sample last primitive found
 }

 float GridPrimitive::PDF(ParamsForNormal const &paramsForNormal) const {
	 // TODO sample last primitive found
	 return 1.0f;
 }

 AABBox GridPrimitive::GetBoundingBox() const {
	 return boundingBox;
 }

void GridPrimitive::ComputeBoundingBox() {
	// TODO
}
