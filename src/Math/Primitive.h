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

	virtual bool Intersect(const Ray &ray, float tMin, float& tMax) = 0;
	virtual Vector3 GetNormalAtPosition(const Point3& position) const = 0;
	
	std::shared_ptr<Material> GetMaterial() {
		return material;
	}

protected:
	std::shared_ptr<Material> material;
};
