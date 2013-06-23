#pragma once

#include <Constructs/Vec3f.h>
#include <Constructs/Vec2f.h>
#include <Constructs/AABB.h>
#include <Renderer/Material.h>

class Form
{
public:
	virtual ~Form() {};

	virtual bool RayTest(const Vec3f &start, const Vec3f &dir, float &t, Vec3f &hitPos) = 0;
	virtual Vec3f GetBarycentricCoords(const Vec3f &hitPos) = 0;
	virtual Vec2f GetImageCoord(const Vec3f &barycentricCoords) = 0;
	virtual Vec3f GetNormal(const Vec3f &barycentricCoords) = 0;
	virtual AABB GetAABB() = 0;
	virtual Vec3f GetCenter() = 0;
	virtual float GetArea() = 0;
	virtual unsigned int GetMaterialIndex() = 0;
};

