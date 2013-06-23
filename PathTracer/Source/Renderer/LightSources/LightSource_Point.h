#pragma once

#include <RayTracer/LightSources/LightSource.h>

class LightSource_Point :
	public LightSource
{
public:
	Vec3f m_position;
	Vec3f m_color;
	Vec3f m_attenuationFactors;

	float m_sourceRadius;

	LightSource_Point();

	// Inherited from LightSource
	Vec3f GetHardPoint();
	Vec3f GetRandomPoint();
	Vec3f GetAttenuation(const Vec3f &position);
};

