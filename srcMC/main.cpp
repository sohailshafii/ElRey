#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Vec3.h"

float getRand() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline float pdf(const Vec3& p) {
	return 1.0/(4.0 * M_PI);
}

Vec3 RandomPointOnUnitSphere() {
	Vec3 p;
	do {
		p = 2.0*Vec3(getRand(), getRand(), getRand()) - Vec3(1,1,1);
	} while(p.squaredLength() >= 1.0);
	p.makeUnitVector();
	return p;
}

int main() {
	srand (time(NULL));

	int N = 1000000;
	float sum;
	for (int i = 0; i < N; i++) {
		Vec3 d = RandomPointOnUnitSphere();
		float cosineSquared = d.z()*d.z();
		sum += cosineSquared/pdf(d);
	}
	std::cout << "I = " << sum/N << ".\n";

	return 0;
}

