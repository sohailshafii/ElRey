#pragma once

#include "Materials/Texture.h"

class CheckerTexture : public Texture {
public:
	CheckerTexture() { }
	CheckerTexture(Texture *t0, Texture *t1): Even(t0), Odd(t1) { }

	~CheckerTexture() {
		if (Even != nullptr) {
			delete Even;
		}
		if (Odd != nullptr) {
			delete Odd;
		}
	}

	virtual Vec3 Value(float u, float v, const Vec3& p) const {
		float sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
		if (sines < 0) {
			return Odd->Value(u, v, p);
		}
		else {
			return Even->Value(u, v, p);
		}
	}

	Texture *Odd, *Even;

};
