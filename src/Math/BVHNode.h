#pragma once

#include <cstdlib>
#include <iostream>
#include "Hittable.h"
#include "Ray.h"
#include "AABB.h"

class BVHNode : public Hittable {
public:
	BVHNode() {
		cleanUpChildren = false;
		left = nullptr;
		right = nullptr;
	}
	BVHNode(Hittable **l, int n, float time0, float time1,
		bool cleanUpLeaves);

	~BVHNode() {
		if (cleanUpChildren) {
			delete left;
		}
		if (cleanUpChildren) {
			delete right;
		}
	}

	virtual bool Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Hittable *left;
	Hittable *right;
	AABB box;
	bool cleanUpChildren;

private:
	static int boxXCompare(const void *a, const void *b);
	static int boxYCompare(const void *a, const void *b);
	static int boxZCompare(const void *a, const void *b);
};

BVHNode::BVHNode(Hittable **l, int n, float time0, float time1,
	bool cleanUpLeaves) {
	int axis = int(3* getRand());
	if (axis == 0) {
		qsort(l, n, sizeof(Hittable *), boxXCompare);
	}
	else if (axis == 1) {
		qsort(l, n, sizeof(Hittable *), boxYCompare);
	}
	else {
		qsort(l, n, sizeof(Hittable *), boxZCompare);
	}

	// don't clean up children -- not owned
	this->cleanUpChildren = cleanUpLeaves;
	if (n == 1) {
		left = right = l[0];
	}
	else if (n == 2) {
		left = l[0];
		right = l[1];
	}
	else {
		left = new BVHNode(l, n/2, time0, time1, cleanUpLeaves);
		right = new BVHNode(l + n/2, n - n/2, time0, time1, cleanUpLeaves);
		this->cleanUpChildren = true; // we don't clean up children but always
		// clean up allocated nodes
	}
	AABB boxLeft, boxRight;
	if (!left->BoundingBox(time0, time1, boxLeft) ||
		!right->BoundingBox(time0, time1, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor!\n";
	}
	box = SurroundingBox(boxLeft, boxRight);
}

int BVHNode::boxXCompare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hittable *ah = *(Hittable**)a;
	Hittable *bh = *(Hittable**)b;

	if (!ah->BoundingBox(0, 0, boxLeft) ||
		!bh->BoundingBox(0, 0, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor compare!\n";
	}

	if (boxLeft.Min().x() - boxRight.Min().x() < 0.0) {
		return -1;
	}
	else {
		return 1;
	}
}

int BVHNode::boxYCompare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hittable *ah = *(Hittable**)a;
	Hittable *bh = *(Hittable**)b;

	if (!ah->BoundingBox(0, 0, boxLeft) ||
		!bh->BoundingBox(0, 0, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor compare!\n";
	}

	if (boxLeft.Min().y() - boxRight.Min().y() < 0.0) {
		return -1;
	}
	else {
		return 1;
	}	
}

int BVHNode::boxZCompare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hittable *ah = *(Hittable**)a;
	Hittable *bh = *(Hittable**)b;

	if (!ah->BoundingBox(0, 0, boxLeft) ||
		!bh->BoundingBox(0, 0, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor compare!\n";
	}

	if (boxLeft.Min().z() - boxRight.Min().z() < 0.0) {
		return -1;
	}
	else {
		return 1;
	}
}

bool BVHNode::BoundingBox(float t0, float t1, AABB& b) const {
	b = box;
	return true;
}

bool BVHNode::Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const {
	if (box.Hit2(r, tmin, tmax)) {
		HitRecord leftRec, rightRec;
		bool hitLeft = left->Hit(r, tmin, tmax, leftRec);
		bool hitRight = right->Hit(r, tmin, tmax, rightRec);
		if (hitLeft && hitRight) {
			if (leftRec.t < rightRec.t) {
				rec = leftRec;
			}
			else {
				rec = rightRec;
			}
			return true;
		}
		else if (hitLeft) {
			rec = leftRec;
			return true;
		}
		else if (hitRight) {
			rec = rightRec;
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

