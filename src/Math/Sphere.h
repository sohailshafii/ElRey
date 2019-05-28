#pragma once

#include <cmath>
#include "Hittable.h"
#include "Materials/Material.h"
#include "Common.h"
#include "Onb.h"

class Sphere : public Hittable {
public:
	Sphere() {}
	Sphere(Vec3 cen, float r, std::shared_ptr<Material> mat) : 
		center(cen), radius(r) { 
		material = mat;
	}

	virtual ~Sphere() { }

	virtual bool BoundingBox(float t0, float t1, AABB &box) const;
	virtual bool Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const;

	virtual float pdfValue(const Vec3& o, const Vec3& V) const;
	virtual Vec3 random(const Vec3& o) const;

	Vec3 center;
	float radius;
};

bool Sphere::BoundingBox(float t0, float t1, AABB &box) const {
	box = AABB(center - Vec3(radius, radius, radius),
		center + Vec3(radius, radius, radius));
	return true;
}

bool Sphere::Hit(const Ray& r, float tMin, float tMax,
	HitRecord& rec) const {
	// note that we had 2.0 in front of b and 4.0 in
	// front of a*c originally -- cancelled that with 2.0 in
	// denominator
	Vec3 centerToOrigin = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(centerToOrigin, r.direction());
	float c = dot(centerToOrigin, centerToOrigin) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant))/a;
		rec.matPtr = material;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParam(rec.t);
			Vec3 vecFromCenter = (rec.p-center)/radius;
			GetSphereUV(vecFromCenter, rec.u, rec.v);
			rec.normal = vecFromCenter;
			return true;
		}
		temp = (-b + sqrt(b*b - a*c))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParam(rec.t);
			Vec3 vecFromCenter = (rec.p-center)/radius;
			GetSphereUV(vecFromCenter, rec.u, rec.v);
			rec.normal = vecFromCenter;
			return true;
		}
	}
	return false;
}

float Sphere::pdfValue(const Vec3& o, const Vec3& V) const {
	HitRecord rec;
	if (this->Hit(Ray(o, V), 0.001f, FLT_MAX, rec)) {
		float cosThetaMax = sqrt(1.0f - radius*radius /
			(center - o).squaredLength());
		float solidAngle = 2.0f*(float)M_PI*(1.0f - cosThetaMax);
		return 1.0f / solidAngle;
	}
	else {
		return 0.0f;
	}
}

Vec3 Sphere::random(const Vec3& o) const {
	Vec3 direction = center - o;
	float distanceSquared = direction.squaredLength();
	ONB uvw;
	uvw.buildFromW(direction);
	return uvw.local(RandomToSphere(radius, distanceSquared));
}
