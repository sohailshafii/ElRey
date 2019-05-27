#pragma once

#include "Pdf.h"
#include "Vec3.h"
#include "Hittable.h"

class HittablePdf : public Pdf {
public:

	HittablePdf(std::shared_ptr<Hittable> p, const Vec3& origin)
		: ptr(p), o(origin) {
	}

	virtual float value(const Vec3& direction) const {
		return ptr->pdfValue(o, direction);
	}

	virtual Vec3 generate() const {
		return ptr->random(o);
	}

	Vec3 o;
	std::shared_ptr<Hittable> ptr;
};
