#pragma once

#include <memory>
#include "Materials/Color.h"
#include "Math/Ray.h"
#include "Materials/Material.h"

class Primitive {
public:
	// TODO: by ref or not?
	Primitive(std::shared_ptr<Material> material)
		: material(material)
	{ }
	virtual ~Primitive() { }

	virtual bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax) = 0;

protected:
	std::shared_ptr<Material> material;
};
