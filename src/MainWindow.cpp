#include <SDL.h>
#include <iostream>
#include <ctime>
#include "ElReyConfig.h"
#include "Performance/FPSCounter.h"
#include "Math/Plane.h"
#include "SceneData/Scene.h"
#include "Math/Sphere.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight);
Scene* createSimpleWorld(); 
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height, const Scene* gameWorld);

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

	Scene *simpleWorld = createSimpleWorld();
	renderLoop(sdlRenderer, frameBufferTex, width, height, simpleWorld);
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
	Plane* simplePlane = new Plane(Point4(0.0f, 0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 1.0f, 0.0f), Color(0.0f, 0.0f, 0.8f, 1.0f));
	Sphere* sphere = new Sphere(Point4(0.0f, 1.0f, 1.0f, 1.0f),
		0.3f, Color(1.0f, 0.0f, 0.0f, 1.0f));

	Primitive** simplePrimitives = new Primitive*[2];
	simplePrimitives[0] = simplePlane;
	simplePrimitives[1] = sphere;
	Scene *simpleWorld = new Scene(simplePrimitives, 2);
	return simpleWorld;
}

void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height, const Scene* gameWorld) {
	SDL_Event e;

	int numPixels = width*height;
	unsigned char* pixels;
	int pitch;
	SDL_LockTexture(frameBufferTex, NULL, (void**)&pixels, &pitch);

	int bytesPerPixel = pitch / width;
	int numBytes = pitch*height;
	std::cout << "Bytes per pixel: " << bytesPerPixel
		<< ", num bytes: " << numBytes << std::endl;

	Ray *raysToCast = new Ray[numPixels];
	// assume left-handed coordinate system, where z goes into screen
	Point4 eyePosition(0.0f, 1.0f, 0.0f, 1.0f);
	float castPlaneHeight = 2.0f;
	float castPlaneWidth = castPlaneHeight * (float)width / (float)height;
	float rowHeight = castPlaneHeight / height;
	float colWidth = castPlaneWidth / width;
	std::cout << "Image plane dimensions: " << castPlaneWidth << " x "
		<< castPlaneHeight << " Row height: " << rowHeight << ", col width: "
		<< colWidth << ".\n";
	Point4 planeUpperLeft(-castPlaneWidth * 0.5f + eyePosition[0],
		castPlaneHeight*0.5f + eyePosition[1], 1.0f + eyePosition[2], 1.0f);

	for (int row = 0, pixel = 0; row < height; row++) {
		for (int column = 0; column < width; column++, pixel++) {
			// find pixel center in world space
			Point4 pixelCenterWorld = planeUpperLeft + Point4(
				colWidth*(column + 0.5f), -rowHeight*(row + 0.5f), 0.0f, 1.0f);

			Vector3 vecToPixelCenter = pixelCenterWorld - eyePosition;
			vecToPixelCenter.Normalize();
			raysToCast[pixel].SetDirection(vecToPixelCenter);
			raysToCast[pixel].SetOrigin(eyePosition);
		}
	}

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

		float invGamma = (1.0/1.8f);
		float maxDist = std::numeric_limits<float>::max();
		for (int pixelIndex = 0, byteIndex = 0; pixelIndex < numPixels;
			pixelIndex++, byteIndex +=4) {
			float tMin = 0.0f, tMax = maxDist; Color intersectedColor = Color::Black();
			gameWorld->Intersect(raysToCast[pixelIndex], intersectedColor,
				0.0f, tMax);
			// gamma-correct
			intersectedColor ^= invGamma;
			pixels[byteIndex] = (unsigned char)(intersectedColor[2] * 255.0f); // B
			pixels[byteIndex + 1] = (unsigned char)(intersectedColor[1] * 255.0f); // G
			pixels[byteIndex + 2] = (unsigned char)(intersectedColor[0] * 255.0f); // R
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

	delete[] raysToCast;
}

