#pragma once

class Point3;
class Point4;
class Ray;

class AABBox {
public:
	AABBox();
	
	AABBox(const float x0, const float y0, const float z0,
		   const float x1, const float y1, const float z1);
	
	AABBox(Point3 const & min, Point3 const & max);
	
	AABBox(Point4 const & min, Point4 const & max);
	
	bool RayHit(Ray const& ray) const;
	
	bool PointInside(Point3 const& point) const;
	
	bool PointInside(Point4 const& point) const;
	
	Point3 GetMin() const;
	
	Point3 GetMax() const;
	
	void Reset();
	
	void Superset(AABBox const & other) {
		x0 = x0 < other.x0 ? x0 : other.x0;
		y0 = y0 < other.y0 ? y0 : other.y0;
		z0 = z0 < other.z0 ? z0 : other.z0;
		
		x1 = x1 > other.x1 ? x1 : other.x1;
		y1 = y1 > other.y1 ? y1 : other.y1;
		z1 = z1 > other.z1 ? z1 : other.z1;
	}

	// min point
	float x0, y0, z0;
	// max point
	float x1, y1, z1;
};

