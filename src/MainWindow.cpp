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
void renderLoop(SDL_Renderer *sdlRenderer, SDL_Texture* frameBufferTex);
float HitSphere(const Vec3& center, float radius, const Ray &r);

Vec3 GetColorForRay(const Ray& r) {
	Vec3 sphereCenter = Vec3(0.0f,0.0f,-1.0f);
	float t = HitSphere(sphereCenter, 0.5f, r);
	if (t > 0.0f) {
		Vec3 N = unitVector(r.PointAtParam(t) - sphereCenter);
		return 0.5f*Vec3(N.x()+1.0f, N.y()+1.0f, N.z()+1.0f);
	}

	Vec3 unitDirection = unitVector(r.direction());
	// y will go from -1 to 1; scale it to 0-1
	t = 0.5f*(unitDirection.y() + 1.0f);
	// interpolate from (0.5, 0.7, 1.0) to (1.0, 1.0, 1.0)
	return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
}

int hitBlack = 0;
int numTotalCasts = 0;
float tMin = 0.0, tMax = 1.0;

int scatterAtAll = 0;

Vec3 GetColorForRay(const Ray &r, Hittable *world, int depth) {
	HitRecord rec;
	numTotalCasts++;
	// first param is 0.001 -- gets rid of shadow acne
	// due to hitting the object they reflect from
	if (world->Hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		// RandomPointInUnitSphere returns values from -1 to 1 in
		// all dimensions
		//Vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
		//return 0.5 * GetColorForRay(Ray(rec.p, target-rec.p), world);
		//return 0.5*Vec3(rec.normal.x()+1, rec.normal.y()+1,
		//	rec.normal.z()+1);
		Ray scattered;
		Vec3 albedo;
		Vec3 emitted = rec.matPtr->emitted(r, rec, rec.u, rec.v,
			rec.p);
		float pdf = 1.0;

		if (depth < 50 && rec.matPtr->scatter(r, rec, albedo,
			scattered, pdf)) {
			/*Vec3 onLight(213.0f + getRand()*(343.0f - 213.0f),
				554.0f, 227.0f + getRand()*(332.0f - 227.0f));
			Vec3 toLight = onLight - rec.p;
			float distanceSquared = toLight.squaredLength();
			toLight.makeUnitVector();
			if (dot(toLight, rec.normal) < 0.0f) {
				return emitted;
			}
			float lightArea = (343.0f - 213.0f)*(332.0f - 227.0f);
			float lightCosine = fabs(toLight.y());
			if (lightCosine < 0.000001f) {
				return emitted;
			}
			pdf = distanceSquared / (lightCosine * lightArea);
			scattered = Ray(rec.p, toLight, r.time());
			*/
			
			std::shared_ptr<Hittable> lightShape
				= std::make_shared<XzRect>(213.0f, 343.0f, 227.0f,
					332.0f, 554.0f, nullptr);
			std::shared_ptr<HittablePdf> p0
				=	std::make_shared<HittablePdf>(lightShape, rec.p);
			std::shared_ptr<CosinePdf> p1
				= std::make_shared<CosinePdf>(rec.normal);
			MixturePdf mixPdf(p0, p1);
			scattered = Ray(rec.p, mixPdf.generate(), r.time());
			pdf = mixPdf.value(scattered.direction());

			scatterAtAll++;
			return emitted +
				albedo*
				rec.matPtr->scatteringPdf(r, rec, scattered)*
				GetColorForRay(scattered, world, depth+1)/pdf;
		}
		else {
			hitBlack++;
			return emitted;
		}
	}
	else {
		return Vec3(0.0f, 0.0f, 0.0f);
		/*Vec3 unitDirection = unitVector(r.direction());
		// y will go from -1 to 1; scale it to 0-1
		float t = 0.5*(unitDirection.y() + 1.0);
		// interpolate from (0.5, 0.7, 1.0) to (1.0, 1.0, 1.0)
		return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);*/
	}
}

float HitSphere(const Vec3& center, float radius, const Ray &r) {
	Vec3 centerToOrigin = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(centerToOrigin, r.direction());
	float c = dot(centerToOrigin, centerToOrigin) - radius*radius;
	float discriminant = b*b - 4.0f*a*c;
	if (discriminant < 0.0f) {
		return -1.0f;
	}
	else {
		return (-b - sqrt(discriminant))/(2.0f*a);
	}
}

HittableList *randomScene() {
	int n = 50000;
	Hittable **list = new Hittable*[n+1];

	std::shared_ptr<CheckerTexture> checkerTex = std::make_shared<CheckerTexture>(CheckerTexture(
		new ConstantTexture(Vec3(0.2f, 0.3f, 0.1f)),
		new ConstantTexture(Vec3(0.9f, 0.9f, 0.9f))));
	list[0] = new Sphere(Vec3(0.0f,-1000.0f,0.0f), 1000,
		std::make_shared<Lambertian>(Lambertian(checkerTex)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			float chooseMat = getRand();
			Vec3 center(a + 0.9f*getRand(),
				0.2f, b + 0.9f*getRand());

			if ((center - Vec3(4.0f,0.2f,0.0f)).length() > 0.9f) {
				if (chooseMat < 0.8f) {
					list[i++] = new MovingSphere(center, center + Vec3(0.0f, 
						0.5f*getRand(), 0.0f),
						tMin, tMax, 0.2f,
						std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>(ConstantTexture
							(Vec3(getRand()*getRand(), getRand()*getRand(), getRand()*getRand())))
							))
						);
				}
				else if (chooseMat < 0.95f) {
					list[i++] = new Sphere(center, 0.2f,
							std::make_shared<Metal>(Metal(
								Vec3(0.5f*(1.0f + getRand()), 0.5f*(1.0f + getRand()), 0.5f*(1.0f + getRand())),
								0.5f*getRand())
							)
						);
				}
				else {
					// glass
					list[i++] = new Sphere(center, 0.2f,
						std::make_shared<Dielectric>(Dielectric(1.5f)));
				}			
			}
		}
	}

	list[i++] = new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0f,
		std::make_shared<Dielectric>(Dielectric(1.5)));
	list[i++] = new Sphere(Vec3(-4.0f, 1.0f, 0.0f), 1.0f,
			std::make_shared<Lambertian>(Lambertian(std::make_shared<ConstantTexture>
					(ConstantTexture(Vec3(0.4f, 0.2f, 0.1f)))
				)
			)
		);
	list[i++] = new Sphere(Vec3(4.0f, 1.0f, 0.0f), 1.0f,
		std::make_shared<Metal>(Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f))
		);

	return new HittableList(list, i);
}

HittableList* TwoPerlinSpheres() {
	std::shared_ptr<NoiseTexture> perlinTexture = 
		std::make_shared<NoiseTexture>(NoiseTexture(1.0));
	Hittable **list = new Hittable*[2];

	int nx, ny, nn;
	unsigned char *texData = stbi_load("earthFromImgur.jpg",
		&nx, &ny, &nn, 0);
	ImageTexture* earthTexture = nullptr;
	if (texData == nullptr) {
		throw std::runtime_error("Could not load earth texture!");
	}
	else {
		std::cout << "Loaded earth texture: " << nx << 
			" x " << ny << std::endl;
		earthTexture = new ImageTexture(texData, nx, ny);
	}

	list[0] = new Sphere(Vec3(0,-1000, 0), 1000.0,
		std::make_shared<Lambertian>(Lambertian(perlinTexture)));
	list[1] = new Sphere(Vec3(0,2, 0), 2.0,
		std::make_shared<Lambertian>
		(Lambertian(std::shared_ptr<ImageTexture>(earthTexture))));
	return new HittableList(list, 2);
}

HittableList* simpleLight() {
	std::shared_ptr<NoiseTexture> perlinTexture = 
		std::make_shared<NoiseTexture>(NoiseTexture(4.0));
	Hittable **list = new Hittable*[4];
	
	list[0] = new Sphere(Vec3(0,-1000, 0), 1000.0,
		std::make_shared<Lambertian>
		(Lambertian(perlinTexture)));
	list[1] = new Sphere(Vec3(0, 2, 0), 2.0,
		std::make_shared<Lambertian>
		(Lambertian(perlinTexture)));
	list[2] = new Sphere(Vec3(0, 7, 0), 2.0,
		std::make_shared<DiffuseLight>
		(DiffuseLight(
			std::make_shared<ConstantTexture>
			(ConstantTexture(Vec3(4, 4, 4)))
			))
		);
	list[3] = new XyRect(3, 5, 1, 3, -2,
		std::make_shared<DiffuseLight>(
			DiffuseLight(std::make_shared<ConstantTexture>
			(ConstantTexture(Vec3(4, 4, 4)))
			))
		);
	return new HittableList(list, 4);	
}

HittableList* CornellBox() {
	Hittable **listItems = new Hittable*[8];

	std::shared_ptr<Lambertian> red = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.65f, 0.05f, 0.05f))))
		);
	std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.73f, 0.73f, 0.73f))))
		);
	std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.12f, 0.45f, 0.15f))))
		);
	std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(
			DiffuseLight(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(15.0f, 15.0f, 15.0f))))
		);

	int i = 0;
	listItems[i++] = new FlippedNormalsHittable(
		std::make_shared<YzRect>(YzRect(0, 555, 0, 555, 555, green)
			)
		); 
	listItems[i++] = new YzRect(0, 555, 0, 555, 0, red);

	listItems[i++] = new FlippedNormalsHittable(
			std::make_shared<XzRect>(XzRect(213, 343, 227, 332, 554, light))
		);
	listItems[i++] = new FlippedNormalsHittable(
		std::make_shared<XzRect>(XzRect(0, 555, 0, 555, 555, white))
		);

	listItems[i++] = new XzRect(0, 555, 0, 555, 0, white);
	listItems[i++] = new FlippedNormalsHittable(
		std::make_shared<XyRect>(XyRect(0, 555, 0,
			555, 555, white)
		)
	);

	auto rotatedBox = new RotateY(new Box(Vec3(0, 0, 0),
		Vec3(165, 165, 165), white), -18.0);
	auto translatedBox = new Translate((Hittable*)rotatedBox,
		Vec3(130, 0, 65));
	listItems[i++] = translatedBox;

	rotatedBox = new RotateY(new Box(Vec3(0, 0, 0),
		Vec3(165, 330, 165), white), 15.0);
	translatedBox = new Translate((Hittable*)rotatedBox,
		Vec3(265, 0, 295));
	listItems[i++] = translatedBox;

	/*auto rotatedBox = new RotateY(new Box(Vec3(0, 0, 0),
		Vec3(165, 165, 165), white), -18.0);
	auto translatedBox = new Translate((Hittable*)rotatedBox,
		Vec3(130, 0, 65));
	auto constantMed = new ConstantMedium((Hittable*)translatedBox,
		0.01f, std::make_shared<ConstantTexture>(
			ConstantTexture(Vec3(1.0, 1.0, 1.0))));
	listItems[i++] = constantMed;

	rotatedBox = new RotateY(new Box(Vec3(0, 0, 0),
		Vec3(165, 330, 165), white), 15.0);
	translatedBox = new Translate((Hittable*)rotatedBox,
		Vec3(265, 0, 295)); 
	constantMed = new ConstantMedium((Hittable*)translatedBox,
		0.01f, std::make_shared<ConstantTexture>(
			ConstantTexture(Vec3(0.0, 0.0, 0.0))));
	listItems[i++] = constantMed;*/

	return new HittableList(listItems, i);
}

HittableList* CornellBox2(Camera** cam, float aspectRatio) {
	Hittable **listItems = new Hittable*[8];

	std::shared_ptr<Lambertian> red = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.65f, 0.05f, 0.05f))))
		);
	std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.73f, 0.73f, 0.73f))))
		);
	std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.12f, 0.45f, 0.15f))))
		);
	std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(
			DiffuseLight(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(15.0f, 15.0f, 15.0f))))
		);

	int i = 0;
	listItems[i++] = new FlippedNormalsHittable(
		std::make_shared<YzRect>(YzRect(0, 555, 0, 555, 555, green)
			)
		); 
	listItems[i++] = new YzRect(0, 555, 0, 555, 0, red);

	listItems[i++] = new XzRect(213, 343, 227, 332, 554, light);
	listItems[i++] = new FlippedNormalsHittable(
		std::make_shared<XzRect>(XzRect(0, 555, 0, 555, 555, white))
		);

	listItems[i++] = new XzRect(0, 555, 0, 555, 0, white);
	listItems[i++] = new FlippedNormalsHittable(
		std::make_shared<XyRect>(XyRect(0, 555, 0,
			555, 555, white)
		)
	);

	auto rotatedBox = new RotateY(new Box(Vec3(0, 0, 0),
		Vec3(165, 165, 165), white), -18.0);
	auto translatedBox = new Translate((Hittable*)rotatedBox,
		Vec3(130, 0, 65));
	auto constantMed = new ConstantMedium((Hittable*)translatedBox,
		0.01f, std::make_shared<ConstantTexture>(
			ConstantTexture(Vec3(1.0, 1.0, 1.0))));
	listItems[i++] = constantMed;

	rotatedBox = new RotateY(new Box(Vec3(0, 0, 0),
		Vec3(165, 330, 165), white), 15.0);
	translatedBox = new Translate((Hittable*)rotatedBox,
		Vec3(265, 0, 295)); 
	constantMed = new ConstantMedium((Hittable*)translatedBox,
		0.01f, std::make_shared<ConstantTexture>(
			ConstantTexture(Vec3(0.0, 0.0, 0.0))));
	listItems[i++] = constantMed;

	Vec3 lookFrom(278, 278,-800);
	Vec3 lookAt(278, 278, 0);
	//Vec3 lookFrom(13, 2, 3);
	//Vec3 lookAt(0, 0, 0);
	float distanceToFocus = 10.0;//(lookFrom - lookAt).length();
	float aperture = 0.0;
	float vfov = 40.0;

	*cam = new Camera(lookFrom, lookAt, Vec3(0, 1, 0), vfov,
		aspectRatio, aperture, distanceToFocus, 0.0, 1.0);

	return new HittableList(listItems, i);
}

HittableList* Final() {
	int nb = 20;
	Hittable **list = new Hittable*[30];
	Hittable **boxList = new Hittable*[nb*nb];

	std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.73f, 0.73f, 0.73f))))
		);
	std::shared_ptr<Lambertian> ground = std::make_shared<Lambertian>(
			Lambertian(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(0.48f, 0.83f, 0.53f))))
		);

	int b = 0;
	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < nb; j++) {
			float w = 100.0f;
			float x0 = -1000.0f + (float)i*w;
			float z0 = -1000.0f + (float)j*w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100.0f*(getRand()+0.01f);
			float z1 = z0 + w;
			boxList[b++] = new Box(Vec3(x0, y0, z0),
				Vec3(x1, y1, z1), ground);
		}
	}

	int l = 0;
	list[l++] = new BVHNode(boxList, b, 0.0f, 1.0f, true);
	std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(
			DiffuseLight(
			std::make_shared<ConstantTexture>(
				ConstantTexture(Vec3(7.0f, 7.0f, 7.0f))))
		);
	list[l++] = new XzRect(123.0f, 423.0f, 147.0f, 412.0f, 554.0f, light);
	Vec3 center(400.0f, 400.0f, 200.0f);
	list[l++] = new MovingSphere(center, 
		center + Vec3(30.0f, 0.0f, 0.0f),
		0.0f, 1.0f, 50.0f,
		std::make_shared<Lambertian>(Lambertian(
			std::make_shared<ConstantTexture>(ConstantTexture
			(Vec3(0.7f, 0.3f, 0.1f)))
			))
		);
	list[l++] = new Sphere(Vec3(260.0f, 150.0f, 45.0f),
		50.0f, std::make_shared<Dielectric>(Dielectric(1.5f)));
	list[l++] = new Sphere(Vec3(0.0f, 150.0f, 145.0f),
		50.0f, std::make_shared<Metal>(
			Metal(Vec3(0.8f, 0.8f, 0.9f), 10.0f)));
	Hittable *boundary = new Sphere(Vec3(360.0f, 150.0f, 145.0f),
		70.0f, std::make_shared<Dielectric>(Dielectric(1.5f)));
	list[l++] = boundary;
	list[l++] = new ConstantMedium(boundary, 0.2f,
		std::make_shared<ConstantTexture>(ConstantTexture(
			Vec3(0.2f, 0.4f, 0.9f))));

	boundary = new Sphere(Vec3(0.0f, 0.0f, 0.0f),
		5000.0f, std::make_shared<Dielectric>(Dielectric(1.5f)));
	list[l++] = new ConstantMedium(boundary, 0.0001f,
		std::make_shared<ConstantTexture>(ConstantTexture(
			Vec3(1.0f, 1.0f, 1.0))));

	int nx, ny, nn;
	unsigned char *texData = stbi_load("earthFromImgur.jpg",
		&nx, &ny, &nn, 0);
	ImageTexture* earthTexture = nullptr;
	if (texData == nullptr) {
		throw std::runtime_error("Could not load earth texture!");
	}
	else {
		std::cout << "Loaded earth texture: " << nx << 
			" x " << ny << std::endl;
		earthTexture = new ImageTexture(texData, nx, ny);
	}
	std::shared_ptr<Material> eMat = std::make_shared<Lambertian>(
			Lambertian(std::shared_ptr<ImageTexture>(earthTexture)));
	list[l++] = new Sphere(Vec3(400.0f, 200.0f, 400.0f), 100.0f, eMat);
	std::shared_ptr<Texture> perText =
		std::make_shared<NoiseTexture>(
			NoiseTexture(0.1f));
	list[l++] = new Sphere(Vec3(220.0f, 280.0f, 300.0f),
		80.0f, std::make_shared<Lambertian>
		(Lambertian(perText)));

	int ns = 1000;
	Hittable **boxList2 = new Hittable*[ns];
	for (int j = 0; j < ns; j++) {
		boxList2[j] = new Sphere(Vec3(165.0f*getRand(),
			165.0f*getRand(), 165.0f*getRand()),
		10.0f, white);
	}

	auto newBvh = new BVHNode(boxList2, ns, 0.0f, 1.0f, true);
	auto rotatedBvh = new RotateY(newBvh, 15.0f);
	auto translatedRotated = new Translate(rotatedBvh,
		Vec3(-100.0f, 270.0f, 395.0f));
	list[l++] = translatedRotated;

	return new HittableList(list, l);
}


int main(int argc, char* argv[]) {
	srand(static_cast <unsigned> (time(0)));
	std::cout << "ElRey version: " << ElRey_VERSION_MAJOR << "."
		<< ElRey_VERSION_MINOR << "\n";
	
	int width = 400, height = 300, numSamples = 10;
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
		}
	}

	std::cout << "Framebuffer dimensions: " <<  width << "x" << height
		<< ", num samples: " << numSamples << ".\n";


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
	std::time_t startBuild = std::time(nullptr);
	//HittableList *world = Final();//CornellBox();//simpleLight();//TwoPerlinSpheres();//randomScene();
	//Camera *cam;
	HittableList *world = CornellBox();//CornellBox2(&cam, aspectRatio);

	BVHNode bvhWorld(world->list, world->listSize, tMin, tMax, false);

	std::cout << "Constructed world and acceleration structure\n";
	//Camera cam(90.0, aspectRatio);
	Vec3 lookFrom(278, 278,-800);
	Vec3 lookAt(278, 278, 0);
	//Vec3 lookFrom(13, 2, 3);
	//Vec3 lookAt(0, 0, 0);
	float distanceToFocus = 10.0;//(lookFrom - lookAt).length();
	float aperture = 0.0;
	float vfov = 40.0;

	Camera cam(lookFrom, lookAt, Vec3(0, 1, 0), vfov,
		aspectRatio, aperture, distanceToFocus, 0.0, 1.0);
	std::cout << "Scene construction time: " << difftime(std::time(nullptr), startBuild) << ".\n";

	std::time_t startRender = std::time(nullptr);
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
				colorVec += GetColorForRay(r, &bvhWorld, 0);

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

	delete world;
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

