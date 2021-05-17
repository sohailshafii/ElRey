#include "TriangleMesh.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include "Math/Point2.h"

TriangleMesh::TriangleMesh() {
}

TriangleMesh::TriangleMesh(TriangleMesh const& otherMesh) : vertices(otherMesh.vertices),
	normals(otherMesh.normals), vertexFaces(otherMesh.vertexFaces),
	textureCoords(otherMesh.textureCoords) {
}

TriangleMesh::TriangleMesh(TriangleMesh && otherMesh) : vertices(std::move(otherMesh.vertices)),
	normals(std::move(otherMesh.normals)),
	vertexFaces(std::move(otherMesh.vertexFaces)),
	textureCoords(std::move(otherMesh.textureCoords)) {
}

TriangleMesh& TriangleMesh::operator= (const TriangleMesh& rhs) {
	if (this == &rhs) {
		return (*this);
	}
	
	vertices = rhs.vertices;
	normals = rhs.normals;
	vertexFaces = rhs.vertexFaces;
	textureCoords = rhs.textureCoords;
	
	return *this;
}



