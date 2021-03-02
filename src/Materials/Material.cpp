#include "Material.h"

Vector3 Material::ReflectVectorOffSurface(Vector3 const &normal,
										  Vector3 const &incomingVecFacingAwaySurface) const {
	return Vector3::Zero();
}
