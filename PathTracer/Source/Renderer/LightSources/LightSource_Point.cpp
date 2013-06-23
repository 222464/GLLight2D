#include <RayTracer/LightSources/LightSource_Point.h>

#include <Utilities/UtilFuncs.h>

LightSource_Point::LightSource_Point()
	: m_position(0.0f, 0.0f, 0.0f),
	m_color(2.0f, 2.0f, 2.0f),
	m_attenuationFactors(0.5f, 0.05f, 0.001f),
	m_sourceRadius(0.5f)
{
}

Vec3f LightSource_Point::GetHardPoint()
{
	return m_position;
}

Vec3f LightSource_Point::GetRandomPoint()
{
	Vec3f rand(Randf(-1.0f, 1.0f), Randf(-1.0f, 1.0f), Randf(-1.0f, 1.0f));

	return m_position + rand.Normalize() * m_sourceRadius;
}

Vec3f LightSource_Point::GetAttenuation(const Vec3f &position)
{
	float distance = (m_position - position).Magnitude();

	return m_color / (m_attenuationFactors.x + distance * m_attenuationFactors.y + distance * distance * m_attenuationFactors.z);
}