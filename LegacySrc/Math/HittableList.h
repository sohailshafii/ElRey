#pragma once

#include "Hittable.h"
#include "Common.h"

class HittableList : public Hittable {
public:
	HittableList() { }
	HittableList(Hittable **l, int n) { list = l; listSize = n; }
	virtual bool Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const;

	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const;

	virtual float pdfValue(const Vec3& o, const Vec3& V) const;
	virtual Vec3 random(const Vec3& o) const;

	virtual ~HittableList() { 
		if (list != nullptr) {
			for (int i = 0; i < listSize; i++) {
				delete list[i];
			}
			delete [] list;
		}
	}

	Hittable **list;
	int listSize;
};

bool HittableList::Hit(const Ray &r, float tMin, float tMax,
	HitRecord &rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	float closestSoFar = tMax;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->Hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}

bool HittableList::BoundingBox(float t0, float t1, AABB &box) const {
	if (listSize < 1) {
		return false;
	}
	AABB tempBox;
	bool firstTrue = list[0]->BoundingBox(t0, t1, tempBox);
	if (!firstTrue) {
		return false;
	}
	else {
		box = tempBox;
	}

	for (int i = 1; i < listSize; i++) {
		if (list[i]->BoundingBox(t0, t1, tempBox)) {
			box = SurroundingBox(box, tempBox);
		}
		else {
			return false;
		}
	}
	return true;
}

float HittableList::pdfValue(const Vec3& o, const Vec3& V) const {
	float weight = 1.0f / listSize;
	float sum = 0.0f;
	for (int i = 0; i < listSize; i++) {
		sum += weight*list[i]->pdfValue(o, V);
	}
	return sum;
}

Vec3 HittableList::random(const Vec3& o) const {
	auto randVal = getRand();
	int index = int(randVal * (listSize-1));
	return list[index]->random(o);
}
