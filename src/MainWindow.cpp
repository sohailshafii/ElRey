#include <SDL.h>
#include <iostream>
#include <fstream>
#include "ElReyConfig.h"
#include "Math/Vec3.h"
#include "Math/Ray.h"

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight);
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex);
bool HitSphere(const Vec3& center, float radius, const Ray &r);

Vec3 GetColorForRay(const Ray& r) {
	if (HitSphere(Vec3(0.0,0.0,-1.0), 0.5, r)) {
		return Vec3(1.0, 0.0, 0.0);
	}

	Vec3 unitDirection = unitVector(r.direction());
	// y will go from -1 to 1; scale it to 0-1
	float t = 0.5*(unitDirection.y() + 1.0);
	// interpolate from (0.5, 0.7, 1.0) to (1.0, 1.0, 1.0)
	return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}

bool HitSphere(const Vec3& center, float radius, const Ray &r) {
	Vec3 centerToOrigin = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(centerToOrigin, r.direction());
	float c = dot(centerToOrigin, centerToOrigin) - radius*radius;
	float discriminant = b*b - 4*a*c;
	return (discriminant > 0);
}

int main(int argc, char* argv[]) {
	std::cout << "ElRey version: " << ElRey_VERSION_MAJOR << "."
		<< ElRey_VERSION_MINOR << "\n";
	
	int width = 200, height = 100;

	std::ofstream ppmFile;
	ppmFile.open("outputImage.ppm");
	ppmFile << "P3\n" << width << " " << height << "\n255\n";
	float aspectRatio = (float)width/height;
	Vec3 lowerLeftCorner(-1.0*aspectRatio, -1.0, -1.0);
	Vec3 horizontal(2.0*aspectRatio, 0.0, 0.0);
	Vec3 vertical(0.0, 2.0, 0.0);
	Vec3 origin(0.0, 0.0, 0.0);

	// TODO: move this code to frame buffer eventually
	for (int row = height-1; row >= 0; row--) {
		for (int column = 0; column < width; column++) {
			float u = float(column)/float(width),
				v = float(row)/float(height);
			//Vec3 colorVec(float(column)/float(width), float(row)/float(height),
			//	0.2);
			Ray r(origin, lowerLeftCorner + u*horizontal + v*vertical);
			Vec3 colorVec = GetColorForRay(r);
			int ir = int(255.99*colorVec[0]);
			int ig = int(255.99*colorVec[1]);
			int ib = int(255.99*colorVec[2]);
			ppmFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	ppmFile.close();	

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

	SDL_Texture* frameBufferTex = SDL_CreateTexture(sdlRenderer,
		SDL_GetWindowPixelFormat(window),
		SDL_TEXTUREACCESS_STREAMING, width, height); 
	renderLoop(sdlRenderer, frameBufferTex);

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

void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex) {
	SDL_Event e;

	while(true) {

		bool quitPressed = false;
		while(SDL_PollEvent(&e) != 0) {
			quitPressed = (e.type == SDL_QUIT);
		}
		if (quitPressed) break;

		void* pixels;
		int pitch;
		SDL_LockTexture(frameBufferTex, NULL, &pixels, &pitch);
		// TODO: render here
		SDL_UnlockTexture(frameBufferTex);
		SDL_RenderCopy(sdlRenderer, frameBufferTex, NULL, NULL);
	}
}

