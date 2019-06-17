#include "Primitive.h"
#include "Math/Point4.h"
#include "Math/Vector3.h"

class Plane : public Primitive {
public:
	Plane(Point4 origin, Vector3 normal) {
		this->origin = origin;
		this->normal = normal;
	}

	bool GetHit(const Ray &ray, Color &newColor) override;

private:
	Point4 origin;
	Vector3 normal;
};