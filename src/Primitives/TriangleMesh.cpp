#include "TriangleMesh.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

Mesh::Mesh() {
}

Mesh::Mesh(Mesh const& otherMesh) : vertices(otherMesh.vertices),
	normals(otherMesh.normals), vertexFaces(otherMesh.vertexFaces) {
}

Mesh::Mesh(Mesh && otherMesh) : vertices(std::move(otherMesh.vertices)),
	normals(std::move(otherMesh.normals)),
	vertexFaces(std::move(otherMesh.vertexFaces)) {
}

Mesh& Mesh::operator= (const Mesh& rhs) {
	if (this == &rhs) {
		return (*this);
	}
	vertices = rhs.vertices;
	normals = rhs.normals;
	vertexFaces = rhs.vertexFaces;
	
	return *this;
}



