// uses code from http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement
#include "FPSCounter.h"
#include <limits>
#include <SDL.h>

#define MAX_FPS 1000.0f

FPSCounter::FPSCounter() {
	frameCount = 0;
	frameTimePreFrame = SDL_GetTicks();
}

void FPSCounter::PreFrame() {
	frameTimePreFrame = SDL_GetTicks();
}

void FPSCounter::PostFrame() {
	auto currTicks = SDL_GetTicks();

	frameTimes[frameCount%NUM_FRAMES] = currTicks - frameTimePreFrame;

	frameCount++;
}

float FPSCounter::GetFPS() const {
	uint32_t framesToConsider = 0;
	if (frameCount < NUM_FRAMES) {
		framesToConsider = frameCount;
	}
	else {
		framesToConsider = NUM_FRAMES;
	}

	uint32_t totalFrameTimeMs = 0;
	for(int i = 0; i < framesToConsider; i++) {
		totalFrameTimeMs += frameTimes[i];
	}
	// in case frame time is too small
	if (totalFrameTimeMs < std::numeric_limits<float>::epsilon()) {
		return MAX_FPS;
	}
	float fpsMs = (float)framesToConsider/totalFrameTimeMs;
	return fpsMs*1000.0f;
}
