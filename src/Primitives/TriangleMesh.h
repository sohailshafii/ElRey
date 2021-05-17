#pragma once

#include <vector>

class Point3;
class Vector3;
class Point2;

class TriangleMesh {
public:
	TriangleMesh();
	TriangleMesh(TriangleMesh const& otherMesh);
	TriangleMesh(TriangleMesh && otherMesh);
	TriangleMesh& operator= (const TriangleMesh& rhs);
	
	~TriangleMesh() {
	}
	
	std::vector<Point3> vertices;
	std::vector<Vector3> normals;
	std::vector<Point2> textureCoords;
	// for each vertex, tell us which triangle faces
	// it's adjacent to
	std::vector<std::vector<unsigned int>> vertexFaces;
};
