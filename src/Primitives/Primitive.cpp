#include "Primitive.h"


void Primitive::SetLocalToWorld(Matrix4x4 const & localToWorld) {
	this->localToWorld = localToWorld;
}

void Primitive::SetWorldToLocal(Matrix4x4 const & worldToLocal) {
	this->worldToLocal = worldToLocal;
}

void Primitive::SetTransformAndInverse(Matrix4x4 const & localToWorld,
									   Matrix4x4 const & worldToLocal) {
	this->localToWorld = localToWorld;
	this->worldToLocal = worldToLocal;
}
