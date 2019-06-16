#pragma once

#include <cstdint>

class FPSCounter {
public:
	FPSCounter();
	void PreFrame();
	void PostFrame();

	float GetFPS() const;

private:
#define NUM_FRAMES 10
	uint32_t frameCount;
	uint32_t frameTimes[NUM_FRAMES];
	uint32_t frameTimePreFrame;
	uint32_t fps;
};
