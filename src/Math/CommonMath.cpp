#include "CommonMath.h"
#include "Vector3.h"

//std::random_device Common::randDevice;
// Standard mersenne_twister_engine seeded with randDevice
//std::mt19937 Common::gen(randDevice());
//std::uniform_real_distribution<> Common::dis(0.0f, 1.0f);

const float CommonMath::randMaxInverse = 1.0f/(float)RAND_MAX;

#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif

// graphics gems code
#define     EQN_EPS     1e-13
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)
//#ifdef NOCBRT
#define     cbrt_custom(x)     ((x) > 0.0 ? pow((double)(x), 1.0/3.0) : \
                          ((x) < 0.0 ? -pow((double)-(x), 1.0/3.0) : 0.0))
//#endif

void CommonMath::ComputeUVWFromUpandForward(Vector3 &right, Vector3 &up, Vector3 &forward) {
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

void CommonMath::ComputeFresnelCoefficients(Vector3 const & wo,
											float etaIn, float etaOut,
											Vector3 & normal,
											float &kr, float &kt,
											float &relativeEta,
											float &cosThetaI,
											float &cosThetaT) {
	cosThetaI = normal * wo;
	Vector3 modifiedNormal = normal;
		
	if (cosThetaI < 0.0) {
		cosThetaI = -cosThetaI;
		normal = -normal;
		relativeEta = etaOut / etaIn;
	}
	else {
		relativeEta = etaIn / etaOut;
	}

	// compute transmitted coefficient before computing the related vector
	float descriminant = CommonMath::ComputeFresnelDescriminant(cosThetaI, 1.0f/relativeEta);
	cosThetaT = sqrt(descriminant);
	float rParallel = (relativeEta * cosThetaI - cosThetaT) / (relativeEta * cosThetaI + cosThetaT);
	float rPerpendicular = (cosThetaI - relativeEta * cosThetaT) / (cosThetaI + relativeEta * cosThetaT);
	kr = 0.5 * (rParallel * rParallel + rPerpendicular * rPerpendicular);
	kt = 1.0f - kr;
}

// originally from graphics gems, volume 1
// http://www.realtimerendering.com/resources/GraphicsGems/gems/Roots3And4.c
int CommonMath::SolveQuadric(double c[3], double s[2]) {
	double p, q, D;
		
	// normal form: x^2 + px + q = 0
	p = c[1] / (2 * c[2]);
	q = c[0] / c[2];

	D = p * p - q;

	if (IsZero(D)) {
		s[0] = -p;
		return 1;
	}
	else if (D < 0) {
		return 0;
	}
	else {
		double sqrtD = sqrt(D);

		s[0] =  sqrtD - p;
		s[1] = -sqrtD - p;
		return 2;
	}
}

// originally from graphics gems, volume 1
// http://www.realtimerendering.com/resources/GraphicsGems/gems/Roots3And4.c
int CommonMath::SolveCubic(double c[4], double s[3])
{
	int    i, num;
	double  sub;
	double  A, B, C;
	double  sqA, p, q;
	double  cbP, D;

	// normal form: x^3 + Ax^2 + Bx + C = 0
	A = c[2] / c[3];
	B = c[1] / c[3];
	C = c[0] / c[3];

	//  substitute x = y - A/3 to eliminate quadric term:
	// x^3 +px + q = 0
	sqA = A * A;
	p = 1.0/3 * (-1.0/3 * sqA + B);
	// second term is 2/27
	q = 1.0/2 * (2.0/27 * A * sqA - 1.0/3 * A * B + C);

	// Cardano's formula
	cbP = p * p * p;
	D = q * q + cbP;

	if (IsZero(D)) {
		if (IsZero(q)) { // triple solution
			s[0] = 0;
			num = 1;
		}
		else { // one single and one double solution
			double u = cbrt_custom(-q);
			s[0] = 2 * u;
			s[1] = - u;
			num = 2;
		}
	}
	else if (D < 0) { // Casus irreducibilis: three real solutions
		double phi = 1.0/3 * acos(-q / sqrt(-cbP));
		double t = 2 * sqrt(-p);

		s[0] =  t * cos(phi);
		s[1] = -t * cos(phi + M_PI / 3.0);
		s[2] = -t * cos(phi - M_PI / 3.0);
		num = 3;
	}
	else { // one real solution
		double sqrtD = sqrt(D);
		double u = cbrt_custom(sqrtD - q);
		double v = - cbrt_custom(sqrtD + q);

		s[0] = u + v;
		num = 1;
	}

	// resubstitute
	sub = 1.0/3 * A;

	for (i = 0; i < num; ++i) {
		s[i] -= sub;
	}

	return num;
}

// originally from graphics gems, volume 1
// http://www.realtimerendering.com/resources/GraphicsGems/gems/Roots3And4.c
int CommonMath::SolveQuartic(double c[5], double s[4]) {
	double  coeffs[4];
	double  z, u, v, sub;
	double  A, B, C, D;
	double  sqA, p, q, r;
	int    i, num;

	// normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0
	A = c[3]/c[4];
	B = c[2]/c[4];
	C = c[1]/c[4];
	D = c[0]/c[4];

	//  substitute x = y - A/4 to eliminate cubic term:
	// x^4 + px^2 + qx + r = 0
	sqA = A * A;
	p = -3.0/8 * sqA + B;
	q = 1.0/8 * sqA * A - 1.0/2 * A * B + C;
	// first coeff is 3/256
	r = -3.0/256*sqA*sqA + 1.0/16*sqA*B - 1.0/4*A*C + D;

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
		coeffs[0] = 1.0/2 * r * p - 1.0/8 * q * q;
		coeffs[1] = -r;
		coeffs[2] = -1.0/2 * p;
		coeffs[3] = 1;
		
		SolveCubic(coeffs, s);

		// and take the one real solution
		z = s[0];

		// to build two quadric equations
		u = z * z - r;
		v = 2 * z - p;

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
	sub = 1.0/4 * A;

	for (i = 0; i < num; ++i) {
		s[i] -= sub;
	}
		
	return num;
}
