#include "TriangleMeshPrim.h"
#include "Primitives/TriangleMesh.h"

Primitive* TriangleMeshPrimitive::Intersect(const Ray &ray, float tMin, float& tMax,
											IntersectionResult &intersectionResult) {
	
	Point3 const & p0 = triangleMesh->vertices[index0];
	Point3 const & p1 = triangleMesh->vertices[index1];
	Point3 const & p2 = triangleMesh->vertices[index2];
	float t, beta, gamma;
	if (!CommonMath::TestTriangle(p0, p1, p2, ray, tMin, tMax, t,
								  beta, gamma)) {
		return nullptr;
	}
	
	tMax = t;
	intersectionResult.rayIntersectT = tMax;
	if (isSmooth) {
		intersectionResult.genericMetadata1 = beta;
		intersectionResult.genericMetadata2 = gamma;
	}
	
	auto const & textureCoords = triangleMesh->textureCoords;
	auto const & texCoord0 = textureCoords[index0];
	auto const & texCoord1 = textureCoords[index1];
	auto const & texCoord2 = textureCoords[index2];
	float firstMultiplier = 1.0f - beta - gamma;
	intersectionResult.u = firstMultiplier * texCoord0[0]
		+ beta * texCoord1[0] + gamma * texCoord2[0];
	intersectionResult.v = firstMultiplier * texCoord0[1]
		+ beta * texCoord1[1] + gamma * texCoord2[1];
	
	return this;
}

bool TriangleMeshPrimitive::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	if (ignoreShadowTest) {
		return false;
	}
	Point3 const & p0 = triangleMesh->vertices[index0];
	Point3 const & p1 = triangleMesh->vertices[index1];
	Point3 const & p2 = triangleMesh->vertices[index2];

	float t, beta, gamma;
	return CommonMath::TestTriangle(p0, p1, p2, ray, tMin, tMax, t,
									beta, gamma);
}

Vector3 TriangleMeshPrimitive::ComputeHardNormal(Point3 const &position) const {
	return normal;
}

Vector3 TriangleMeshPrimitive::GetNormal(ShadingInfo const & shadingInfo) const {
	if (!isSmooth) {
		return normal;
	}
		
	Vector3 normal(triangleMesh->normals[index0]*
				   (1 - shadingInfo.intGenericMetadata1 - shadingInfo.intGenericMetadata2)
				   + triangleMesh->normals[index1] * shadingInfo.intGenericMetadata1
				   + triangleMesh->normals[index2] * shadingInfo.intGenericMetadata2);
	normal.Normalize();
	return normal;
}

void TriangleMeshPrimitive::SamplePrimitive(Point3& resultingSample,
											const ShadingInfo &shadingInfo) {
	// Not valid yet
}

float TriangleMeshPrimitive::PDF(const ShadingInfo &shadingInfo) const {
	return 1.0f; // not used yet
}

AABBox TriangleMeshPrimitive::GetBoundingBox() const {
	return boundingBoxLocal;
}

void TriangleMeshPrimitive::ComputeNormal() {
	Point3 p0(triangleMesh->vertices[index0]);
	Point3 p1(triangleMesh->vertices[index1]);
	Point3 p2(triangleMesh->vertices[index2]);
	normal = (p1-p0)^(p2-p0);
	normal.Normalize();
	if (reverseNormal) {
		normal = -normal;
	}
}

void TriangleMeshPrimitive::ComputeBoundingBox() {
	double delta = 0.0001;  // to avoid degenerate bounding boxes
	
	Point3 const & v1 = triangleMesh->vertices[index0];
	Point3 const & v2 = triangleMesh->vertices[index1];
	Point3 const & v3 = triangleMesh->vertices[index2];
	
	Point3 minPoint, maxPoint;
	
	minPoint[0] = std::min(v1[0], v2[0]);
	minPoint[0] = std::min(minPoint[0], v3[0]);
	
	minPoint[1] = std::min(v1[1], v2[1]);
	minPoint[1] = std::min(minPoint[1], v3[1]);
	
	minPoint[2] = std::min(v1[2], v2[2]);
	minPoint[2] = std::min(minPoint[2], v3[2]);
	
	maxPoint[0] = std::max(v1[0], v2[0]);
	maxPoint[0] = std::max(maxPoint[0], v3[0]);
	
	maxPoint[1] = std::max(v1[1], v2[1]);
	maxPoint[1] = std::max(maxPoint[1], v3[1]);
	
	maxPoint[2] = std::max(v1[2], v2[2]);
	maxPoint[2] = std::max(maxPoint[2], v3[2]);
	
	// in case any dimension is too thin, add some thickness
	minPoint[0] -= 0.1f;
	minPoint[1] -= 0.1f;
	minPoint[2] -= 0.1f;
	maxPoint[0] += 0.1f;
	maxPoint[1] += 0.1f;
	maxPoint[2] += 0.1f;
	
	boundingBoxLocal = AABBox(minPoint, maxPoint);
}
