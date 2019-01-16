#include <SDL.h>
#include <iostream>
#include <fstream>
#include "ElReyConfig.h"
#include "Math/Vec3.h"
#include "Math/Ray.h"
#include "Math/HittableList.h"
#include "Math/Sphere.h"
#include "Math/MovingSphere.h"
#include "Math/Common.h"

#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectric.h"

#include "Camera/Camera.h"

bool initializeSDL();
SDL_Window* createWindow(int screenWidth, int screenHeight);
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex);
float HitSphere(const Vec3& center, float radius, const Ray &r);

Vec3 GetColorForRay(const Ray& r) {
	Vec3 sphereCenter = Vec3(0.0,0.0,-1.0);
	float t = HitSphere(sphereCenter, 0.5, r);
	if (t > 0.0) {
		Vec3 N = unitVector(r.PointAtParam(t) - sphereCenter);
		return 0.5*Vec3(N.x()+1, N.y()+1, N.z()+1);
	}

	Vec3 unitDirection = unitVector(r.direction());
	// y will go from -1 to 1; scale it to 0-1
	t = 0.5*(unitDirection.y() + 1.0);
	// interpolate from (0.5, 0.7, 1.0) to (1.0, 1.0, 1.0)
	return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}

int hitBlack = 0;
int numTotalCasts = 0;

Vec3 GetColorForRay(const Ray &r, Hittable *world, int depth) {
	HitRecord rec;
	numTotalCasts++;
	// first param is 0.001 -- gets rid of shadow acne
	// due to hitting the object they reflect from
	if (world->Hit(r, 0.001, MAXFLOAT, rec)) {
		// RandomPointInUnitSphere returns values from -1 to 1 in
		// all dimensions
		//Vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
		//return 0.5 * GetColorForRay(Ray(rec.p, target-rec.p), world);
		//return 0.5*Vec3(rec.normal.x()+1, rec.normal.y()+1,
		//	rec.normal.z()+1);
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation,
			scattered)) {
			return attenuation*GetColorForRay(scattered, world, depth+1);
		}
		else {
			hitBlack++;
			return Vec3(0.0, 0.0, 0.0);
		}
	}
	else {
		Vec3 unitDirection = unitVector(r.direction());
		// y will go from -1 to 1; scale it to 0-1
		float t = 0.5*(unitDirection.y() + 1.0);
		// interpolate from (0.5, 0.7, 1.0) to (1.0, 1.0, 1.0)
		return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
	}
}

float HitSphere(const Vec3& center, float radius, const Ray &r) {
	Vec3 centerToOrigin = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(centerToOrigin, r.direction());
	float c = dot(centerToOrigin, centerToOrigin) - radius*radius;
	float discriminant = b*b - 4*a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant))/(2.0*a);
	}
}

HittableList *randomScene() {
	int n = 50000;
	Hittable **list = new Hittable*[n+1];
	list[0] = new Sphere(Vec3(0.0,-1000.0,0.0), 1000,
		new Lambertian(Vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float chooseMat = drand48();
			Vec3 center(a + 0.9*drand48(),
				0.2, b + 0.9*drand48());

			if ((center - Vec3(4,0.2,0)).length() > 0.9) {
				if (chooseMat < 0.8) {
					list[i++] = new MovingSphere(center, center + Vec3(0, 0.5*drand48(), 0),
						0.0, 1.0, 0.2,
						new Lambertian(
							Vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if (chooseMat < 0.95) {
					list[i++] = new Sphere(center, 0.2,
						new Metal(
							Vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),
							0.5*drand48()));
				}
				else {
					// glass
					list[i++] = new Sphere(center, 0.2,
						new Dielectric(1.5));
				}			
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0,
		new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0,
		new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

	return new HittableList(list, i);
}

int main(int argc, char* argv[]) {
	std::cout << "ElRey version: " << ElRey_VERSION_MAJOR << "."
		<< ElRey_VERSION_MINOR << "\n";
	
	int width = 1920, height = 1080, numSamples = 10;

	std::ofstream ppmFile;
	ppmFile.open("outputImage.ppm");
	ppmFile << "P3\n" << width << " " << height << "\n255\n";
	float aspectRatio = (float)width/height;
	/*Vec3 lowerLeftCorner(-1.0*aspectRatio, -1.0, -1.0);
	Vec3 horizontal(2.0*aspectRatio, 0.0, 0.0);
	Vec3 vertical(0.0, 2.0, 0.0);
	Vec3 origin(0.0, 0.0, 0.0);*/

	//int numHittables = 2;
	//Hittable *list[numHittables];
	/*list[0] = new Sphere(Vec3(0.0, 0.0,-1.0), 0.5,
		new Lambertian(Vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(Vec3(0.0,-100.5,-1.0), 100,
		new Lambertian(Vec3(0.8, 0.8, 0.0)));

	list[2] = new Sphere(Vec3(1.0,0.0,-1.0), 0.5,
		new Metal(Vec3(0.8, 0.6, 0.2)));
	list[3] = new Sphere(Vec3(-1.0,0.0,-1.0), 0.5,
		new Dielectric(1.5));
	list[4] = new Sphere(Vec3(-1.0,0.0,-1.0), -0.45,
		new Dielectric(1.5));*/
	/*float R = cos(M_PI/4); 
	list[0] = new Sphere(Vec3(-R,0,-1), R,
		new Lambertian(Vec3(0, 0, 1)));
	list[1] = new Sphere(Vec3(R,0,-1), R,
		new Lambertian(Vec3(1, 0, 0)));*/

	//Hittable *world = new HittableList(list, numHittables);
	HittableList *world = randomScene();

	//Camera cam(90.0, aspectRatio);
	Vec3 lookFrom(13, 2, 3);
	Vec3 lookAt(0, 0, 0);
	float distanceToFocus = 10.0;//(lookFrom - lookAt).length();
	float aperture = 0.0;
	Camera cam(lookFrom, lookAt, Vec3(0, 1, 0), 20,
		aspectRatio, aperture, distanceToFocus, 0.0, 1.0);

	// TODO: move this code to frame buffer eventually
	for (int row = height-1; row >= 0; row--) {
		for (int column = 0; column < width; column++) {
			Vec3 colorVec(0.0, 0.0, 0.0);
			for (int sample = 0; sample < numSamples; sample++) {
				float u = float(column + drand48())/float(width);
				float v = float(row + drand48())/float(height);
				Ray r = cam.GetRay(u, v);
				//Vec3 p = r.PointAtParam(2.0);
				colorVec += GetColorForRay(r, world, 0);
			}
			colorVec /= float(numSamples);
			// gamma-correct (kinda) -- gamma-2
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
		hitBlack << " black pixels\n";
	ppmFile.close();

	auto hittableList = world->list;
	auto numHittables = world->listSize;
	for (int i = 0; i < numHittables; i++) {
		delete hittableList[i]->material;
		delete hittableList[i];
	}
	delete world;

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

