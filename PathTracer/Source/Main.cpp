#include <SFML/Graphics.hpp>

#include <Renderer/TracerScene.h>
#include <Renderer/Model_OBJ.h>

#include <Utilities/UtilFuncs.h>

#include <Renderer/BufferObjects/FBO.h>

#include <Renderer/RenderUtils.h>

#include <Renderer/SampleBuffer.h>

#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "Path Tracing 2D");
	window.setVerticalSyncEnabled(true);

	glewInit();

	glMatrixMode(GL_PROJECTION);
	Matrix4x4f::OrthoMatrix(0.0f, 900.0f, 0.0f, 600.0f, -1.0f, 1.0f).GL_Load();

	SampleBuffer buffer;

	const float sampleScalar = 1.0f;

	const int downsampledWidth = window.getSize().x * sampleScalar;
	const int downsampledHeight = window.getSize().y * sampleScalar;

	if(!buffer.Create(downsampledWidth, downsampledHeight, "NONE NONE tracerShader.frag"))
		abort();

	buffer.Clear(AABB2D(Vec2f(-downsampledWidth, -downsampledHeight), Vec2f(downsampledWidth, downsampledHeight)));

	Light light;
	light.m_position = Vec3f(0.0f, 0.0f, 32.0f);
	light.m_color = Vec3f(10.0f, 10.0f, 10.0f);

	buffer.m_scene.m_lights.push_back(light);

	Polygon2D basePoly;
	basePoly.m_points.resize(4);
	basePoly.m_points[0] = Vec2f(-50.0f, -50.0f) * sampleScalar;
	basePoly.m_points[1] = Vec2f(50.0f, -50.0f) * sampleScalar;
	basePoly.m_points[2] = Vec2f(50.0f, 50.0f) * sampleScalar;
	basePoly.m_points[3] = Vec2f(-50.0f, 50.0f) * sampleScalar;

	buffer.Add(basePoly);

	Polygon2D basePoly2;
	basePoly2.m_points.resize(4);
	basePoly2.m_points[0] = Vec2f(-50.0f, 100.0f) * sampleScalar;
	basePoly2.m_points[1] = Vec2f(50.0f, 100.0f) * sampleScalar;
	basePoly2.m_points[2] = Vec2f(50.0f, 200.0f) * sampleScalar;
	basePoly2.m_points[3] = Vec2f(-50.0f, 200.0f) * sampleScalar;

	buffer.Add(basePoly2);

	buffer.Compile();
	buffer.Refresh();

	bool close = false;

	while(!close)
	{
		sf::Event event;

		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				close = true;

			if(event.type == sf::Event::MouseMoved)
			{
				buffer.Refresh();

				buffer.m_scene.m_lights.back().m_position = Vec3f((event.mouseMove.x - 450) * sampleScalar, (300 - event.mouseMove.y) * sampleScalar, 32.0f);
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			close = true;

		for(size_t i = 0; i < 1; i++)
			buffer.Accumulate(Vec2f(0.0f, 0.0f), Vec2f(downsampledWidth, downsampledHeight), 0.0f);
		
		glFlush();

		glViewport(0, 0, window.getSize().x, window.getSize().y);
		buffer.RenderBuffer(Vec2f(0.0f, 0.0f), Vec2f(900.0f, 600.0f), 0.0f);

		window.display();
	}

	return 0;
}