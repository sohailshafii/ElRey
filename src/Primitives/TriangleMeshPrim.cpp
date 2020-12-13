#include "TriangleMeshPrim.h"
#include "Primitives/TriangleMesh.h"

bool TriangleMeshPrimitive::Intersect(const Ray &ray, float tMin, float& tMax,
			   IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	Point3 const & p0 = triangleMesh->vertices[index0];
	Point3 const & p1 = triangleMesh->vertices[index1];
	Point3 const & p2 = triangleMesh->vertices[index2];
	
	float a = p0[0] - p1[0], b = p0[0] - p2[0],
		c = rayDirection[0], d = p0[0] - rayOrigin[0];
	float e = p0[1] - p1[1], f = p0[1] - p2[1],
		g = rayDirection[1], h = p0[1] - rayOrigin[1];
	float i = p0[2] - p1[2], j = p0[2] - p2[2],
		k = rayDirection[2], l = p0[2] - rayOrigin[2];
	
	float m = f * k - g * j, n = h * k - g * l,
		p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float invDenom = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * invDenom;
	
	if (beta < 0.0f) {
		return false;
	}
	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * invDenom;
	
	if (gamma < 0.0f) {
		return false;
	}
	
	if ((beta + gamma) > 1.0f) {
		return false;
	}
	
	float e3 = a * p - b * r + d * s;
	float t = e3 * invDenom;
	
	if (t < tMin || t > tMax) {
		return false;
	}
	
	tMax = t;
	intersectionResult.SetIntersectionT(tMax);
	
	return true;
}

bool TriangleMeshPrimitive::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	Point3 const & p0 = triangleMesh->vertices[index0];
	Point3 const & p1 = triangleMesh->vertices[index1];
	Point3 const & p2 = triangleMesh->vertices[index2];
	
	float a = p0[0] - p1[0], b = p0[0] - p2[0],
		c = rayDirection[0], d = p0[0] - rayOrigin[0];
	float e = p0[1] - p1[1], f = p0[1] - p2[1],
		g = rayDirection[1], h = p0[1] - rayOrigin[1];
	float i = p0[2] - p1[2], j = p0[2] - p2[2],
		k = rayDirection[2], l = p0[2] - rayOrigin[2];
	
	float m = f * k - g * j, n = h * k - g * l,
		p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float invDenom = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * invDenom;
	
	if (beta < 0.0f) {
		return false;
	}
	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * invDenom;
	
	if (gamma < 0.0f) {
		return false;
	}
	
	if ((beta + gamma) > 1.0f) {
		return false;
	}
	
	float e3 = a * p - b * r + d * s;
	float t = e3 * invDenom;
	
	if (t < tMin || t > tMax) {
		return false;
	}
	
	return true;
}

Vector3 TriangleMeshPrimitive::GetNormal(ParamsForNormal const &paramsForNormal) const {
	return normal;
}

void TriangleMeshPrimitive::SamplePrimitive(Point3& resultingSample,
											IntersectionResult const & intersectionResult) {
	// Not valid yet
}

float TriangleMeshPrimitive::PDF(ParamsForNormal const &paramsForNormal) const {
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
