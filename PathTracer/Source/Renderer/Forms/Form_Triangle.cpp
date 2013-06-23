#include <Renderer/Forms/Form_Triangle.h>

#include <Utilities/UtilFuncs.h>

#include <iostream>

Form_Triangle::Form_Triangle()
	: m_materialIndex(0)
{
}

bool Form_Triangle::RayTest(const Vec3f &start, const Vec3f &dir, float &t, Vec3f &hitPos)
{
	Vec3f e1 = m_points[1] - m_points[0];
	Vec3f e2 = m_points[2] - m_points[0];

	Vec3f h = dir.Cross(e2);
	float a = e1.Dot(h);

	if(abs(a) < 0.00001f)
		return false;

	float f = 1.0f / a;

	Vec3f s = start - m_points[0];

	float u = f * s.Dot(h);

	if(u < 0.0f || u > 1.0f)
		return false;

	Vec3f q = s.Cross(e1);

	float v = f * dir.Dot(q);

	if(v < 0.0f || u + v > 1.0f)
		return false;

	t = f * e2.Dot(q);

	if(t > 0.00001f)
	{
		hitPos = start + dir * t;
		
		return true;
	}

	return false;
}

Vec3f Form_Triangle::GetBarycentricCoords(const Vec3f &hitPos)
{
	// Find barycentric coordinates (area-weighted coordinates of hitPos)
	Vec3f f0 = m_points[0] - hitPos;
	Vec3f f1 = m_points[1] - hitPos;
	Vec3f f2 = m_points[2] - hitPos;

	Vec3f vecArea = (m_points[0] - m_points[1]).Cross(m_points[0] - m_points[2]);
	Vec3f vecArea0 = f1.Cross(f2);
	Vec3f vecArea1 = f2.Cross(f0);
	Vec3f vecArea2 = f0.Cross(f1);

	float areaInv = 1.0f / vecArea.Magnitude();
	float area0 = vecArea0.Magnitude() * areaInv * (vecArea.Dot(vecArea0) > 0.0f ? 1.0f : -1.0f);
	float area1 = vecArea1.Magnitude() * areaInv * (vecArea.Dot(vecArea1) > 0.0f ? 1.0f : -1.0f);
	float area2 = vecArea2.Magnitude() * areaInv * (vecArea.Dot(vecArea2) > 0.0f ? 1.0f : -1.0f);

	return Vec3f(area0, area1, area2);
}

Vec2f Form_Triangle::GetImageCoord(const Vec3f &barycentricCoords)
{
	return m_uv[0] * barycentricCoords.x + m_uv[1] * barycentricCoords.y + m_uv[2] * barycentricCoords.z;
}

Vec3f Form_Triangle::GetNormal(const Vec3f &barycentricCoords)
{
	return m_normals[0] * barycentricCoords.x + m_normals[1] * barycentricCoords.y + m_normals[2] * barycentricCoords.z;
}

unsigned int Form_Triangle::GetMaterialIndex()
{
	return m_materialIndex;
}

AABB Form_Triangle::GetAABB()
{
	AABB aabb;

	aabb.m_lowerBound = aabb.m_upperBound = m_points[0];

	if(m_points[1].x < aabb.m_lowerBound.x)
		aabb.m_lowerBound.x = m_points[1].x;
	if(m_points[1].y < aabb.m_lowerBound.y)
		aabb.m_lowerBound.y = m_points[1].y;
	if(m_points[1].z < aabb.m_lowerBound.z)
		aabb.m_lowerBound.z = m_points[1].z;
	if(m_points[2].x < aabb.m_lowerBound.x)
		aabb.m_lowerBound.x = m_points[2].x;
	if(m_points[2].y < aabb.m_lowerBound.y)
		aabb.m_lowerBound.y = m_points[2].y;
	if(m_points[2].z < aabb.m_lowerBound.z)
		aabb.m_lowerBound.z = m_points[2].z;

	if(m_points[1].x > aabb.m_upperBound.x)
		aabb.m_upperBound.x = m_points[1].x;
	if(m_points[1].y > aabb.m_upperBound.y)
		aabb.m_upperBound.y = m_points[1].y;
	if(m_points[1].z > aabb.m_upperBound.z)
		aabb.m_upperBound.z = m_points[1].z;
	if(m_points[2].x > aabb.m_upperBound.x)
		aabb.m_upperBound.x = m_points[2].x;
	if(m_points[2].y > aabb.m_upperBound.y)
		aabb.m_upperBound.y = m_points[2].y;
	if(m_points[2].z > aabb.m_upperBound.z)
		aabb.m_upperBound.z = m_points[2].z;

	aabb.CalculateHalfDims();
	aabb.CalculateCenter();

	return aabb;
}

Vec3f Form_Triangle::GetCenter()
{
	return (m_points[0] + m_points[1] + m_points[2]) * 0.33333f;
}

float Form_Triangle::GetArea()
{
	float a = (m_points[0] - m_points[1]).Magnitude();
	float b = (m_points[0] - m_points[2]).Magnitude();
	float c = (m_points[1] - m_points[2]).Magnitude();

	float s = (a + b + c) * 0.5f;

	return sqrtf(s * (s - a) * (s - b) * (s - c));
}