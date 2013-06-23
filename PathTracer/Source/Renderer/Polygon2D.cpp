#include <Renderer/Polygon2D.h>

Polygon2D::Polygon2D()
	: m_height(64.0f)
{
}

Polygon2D::~Polygon2D()
{
}

Polygon2D Polygon2D::Transform(Matrix3x3f &transform)
{
	Polygon2D polygon;

	polygon.m_points.resize(m_points.size());

	for(size_t i = 0, numPoints = m_points.size(); i < numPoints; i++)
		polygon.m_points[i] = transform * m_points[i];

	polygon.m_height = m_height;
	
	return polygon;
}