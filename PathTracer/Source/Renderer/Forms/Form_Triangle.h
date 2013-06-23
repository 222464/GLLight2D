#pragma once

#include <Renderer/Forms/Form.h>

#include <Constructs/Vec2f.h>

class Form_Triangle :
	public Form
{
public:
	unsigned int m_materialIndex;

	Vec3f m_points[3];
	Vec3f m_normals[3];
	Vec2f m_uv[3];

	Form_Triangle();

	// Inherited from Form
	bool RayTest(const Vec3f &start, const Vec3f &dir, float &t, Vec3f &hitPos);
	Vec3f GetBarycentricCoords(const Vec3f &hitPos);
	Vec2f GetImageCoord(const Vec3f &barycentricCoords);
	Vec3f GetNormal(const Vec3f &barycentricCoords);
	unsigned int GetMaterialIndex();
	AABB GetAABB();
	Vec3f GetCenter();
	float GetArea();
};

