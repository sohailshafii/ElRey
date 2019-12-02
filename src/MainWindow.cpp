#include <SDL.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cctype>
#include "ElReyConfig.h"
#include "Common.h"
#include "Performance/FPSCounter.h"
#include "Math/Plane.h"
#include "SceneData/Scene.h"
#include "Math/Sphere.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"
#include "CommonMath.h"
#include "Cameras/PinholeCamera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight);
Scene* createSimpleWorld(); 
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height, int numSamples, RandomSamplerType randomSamplerType,
	const Scene* gameWorld);

Uint32 lastFPSTickTime = 0; 

int main(int argc, char* argv[]) {
	CommonMath::SetRandSeed(static_cast <unsigned> (time(0)));
	std::cout << "ElRey version: " << ElRey_VERSION_MAJOR << "."
		<< ElRey_VERSION_MINOR << "\n";
	
	int width = 300, height = 200, numSamples = 1;
	bool offlineRender = false;
	RandomSamplerType randomSamplerType = None;

	if (argc > 1) {
		for (int argIndex = 1; argIndex < argc; argIndex++) {
			std::string currentToken = argv[argIndex];
			std::transform(currentToken.begin(),
				currentToken.end(), currentToken.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if (currentToken == "-w" && argIndex+1 < argc) {
				width = atoi(argv[++argIndex]);
			}
			if (currentToken == "-h" && argIndex+1 < argc) {
				height = atoi(argv[++argIndex]);
			}
			if (currentToken == "-ns" && argIndex+1 < argc) {
				numSamples = atoi(argv[++argIndex]);
			}
			if (currentToken == "-offline") {
				offlineRender = true;
			}
			if (currentToken == "-samplertype" && argIndex + 1 < argc) {
				std::string samplerTypeToken = argv[++argIndex];
				std::transform(samplerTypeToken.begin(),
					samplerTypeToken.end(), samplerTypeToken.begin(),
					[](unsigned char c) { return std::tolower(c); });
				if (samplerTypeToken == "random") {
					randomSamplerType = Random;
				}
				else if (samplerTypeToken == "jittered") {
					randomSamplerType = Jittered;
				}
				else if (samplerTypeToken == "nrooks") {
					randomSamplerType = NRooks;
				}
				else if (samplerTypeToken == "multijittered") {
					randomSamplerType = MultiJittered;
				}
				else {
					std::cerr << "Cannot understand sampler type specified: "
						<< samplerTypeToken.c_str() << std::endl;
				}
			}
		}
	}

	std::cout << "Framebuffer dimensions: " <<  width << "x" << height
		<< ", num samples: " << numSamples << ".\n";

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

	Scene *simpleWorld = createSimpleWorld();
	renderLoop(sdlRenderer, frameBufferTex, width, height, numSamples, randomSamplerType,
		simpleWorld);
	delete simpleWorld;

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

Scene* createSimpleWorld() {
	Plane* simplePlane = new Plane(Point3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f), Color(0.0f, 0.0f, 0.8f, 1.0f));
	Sphere* sphere = new Sphere(Point3(0.0f, 1.0f, 1.0f),
		0.3f, Color(1.0f, 0.0f, 0.0f, 1.0f));

	Primitive** simplePrimitives = new Primitive*[2];
	simplePrimitives[0] = simplePlane;
	simplePrimitives[1] = sphere;
	Scene *simpleWorld = new Scene(simplePrimitives, 2);
	return simpleWorld;
}

void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int widthPixels, int heightPixels, int numSamples,
	RandomSamplerType randomSamplerType, const Scene* gameWorld) {
	SDL_Event e;

	unsigned char* pixels;
	int pitch;
	SDL_LockTexture(frameBufferTex, NULL, (void**)&pixels, &pitch);

	int bytesPerPixel = pitch / widthPixels;
	int numBytes = pitch*heightPixels;
	std::cout << "Bytes per pixel: " << bytesPerPixel << ", num bytes: "
		<< numBytes << std::endl;
	if (randomSamplerType != None && numSamples == 1) {
		std::cerr << "You can't specify a random sampler with one sample. Defaulting " <<
			"to no random sampler.\n";
		randomSamplerType = None;
	}
	else if (randomSamplerType == None && numSamples > 1) {
		std::cerr << "An ordinary sampler cannot have more than one sample per pixel!\n";
		numSamples = 1;
	}

	GenericSampler* sampler = nullptr;
	switch (randomSamplerType) {
		case Jittered:
			sampler = new JitteredSampler(1, numSamples);
			break;
		case Random:
			sampler = new RandomSampler(1, numSamples);
			break;
		case NRooks:
			sampler = new NRooksSampler(1, numSamples);
			break;
		case MultiJittered:
			sampler = new MultiJitteredSampler(1, numSamples);
			break;
		default:
			sampler = new OneSampleSampler();
			break;
	}

	std::cout.precision(5);
	// assume left-handed coordinate system, where z goes into screen
	Point3 eyePosition(0.0f, 1.0f, 0.6f);
	float castPlaneHeight = 0.8f;
	float castPlaneWidth = castPlaneHeight * (float)widthPixels / (float)heightPixels;
	float rowHeight = castPlaneHeight / heightPixels;
	float colWidth = castPlaneWidth / widthPixels;
	std::cout << "Image plane dimensions: " << castPlaneWidth << " x "
		<< castPlaneHeight << " Row height: " << rowHeight << ", col width: "
		<< colWidth << ".\n";
	
	Point3 lookAtPosition = eyePosition + Vector3(0.0f, 0.0f, 0.2f);
	Vector3 upVector = Vector3::Up();
	PinholeCamera mainCamera(eyePosition, lookAtPosition, widthPixels, heightPixels,
							 castPlaneWidth, castPlaneHeight, upVector, randomSamplerType,
							 numSamples, 1);

	uint32_t lastFpsReportTime = SDL_GetTicks();
	FPSCounter fpsCounter;

	while(true) {
		bool quitPressed = false;
		while(SDL_PollEvent(&e) != 0) {
			quitPressed = (e.type == SDL_QUIT);
		}
		if (quitPressed) break;

		fpsCounter.PreFrame();

		SDL_LockTexture(frameBufferTex, NULL, (void**) &pixels, &pitch);

		mainCamera.CastIntoScene(pixels, bytesPerPixel, gameWorld);

		SDL_UnlockTexture(frameBufferTex);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, frameBufferTex, NULL, NULL);
		SDL_RenderPresent(sdlRenderer);

		fpsCounter.PostFrame();
		uint32_t currTicks = SDL_GetTicks();
		if (currTicks > (lastFpsReportTime + 1000)) {
			std::cout << "Current FPS: " << fpsCounter.GetFPS() << "\n";
			lastFpsReportTime = currTicks;
		}
	}

	delete sampler;
}

