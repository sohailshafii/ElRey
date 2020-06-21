
#pragma once

#include <random>
#include <cstdlib>
#include <cmath>

#define EPSILON 0.0000001f

#define SHADOW_FEELER_EPSILON 0.01f

#ifndef M_PI
# define M_PI 3.14159265358979323846f
#endif

#define INV_PI 1.0/M_PI

#define DEG_2_RAD M_PI/180.0f
#define RAD_2_DEG 180.0f/M_PI

class Vector3;

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
	
	static void ComputeUVWFromWandU(Vector3 &right, Vector3 &up, Vector3 &forward);
	
	static int SolveQuadric(float c[3], float s[2]);
	static int SolveCubic(float c[4], float s[3]);
	static int SolveQuartic(float c[5], float s[4]);

	static const float randMaxInverse;

private:
	//static std::random_device randDevice;
	//static std::mt19937 gen;
	//static std::uniform_real_distribution<> dis;
};

