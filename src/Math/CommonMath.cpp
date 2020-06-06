#include "CommonMath.h"
#include "Vector3.h"


//std::random_device Common::randDevice;
// Standard mersenne_twister_engine seeded with randDevice
//std::mt19937 Common::gen(randDevice());
//std::uniform_real_distribution<> Common::dis(0.0f, 1.0f);

const float CommonMath::randMaxInverse = 1.0f/(float)RAND_MAX;

void CommonMath::ComputeUVWFromWandU(Vector3 &right, Vector3 &up, Vector3 &forward) {
	// turn on sanity check in case we need it but leave it off to
	// avoid excess computational cost
#if 1
	// edge-case: what if up and forward to are parallel or
	// anti-parallel?
	auto dotProduct = forward * up;
	if (fabs(fabs(dotProduct) - 1.0f) < EPSILON) {
		// find unit vector in coordinate axis that is perpendicular
		// to forward
		Vector3 candidateUp = Vector3::Forward();
		Vector3 candidateUp2 = Vector3::Up();
		Vector3 candidateUp3 = Vector3::Right();
		float dot1 = fabs(candidateUp * forward);
		float dot2 = fabs(candidateUp2 * forward);
		float dot3 = fabs(candidateUp3 * forward);
		if (dot1 < dot2 && dot1 < dot3) {
			// subtract out the parallel part
			up = candidateUp - forward*(candidateUp * forward);
		}
		else if (dot2 < dot1 && dot2 < dot3) {
			// subtract out the parallel part
			up = candidateUp2 - forward * (candidateUp2 * forward);
		}
		else {
			// subtract out the parallel part
			up = candidateUp3 - forward * (candidateUp3 * forward);
		}
		up.Normalize();
	}
#endif

	// left-handed coordinate system
	right = up ^ forward;
	right.Normalize();
	up = forward ^ right;

	// (no need to normalize as two normal vectors
	// crossed results in a normal vector)
}
