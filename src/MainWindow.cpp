#include <SDL.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include "ElReyConfig.h"
#include "Math/Vec3.h"
#include "Math/Ray.h"
#include "Math/HittableList.h"
#include "Math/Sphere.h"
#include "Math/MovingSphere.h"
#include "Math/Common.h"
#include "Math/BVHNode.h"
#include "Math/XyRect.h"
#include "Math/ConstantMedium.h"

#include "Math/XzRect.h"
#include "Math/YzRect.h"
#include "Math/FlippedNormalsHittable.h"
#include "Math/Box.h"
#include "Math/Translate.h"
#include "Math/RotateY.h"

#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectric.h"
#include "Materials/ConstantTexture.h"
#include "Materials/CheckerTexture.h"
#include "Materials/NoiseTexture.h"
#include "Materials/ImageTexture.h"
#include "Materials/DiffuseLight.h"

#include "Math/CosinePdf.h"
#include "Math/HittablePdf.h"
#include "Math/MixturePdf.h"

#include "Camera/Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight); 
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex,
	int width, int height);

int hitBlack = 0;
int numTotalCasts = 0;
float tMin = 0.0, tMax = 1.0;

int scatterAtAll = 0;


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


	//std::ofstream ppmFile;
	//ppmFile.open("outputImage.ppm");
	//ppmFile << "P3\n" << width << " " << height << "\n255\n";
	float aspectRatio = (float)width/height;

	Vec3 lookFrom(278, 278,-800);
	Vec3 lookAt(278, 278, 0);
	float distanceToFocus = 10.0;//(lookFrom - lookAt).length();
	float aperture = 0.0;
	float vfov = 40.0;

	Camera cam(lookFrom, lookAt, Vec3(0, 1, 0), vfov,
		aspectRatio, aperture, distanceToFocus, 0.0, 1.0);

	/*std::time_t startRender = std::time(nullptr);
	int numTotalSamples = width*height*numSamples;
	int sampleCount = 0;
	float lastPercentage = 0.0f;

	// TODO: move this code to frame buffer eventually
	for (int row = height-1; row >= 0; row--) {
		for (int column = 0; column < width; column++) {
			Vec3 colorVec(0.0, 0.0, 0.0);
			for (int sample = 0; sample < numSamples; sample++) {
				float u = float(column + getRand())/float(width);
				float v = float(row + getRand())/float(height);
				Ray r = cam.GetRay(u, v);
				//Vec3 p = r.PointAtParam(2.0);
				colorVec += deNan(GetColorForRay(r, &bvhWorld, hittableList, 0));

				sampleCount++;
				float newPercentage = 100.0f*(float)sampleCount/
					(float)numTotalSamples;
				if ((newPercentage - lastPercentage) > 1.0f) {
					std::cout << "Percentage complete: " <<
						newPercentage << ", time so far: " <<
						difftime(std::time(nullptr), startRender) 
						<< ".\n";

					lastPercentage = newPercentage;
				}
			}
			colorVec /= float(numSamples);
			// gamma-correct (kinda) -- gamma-2
			// this can cause green-ish pixels to become magenta
			// like if ground in final scene intersects with light,
			// it gets shifted from green to magenta
			colorVec = Vec3(sqrt(colorVec[0]), sqrt(colorVec[1]),
				sqrt(colorVec[2]));

			//float u = float(column)/float(width),
			//	v = float(row)/float(height);
			//Vec3 colorVec(float(column)/float(width), float(row)/float(height),
			//	0.2);
			//Ray r(origin, lowerLeftCorner + u*horizontal + v*vertical);
			//Vec3 colorVec = GetColorForRay(r, world);
			int ir = int(255.99*colorVec[0]);
			int ig = int(255.99*colorVec[1]);
			int ib = int(255.99*colorVec[2]);
			ppmFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	std::cout << "out of " << numTotalCasts <<  " casts, hit: " << 
		hitBlack << " black pixels. Scattered: " << scatterAtAll << "\n";
	ppmFile.close();
	std::cout << "Render time: " << difftime(std::time(nullptr), startRender) << ".\n";

	delete world;*/
	//delete cam;

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

	std::time_t lastReportTime = std::time(nullptr);
	std::cout.precision(5);
	while(true) {
		bool quitPressed = false;
		while(SDL_PollEvent(&e) != 0) {
			quitPressed = (e.type == SDL_QUIT);
		}
		if (quitPressed) break;

		std::time_t startFrameTime = std::time(nullptr);
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

		double diffRepTime =
			std::difftime(std::time(nullptr), lastReportTime);
		if (diffRepTime > 1.0) {
			double diffTime = std::difftime(std::time(nullptr), startFrameTime);
			std::cout << "FPS: "
				<<  1.0/diffRepTime << ".\n";
			lastReportTime = std::time(nullptr);
		}
	}
}

