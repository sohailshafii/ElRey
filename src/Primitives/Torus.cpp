#include "Torus.h"
#include "CommonMath.h"
#include <algorithm>
#include <limits>

bool Torus::Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) {
	if (!boundingBox.RayHit(ray)) {
		return false;
	}
	
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float x1 = rayOrigin[0];
	float y1 = rayOrigin[1];
	float z1 = rayOrigin[2];
	float d1 = rayDirection[0];
	float d2 = rayDirection[1];
	float d3 = rayDirection[2];
	
	float coeffs[5];	// coefficient array for the quartic equation
	float roots[4];	// solution array for the quartic equation
	
	// define the coefficients of the quartic equation
	float sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	float e			= x1 * x1 + y1 * y1 +
		z1 * z1 - sweptRadiusSquared - tubeRadiusSquared;
	float f			= x1 * d1 + y1 * d2 + z1 * d3;
	float four_a_sqrd	= 4.0 * sweptRadiusSquared;
	
	coeffs[0] = e * e - four_a_sqrd * (tubeRadiusSquared - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4
	
	// find roots of the quartic equation
	int num_real_roots = CommonMath::SolveQuartic(coeffs, roots);
	bool	intersected = false;
	float 	t 		 	= std::numeric_limits<float>::max();
	
	if (num_real_roots == 0)  // ray misses the torus
		return(false);
	
	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted
	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > EPSILON) {
			intersected = true;
			if (roots[j] < tMax && roots[j] > tMin) {
				t = roots[j];
			}
		}
		
	if(!intersected) {
		return false;
	}
		
	tMax = t;
	intersectionResult.SetIntersectionT(tMax);
	
	return true;
}

bool Torus::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	if (!boundingBox.RayHit(ray)) {
		return false;
	}
	
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float x1 = rayOrigin[0];
	float y1 = rayOrigin[1];
	float z1 = rayOrigin[2];
	float d1 = rayDirection[0];
	float d2 = rayDirection[1];
	float d3 = rayDirection[2];
	
	float coeffs[5];	// coefficient array for the quartic equation
	float roots[4];	// solution array for the quartic equation
	
	// define the coefficients of the quartic equation
	float sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	float e			= x1 * x1 + y1 * y1 +
		z1 * z1 - sweptRadiusSquared - tubeRadiusSquared;
	float f			= x1 * d1 + y1 * d2 + z1 * d3;
	float four_a_sqrd	= 4.0 * sweptRadiusSquared;
	
	coeffs[0] = e * e - four_a_sqrd * (tubeRadiusSquared - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4
	
	// find roots of the quartic equation
	int num_real_roots = CommonMath::SolveQuartic(coeffs, roots);
	bool	intersected = false;
	float 	t 		 	= std::numeric_limits<float>::max();
	
	if (num_real_roots == 0)  // ray misses the torus
		return(false);
	
	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted
	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > EPSILON) {
			intersected = true;
			if (roots[j] < tMax && roots[j] > tMin) {
				t = roots[j];
			}
		}
		
	if (!intersected) {
		return false;
	}
	
	return true;
}

void Torus::SamplePrimitive(Point3& resultingSample) {
	// nothing to see here for now
}

// each sample's probability is 1.0/inverseArea
float Torus::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f;
}

AABBox Torus::GetBoundingBox() const {
	return boundingBox;
}

