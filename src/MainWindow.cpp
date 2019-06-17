#include <SDL.h>
#include <iostream>
#include "ElReyConfig.h"
#include "Performance/FPSCounter.h"
#include "Math/Plane.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight); 
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height);

Uint32 lastFPSTickTime = 0; 

int main(int argc, char* argv[]) {
	srand(static_cast <unsigned> (time(0)));
	std::cout << "ElRey version: " << ElRey_VERSION_MAJOR << "."
		<< ElRey_VERSION_MINOR << "\n";
	
	int width = 400, height = 300, numSamples = 10;
	bool offlineRender = false;
	//int width = 2, height = 2, numSamples = 1;

	if (argc > 1) {
		for (int argIndex = 1; argIndex < argc; argIndex++) {
			if (!strcmp(argv[argIndex], "-w") && argIndex+1 < argc) {
				width = atoi(argv[++argIndex]);
			}
			if (!strcmp(argv[argIndex], "-h") && argIndex+1 < argc) {
				height = atoi(argv[++argIndex]);
			}
			if (!strcmp(argv[argIndex], "-s") && argIndex+1 < argc) {
				numSamples = atoi(argv[++argIndex]);
			}
			if (!strcmp(argv[argIndex], "-offline")) {
				offlineRender = true;
			}
		}
	}

	std::cout << "Framebuffer dimensions: " <<  width << "x" << height
		<< ", num samples: " << numSamples << ".\n";

	float aspectRatio = (float)width/height;

	if (!initializeSDL()) {
		return 2;
	}

	SDL_Window *window = createWindow(width, height);
	if (window == nullptr) {
		return 3;
	}

	SDL_Renderer *sdlRenderer = SDL_CreateRenderer(window, -1, 0);
	if (window == nullptr) {
		return 3;
	}

	auto renderFormat = SDL_GetWindowPixelFormat(window);
	std::cout << "Render format: " <<
		SDL_GetPixelFormatName(renderFormat) << ", bits per pixel: "
		<< SDL_BITSPERPIXEL(renderFormat) << ".\n";

	SDL_Texture* frameBufferTex = SDL_CreateTexture(sdlRenderer,
		renderFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
	renderLoop(sdlRenderer, frameBufferTex, width, height);

	SDL_DestroyTexture(frameBufferTex);
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

bool initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Could not initialize SDL, error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

SDL_Window* createWindow(int screenWidth, int screenHeight) {
	SDL_Window *window = SDL_CreateWindow("ElRey", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cerr << "Could not create window, error: " << SDL_GetError() << std::endl;
	}
	return window;
}

void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height) {
	SDL_Event e;

	int numPixels = width*height;
	unsigned char* pixels;
	int pitch;
	SDL_LockTexture(frameBufferTex, NULL, (void**)&pixels, &pitch);

	int bytesPerPixel = pitch / width;
	int numBytes = pitch*height;
	std::cout << "Bytes per pixel: " << bytesPerPixel
		<< ", num bytes: " << numBytes << std::endl;

	Plane simplePlane(Point4(0.0, 0.0, 0.0, 1.0),
		Vector3(0.0, 1.0, 0.0), Color(1.0, 0.5, 0.0, 1.0));
	uint32_t lastFpsReportTime = SDL_GetTicks();
	FPSCounter fpsCounter;

	std::cout.precision(5);
	while(true) {
		bool quitPressed = false;
		while(SDL_PollEvent(&e) != 0) {
			quitPressed = (e.type == SDL_QUIT);
		}
		if (quitPressed) break;

		fpsCounter.PreFrame();

		SDL_LockTexture(frameBufferTex, NULL, (void**) &pixels, &pitch);
		
		for (int byteIndex = 0; byteIndex < numBytes-bytesPerPixel;
			byteIndex += bytesPerPixel) {
			pixels[byteIndex] = 0;
			pixels[byteIndex +1] = 255;
			pixels[byteIndex +2] = 0;
			if (bytesPerPixel == 4) {
				pixels[byteIndex + 3] = 255;
			}
		}
		SDL_UnlockTexture(frameBufferTex);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, frameBufferTex, NULL, NULL);
		SDL_RenderPresent(sdlRenderer);

		fpsCounter.PostFrame();
		uint32_t currTicks = SDL_GetTicks();
		if (currTicks > (lastFpsReportTime + 1000)) {

			std::cout << "Current FPS: "
				<< fpsCounter.GetFPS() << "\n";
			lastFpsReportTime = currTicks;
		}
	}
}

