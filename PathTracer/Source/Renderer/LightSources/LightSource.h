#pragma once

#include <Constructs/Vec3f.h>

#include <Renderer/Octree/Octree.h>

class LightSource
{
public:
	virtual ~LightSource() {}

	virtual Vec3f GetHardPoint() = 0;
	virtual Vec3f GetRandomPoint() = 0;
	virtual Vec3f GetAttenuation(const Vec3f &position) = 0;
};

