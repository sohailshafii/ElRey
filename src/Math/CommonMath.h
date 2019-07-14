
#pragma once

#include <random>
#include <cstdlib>

class CommonMath {
public:
	// returns random value between 0 and 1
	static float randomUnitValue() {
		// I have NO IDEA why, but using std::random_device
		// causes strange memory errors using Dr. Memory like
		// "UNADDRESSABLE ACCESS" beyond top of stack. Sticking with C
		return (float)rand() / (float)RAND_MAX;
		//std::random_device rd;
		// Standard mersenne_twister_engine seeded with rd()
		//std::mt19937 gen(rd());
		//std::uniform_real_distribution<>dis(0.0f, 1.0f);
		//return (float)dis(gen);
	}

private:
	//static std::random_device randDevice;
	//static std::mt19937 gen;
	//static std::uniform_real_distribution<> dis;
};

