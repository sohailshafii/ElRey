#pragma once

#include <cmath>
#include <cfloat>
#include "Math/Hittable.h"

class RotateY : public Hittable {
public:
	RotateY(Hittable *p, float angle);
	~RotateY() {
		if (ptr != nullptr) {
			delete ptr;
		}
	}

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const;

	Hittable *ptr;
	float sinTheta, cosTheta;
	bool hasBox;
	AABB bbox;
};

RotateY::RotateY(Hittable *p, float angle) 
 : ptr(p) {
 	float radians = (M_PI/180.)*angle;
 	sinTheta = sin(radians);
 	cosTheta = cos(radians);

 	hasBox = ptr->BoundingBox(0, 1, bbox);

 	auto bboxMin = bbox.Min();
 	auto bboxMax = bbox.Max();

 	// find AABB of rotated box by finding
 	// extremities
 	Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
 	Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
 	for (int i = 0; i < 2; i++) {
 		for (int j = 0; j < 2; j++) {
 			for (int k = 0; k < 2; k++) {
 				float x = i*bboxMax.x() +
 					(1-i)*bboxMin.x();
 				float y = j*bboxMax.y() +
 					(1-j)*bboxMin.y();
 				float z = k*bboxMax.z() +
 					(1-k)*bboxMin.z();

 				float newX = cosTheta*x + sinTheta*z;
 				float newZ = -sinTheta*x + cosTheta*z;
 				Vec3 tester(newX, y, newZ);

 				for (int c = 0; c < 3; c++) {
 					if (tester[c] > max[c]) {
 						max[c] = tester[c];
 					}
 					if (tester[c] < min[c]) {
 						min[c] = tester[c];
 					}
 				}
 			}
 		}
 	}

 	bbox = AABB(min, max);
}

bool RotateY::Hit(const Ray &r, float tMin, float tMax,
	HitRecord &rec) const {
	Vec3 origin = r.origin();
	Vec3 direction = r.direction();

	// rotate vector in opposite dir
	origin[0] = cosTheta*r.origin()[0] - sinTheta*r.origin()[2];
	origin[2] = sinTheta*r.origin()[0] + cosTheta*r.origin()[2];
	direction[0] = cosTheta*r.direction()[0] - sinTheta*r.direction()[2];
	direction[2] = sinTheta*r.direction()[0] + cosTheta*r.direction()[2];

	Ray rotatedR(origin, direction, r.time());
	if (ptr->Hit(rotatedR, tMin, tMax, rec)) {
		Vec3 p = rec.p;
		Vec3 normal = rec.normal;
		p[0] = cosTheta*rec.p[0] + sinTheta*rec.p[2];
		p[2] = -sinTheta*rec.p[0] + cosTheta*rec.p[2];

		normal[0] = cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
		normal[2] = -sinTheta*rec.normal[0] + cosTheta*rec.normal[2];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else {
		return false;
	}
}

bool RotateY::BoundingBox(float t0, float t1,
	AABB &box) const {
	box = bbox;
	return hasBox;
}
