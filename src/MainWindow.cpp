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
#include "Math/Vector3.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"
#include "CommonMath.h"
#include "Cameras/PinholeCamera.h"
#include "Cameras/ThinLensCamera.h"
#include "Cameras/FisheyeCamera.h"
#include "Cameras/SphericalPanoramicCamera.h"
#include "Cameras/OrthographicCamera.h"

#include "SceneData/SceneLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TICKS_TO_SECONDS 1.0f/1000.0f

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight);
Scene* createSimpleWorld(const std::string& sceneFilePath); 
void startRenderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height, int numSamples, RandomSamplerType randomSamplerType,
	Camera::CameraType cameraType, Scene* gameWorld);

Uint32 lastFPSTickTime = 0; 

void convertTokenToLowerCase(std::string &token) {
	std::transform(token.begin(),
		token.end(), token.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

int main(int argc, char* argv[]) {
	CommonMath::SetRandSeed(static_cast <unsigned> (time(0)));
	std::cout << "ElRey version: " << ElRey_VERSION_MAJOR << "."
		<< ElRey_VERSION_MINOR << "\n";
	
	int width = 300, height = 200, numSamples = 1;
	bool offlineRender = false;
	RandomSamplerType randomSamplerType = None;
	Camera::CameraType cameraType = Camera::CameraType::Pinhole;
#if __APPLE__
	std::string scenePath = "../../sceneData.json";
#else
	std::string scenePath = "../sceneData.json";
#endif

	if (argc > 1) {
		for (int argIndex = 1; argIndex < argc; argIndex++) {
			std::string currentToken = argv[argIndex];
			convertTokenToLowerCase(currentToken);

			if (currentToken == "-w" && argIndex+1 < argc) {
				width = atoi(argv[++argIndex]);
			}
			else if (currentToken == "-h" && argIndex+1 < argc) {
				height = atoi(argv[++argIndex]);
			}
			else if (currentToken == "-scenePath" && argIndex+1 < argc) {
				scenePath = atoi(argv[++argIndex]);
			}
			else if (currentToken == "-ns" && argIndex+1 < argc) {
				numSamples = atoi(argv[++argIndex]);
			}
			else if (currentToken == "-cameratype" && argIndex + 1 < argc) {
				std::string cameraTypeToken = argv[++argIndex];
				convertTokenToLowerCase(cameraTypeToken);
				if (cameraTypeToken == "thinlens") {
					cameraType = Camera::CameraType::ThinLens;
				}
				else if (cameraTypeToken == "fisheye") {
					cameraType = Camera::CameraType::FishEye;
				}
				else if (cameraTypeToken == "spherical") {
					cameraType = Camera::CameraType::SphericalPanoramicCamera;
				}
				else if (cameraTypeToken == "orthographic") {
					cameraType = Camera::CameraType::Orthographic;
				}
			}
			else if (currentToken == "-offline") {
				offlineRender = true;
			}
			else if (currentToken == "-samplertype" && argIndex + 1 < argc) {
				std::string samplerTypeToken = argv[++argIndex];
				convertTokenToLowerCase(samplerTypeToken);
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
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(window, width / 2, height / 2);
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_ShowCursor(0);
	
	Scene *simpleWorld = createSimpleWorld(scenePath);
	startRenderLoop(sdlRenderer, frameBufferTex, width, height,
		numSamples, randomSamplerType, cameraType, simpleWorld);
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

Scene* createSimpleWorld(const std::string& sceneFilePath) {
	Scene *simpleWorld = new Scene();
	SceneLoader::DeserializeJSONFileIntoScene(simpleWorld,
											  sceneFilePath);
	return simpleWorld;
}

Vector3 getMovementVectorFromKeyPresses(const SDL_Event &event) {
	Vector3 movementVector(0.0f, 0.0f, 0.0f);
	if (event.type == SDL_KEYDOWN) {
		switch(event.key.keysym.sym) {
			case SDLK_LEFT:
			case SDLK_a:
				movementVector[0] =-1.0f;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				movementVector[0] = 1.0f;
				break;
			case SDLK_UP:
			case SDLK_w:
				movementVector[2] = 1.0f;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				movementVector[2] =-1.0f;
				break;
		}
	}
	return movementVector;
}

Vector3 getMouseMovementVector(const SDL_Event &event) {
	Vector3 mouseMoveVector(0.0f, 0.0f, 0.0f);
	if(event.type == SDL_MOUSEMOTION)
	{
		mouseMoveVector[0] = (float)-event.motion.yrel;
		mouseMoveVector[1] = (float)-event.motion.xrel;
	}
	return mouseMoveVector;
}

void startRenderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int widthPixels, int heightPixels, int numSamples,
	RandomSamplerType randomSamplerType, Camera::CameraType cameraType,
	Scene* gameWorld) {

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

	uint32_t lastFpsReportTime = SDL_GetTicks();
	FPSCounter fpsCounter;
	
	SDL_Event event;
	uint32_t lastFrameTicks = SDL_GetTicks();
	Vector3 rotationVector(0.0f, 0.0f, 0.0f);
	while(true) {
		uint32_t currTicks = SDL_GetTicks();
		bool quitPressed = false;
		Vector3 translationVector(0.0f, 0.0f, 0.0f);
		float frameTime = (float)(currTicks - lastFrameTicks) * TICKS_TO_SECONDS;

		while(SDL_PollEvent(&event) != 0) {
			quitPressed = (event.type == SDL_QUIT);
			if (!quitPressed) {
				Vector3 currentMovement = getMovementVectorFromKeyPresses(event);
				translationVector += currentMovement;
				
				Vector3 currentRotation = getMouseMovementVector(event);
				// affect by speed
				currentRotation[0] *= 10.0f*frameTime;
				currentRotation[1] *= 10.0f*frameTime;
				rotationVector += currentRotation;
				if (rotationVector[0] > 89.0f) {
					rotationVector[0] = 89.0f;
				}
				if (rotationVector[0] < -89.0f) {
					rotationVector[0] = -89.0f;
				}
			}
		}
		if (quitPressed) break;

		fpsCounter.PreFrame();

		SDL_LockTexture(frameBufferTex, NULL, (void**) &pixels, &pitch);

		translationVector *= 3.0f*frameTime;
		gameWorld->TranslateAndRotate(translationVector, rotationVector[0], rotationVector[1]);
		gameWorld->CastIntoScene(pixels, bytesPerPixel, frameTime);

		SDL_UnlockTexture(frameBufferTex);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, frameBufferTex, NULL, NULL);
		SDL_RenderPresent(sdlRenderer);

		fpsCounter.PostFrame();
		if (currTicks > (lastFpsReportTime + 1000)) {
			std::cout << "Current FPS: " << fpsCounter.GetFPS() << "\n";
			lastFpsReportTime = currTicks;
			//std::cout << rotationVector[0] << " " << rotationVector[1] << std::endl;
		}
		lastFrameTicks = currTicks;
	}
}

