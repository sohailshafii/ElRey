#pragma once

#include "Math/Hittable.h"
#include "Math/XyRect.h"
#include "Math/XzRect.h"
#include "Math/YzRect.h"

class Box : public Hittable {
public:
	Box() {

	}
	Box(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> mat);

	~Box() {
		if (hittableList != nullptr) {
			delete hittableList;
		}
	}

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const {
		box = AABB(pMin, pMax);
		return true;
	}

	Vec3 pMin, pMax;
	HittableList *hittableList;
};

Box::Box(const Vec3& p0, const Vec3& p1, std::shared_ptr<Material> mat) {
	pMin = p0;
	pMax = p1;
	material = mat;

	Hittable **listItems = new Hittable*[6];
	listItems[0] = new XyRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(),
		mat);
	listItems[1] = new FlippedNormalsHittable(
		std::make_shared<XyRect>(XyRect(p0.x(), p1.x(), p0.y(), p1.y(),
			p0.z(), mat))
		);

	listItems[2] = new XzRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(),
		mat);
	listItems[3] = new FlippedNormalsHittable(
		std::make_shared<XzRect>(XzRect(p0.x(), p1.x(), p0.z(), p1.z(),
			p0.y(), mat))
		);

	listItems[4] = new YzRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(),
		mat);
	listItems[5] = new FlippedNormalsHittable(
		std::make_shared<YzRect>(YzRect(p0.y(), p1.y(), p0.z(), p1.z(),
			p0.x(), mat))
		);
	hittableList = new HittableList(listItems, 6);
}

bool Box::Hit(const Ray &r, float tMin, float tMax,
	HitRecord &rec) const {
	return hittableList->Hit(r, tMin, tMax, rec);
}
