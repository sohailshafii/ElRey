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
	

	// min point
	float x0, y0, z0;
	// max point
	float x1, y1, z1;
};
