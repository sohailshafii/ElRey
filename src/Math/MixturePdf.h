#pragma once

#include "Pdf.h"
#include "Common.h"

class MixturePdf : public Pdf {
public:
	MixturePdf(std::shared_ptr<Pdf> p0,
		std::shared_ptr<Pdf> p1) {
		p[0] = p0;
		p[1] = p1;
	}

	virtual float value(const Vec3& direction) const {
		return 0.5f * p[0]->value(direction) +
			0.5f * p[1]->value(direction);
	}

	virtual Vec3 generate() const {
		if (getRand() < 0.5) {
			return p[0]->generate();
		}
		else {
			return p[1]->generate();
		}
	}

	std::shared_ptr<Pdf> p[2]; 
};
