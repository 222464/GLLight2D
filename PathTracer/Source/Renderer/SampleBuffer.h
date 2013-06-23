#pragma once

#include <Renderer/BufferObjects/FBO.h>

#include <Renderer/TracerScene.h>

#include <Renderer/Polygon2D.h>

#include <Constructs/AABB2D.h>

class SampleBuffer
{
private:
	FBO m_sampleBuffer;

	int m_numFramesUnrefreshed;

	AABB m_aabb;

public:
	TracerScene m_scene;

	float m_sceneHeight;

	unsigned int m_maxNumSamples;

	SampleBuffer();
	~SampleBuffer();

	bool Create(unsigned int width, unsigned int height, const std::string &tracerShaderName);

	void Add(const Polygon2D &polygon);

	void Clear();
	void Clear(const AABB2D &sceneBounds);

	void Compile();
	void Accumulate(const Vec2f &cameraPosition, const Vec2f &windowDimensions, float angle);
	void Refresh();
	void RenderBuffer(const Vec2f &position, const Vec2f &windowDimensions, float angle);
};

