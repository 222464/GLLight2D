#pragma once

#include <Constructs/Vec2f.h>
#include <Constructs/Matrix3x3f.h>

#include <vector>

class Polygon2D
{
private:

public:
	float m_height;

	std::vector<Vec2f> m_points;

	Polygon2D();
	~Polygon2D();

	Polygon2D Transform(Matrix3x3f &transform);
};

