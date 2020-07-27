#include "Primitive.h"

void Primitive::SetInverseTransformation(Matrix4x4 const & worldToLocal) {
	this->worldToLocal = worldToLocal;
}
