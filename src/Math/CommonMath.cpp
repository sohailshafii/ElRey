#include "CommonMath.h"
#include "Vector3.h"

//std::random_device Common::randDevice;
// Standard mersenne_twister_engine seeded with randDevice
//std::mt19937 Common::gen(randDevice());
//std::uniform_real_distribution<> Common::dis(0.0f, 1.0f);

const float CommonMath::randMaxInverse = 1.0f/(float)RAND_MAX;

// graphics gems code
#define     EQN_EPS     1e-9
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

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

// originally from graphics gems, volume 1
// http://www.realtimerendering.com/resources/GraphicsGems/gems/Roots3And4.c
int CommonMath::SolveQuadric(float c[3], float s[2])
{
	float p, q, D;
		
	// normal form: x^2 + px + q = 0
	p = c[1] / (2.0f * c[2]);
	q = c[0] / c[2];

	D = p * p - q;

	if (IsZero(D)) {
		s[0] = - p;
		return 1;
	}
	else if (D > 0) {
		double sqrtD = sqrt(D);

		s[0] =   sqrtD - p;
		s[1] = - sqrtD - p;
		return 2;
	}
	else {
		return 0;
	}
}

// originally from graphics gems, volume 1
// http://www.realtimerendering.com/resources/GraphicsGems/gems/Roots3And4.c
int CommonMath::SolveCubic(float c[4], float s[3])
{
	int    i, num;
	float  sub;
	float  A, B, C;
	float  sqA, p, q;
	float  cbP, D;

	// normal form: x^3 + Ax^2 + Bx + C = 0
	A = c[2] / c[3];
	B = c[1] / c[3];
	C = c[0] / c[3];

	//  substitute x = y - A/3 to eliminate quadric term:
	// x^3 +px + q = 0
	sqA = A * A;
	p = 0.333f * (-0.333f * sqA + B);
	// second term is 2/27
	q = 0.5f * (0.07407 * A * sqA - 0.333f * A * B + C);

	// Cardano's formula
	cbP = p * p * p;
	D = q * q + cbP;

	if (IsZero(D)) {
		if (IsZero(q)) { // triple solution
			s[0] = 0;
			num = 1;
		}
		else { // one single and one double solution
			double u = cbrt(-q);
			s[0] = 2 * u;
			s[1] = - u;
			num = 2;
		}
	}
	else if (D < 0) { // Casus irreducibilis: three real solutions
		float phi = 0.333f * acos(-q / sqrt(-cbP));
		float t = 2.0f * sqrt(-p);

		s[0] =  t * cos(phi);
		s[1] = -t * cos(phi + M_PI * 0.333f);
		s[2] = -t * cos(phi - M_PI * 0.333f);
		num = 3;
	}
	else { // one real solution
		float sqrtD = sqrt(D);
		float u = cbrt(sqrtD - q);
		float v = - cbrt(sqrtD + q);

		s[0] = u + v;
		num = 1;
	}

	// resubstitute
	sub = 0.3333f * A;

	for (i = 0; i < num; ++i) {
		s[i] -= sub;
	}

	return num;
}

// originally from graphics gems, volume 1
// http://www.realtimerendering.com/resources/GraphicsGems/gems/Roots3And4.c
int CommonMath::SolveQuartic(float c[5], float s[4]) {
	float  coeffs[4];
	float  z, u, v, sub;
	float  A, B, C, D;
	float  sqA, p, q, r;
	int    i, num;

	// normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0
	A = c[3]/c[4];
	B = c[2]/c[4];
	C = c[1]/c[4];
	D = c[0]/c[4];

	//  substitute x = y - A/4 to eliminate cubic term:
	// x^4 + px^2 + qx + r = 0
	sqA = A * A;
	p = -0.375f * sqA + B;
	q = 0.125f * sqA * A - 0.5f * A * B + C;
	// first coeff is 3/256
	r = -0.01171875f*sqA*sqA + 0.0625f*sqA*B - 0.25f*A*C + D;

	if (IsZero(r)) {
		// no absolute term: y(y^3 + py + q) = 0
		coeffs[0] = q;
		coeffs[1] = p;
		coeffs[2] = 0;
		coeffs[3] = 1;

		num = SolveCubic(coeffs, s);
		s[num++] = 0;
	}
	else {
		// solve the resolvent cubic
		coeffs[0] = 0.5f * r * p - 0.125f * q * q;
		coeffs[1] = -r;
		coeffs[2] = -0.5f * p;
		coeffs[3] = 1;
		
		SolveCubic(coeffs, s);

		// and take the one real solution
		z = s[0];

		// to build two quadric equations
		u = z * z - r;
		v = 2.0f * z - p;

		if (IsZero(u)) {
			u = 0;
		}
		else if (u > 0) {
			u = sqrt(u);
		}
		else {
			return 0;
		}
			
		if (IsZero(v)) {
			v = 0;
		}
		else if (v > 0) {
			v = sqrt(v);
		}
		else {
			return 0;
		}
			
		coeffs[0] = z - u;
		coeffs[1] = q < 0 ? -v : v;
		coeffs[2] = 1;

		num = SolveQuadric(coeffs, s);

		coeffs[0]= z + u;
		coeffs[1] = q < 0 ? v : -v;
		coeffs[2] = 1;

		num += SolveQuadric(coeffs, s + num);
	}

	// resubstitute
	sub = 0.25f * A;

	for (i = 0; i < num; ++i) {
		s[i] -= sub;
	}
		
	return num;
}
