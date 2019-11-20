
#pragma once

#include <random>
#include <cstdlib>
#include <cmath>

#define EPSILON 0.0000001f

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

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

	static const float randMaxInverse;

private:
	//static std::random_device randDevice;
	//static std::mt19937 gen;
	//static std::uniform_real_distribution<> dis;
};

