#pragma once

#include "Pdf.h"
#include "Onb.h"
#include "Common.h"
#include <cmath>

class CosinePdf : public Pdf {
public:
	CosinePdf(const Vec3& w) {
		uvw.buildFromW(w);
	}

	virtual float value(const Vec3& direction) const {
		float cosineVal = dot(unitVector(direction),
			uvw.w());
		if (cosineVal > 0.0f) {
			return cosineVal/(float)M_PI;
		}
		else {
			return 0.0f;
		}
	}

	virtual Vec3 generate() const {
		return uvw.local(RandomCosineDirection());
	}

	ONB uvw;
};
