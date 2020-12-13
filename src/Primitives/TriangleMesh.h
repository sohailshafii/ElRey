#pragma once

#include <vector>

class Point3;
class Vector3;

class Mesh {
public:
	Mesh();
	Mesh(Mesh const& otherMesh);
	Mesh(Mesh && otherMesh);
	Mesh& operator= (const Mesh& rhs);
	
	~Mesh();
	
	void AddVertexFace(unsigned int vertexIndex,
					   unsigned int triangleIndex);
	
	std::vector<Point3> vertices;
	std::vector<Vector3> normals;
	// for each vertex, tell us which triangle faces
	// it's adjacent to
	std::vector<std::vector<unsigned int>> vertexFaces;
};
