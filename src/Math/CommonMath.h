
#pragma once

#include <random>
#include <cstdlib>
#include <cmath>

#define EPSILON 0.0000001f

#define SHADOW_FEELER_EPSILON 0.01f

#ifndef M_PI
# define M_PI 3.14159265358979323846f
#endif

#define TWO_PI 2.0f*M_PI

#define INV_PI 1.0f/M_PI

#define INV_TWO_PI 1.0f/(2.0f*M_PI)

#define DEG_2_RAD M_PI/180.0f
#define RAD_2_DEG 180.0f/M_PI

class Vector3;
class Point3;
class Ray;

class CommonMath {
public:
	// returns random value between 0 and 1
	static float RandomUnitValue() {
		// I have NO IDEA why, but using std::random_device
		// causes strange memory errors using Dr. Memory like
		// "UNADDRESSABLE ACCESS" beyond top of stack. Sticking with C
		return (float)rand() * randMaxInverse;
		//std::random_device rd;
		// Standard mersenne_twister_engine seeded with rd()
		//std::mt19937 gen(rd());
		//std::uniform_real_distribution<>dis(0.0f, 1.0f);
		//return (float)dis(gen);
	}
	
	static void Clamp(float &value, float min, float max) {
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}
	}
	
	static float RandomRangeValue(float low, float high) {
		return RandomUnitValue() * (high - low) + low;
	}

	static int RandInt() {
		return rand();
	}

	static int RandInt(int low, int high) {
		// add one to be inclusive of end
		float randomFloat = RandomRangeValue(0.0f,
			high - low + 1.0f);
		return (int)randomFloat + low;
	}

	static void SetRandSeed(unsigned int seed) {
		srand(seed);
	}
	
	static float Clamp(float x, float min, float max) {
		return (x < min ? min : (x > max ? max : x));
	}
	
	static void ComputeUVWFromUpandForward(Vector3 &right, Vector3 &up, Vector3 &forward);
	
	static void ComputeFresnelCoefficients(Vector3 const & wo,
										   float etaIn, float etaOut,
										   Vector3 & normal,
										   float &kr, float &kt,
										   float &relativeEta,
										   float &cosThetaI,
										   float &cosThetaT);
	
	static float ComputeFresnelDescriminant(float cosTheta,
											float invRelativeEta) {
		return 1.0f - invRelativeEta*invRelativeEta*(1.0f - cosTheta*cosTheta);
	}
	
	static int SolveQuadric(double c[3], double s[2]);
	static int SolveCubic(double c[4], double s[3]);
	static int SolveQuartic(double c[5], double s[4]);
	
	static bool TestTriangle(Point3 const & p0, Point3 const & p1,
							 Point3 const & p2, Ray const & ray,
							 float tMin, float tMax, float& t,
							 float &beta, float &gamma);

	static const float randMaxInverse;

private:
	//static std::random_device randDevice;
	//static std::mt19937 gen;
	//static std::uniform_real_distribution<> dis;
};

