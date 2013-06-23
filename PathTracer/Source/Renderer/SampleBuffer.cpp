#include <Renderer/SampleBuffer.h>

#include <Renderer/RenderUtils.h>

#include <Utilities/UtilFuncs.h>

#include <assert.h>

SampleBuffer::SampleBuffer()
	: m_maxNumSamples(80), m_numFramesUnrefreshed(0), m_sceneHeight(64.0f)
{
}

SampleBuffer::~SampleBuffer()
{
}

bool SampleBuffer::Create(unsigned int width, unsigned int height, const std::string &tracerShaderName)
{
	m_sampleBuffer.Create(width, height, false, GL_RGB32F, GL_RGB, GL_FLOAT);

	if(!m_scene.Create(tracerShaderName))
		return false;

	Material defaultMaterial;

	defaultMaterial.m_diffuseColor = Vec3f(1.0f, 1.0f, 1.0f);
	defaultMaterial.m_specularColor = 0.5f;
	defaultMaterial.m_shininess = 0.0f;

	m_scene.m_materials.push_back(defaultMaterial);

	return true;
}

void SampleBuffer::Add(const Polygon2D &polygon)
{
	// Create 3D geometry for polygon
	for(int i = 0, numPoints = static_cast<int>(polygon.m_points.size()); i < numPoints; i++)
	{
		int nextPointIndex = Wrap(i + 1, numPoints);

		Vec2f dPoints = polygon.m_points[nextPointIndex] - polygon.m_points[i];
		Vec2f normal2D = Vec2f(dPoints.y, -dPoints.x).Normalize();
		Vec3f normal3D = Vec3f(normal2D.x, normal2D.y, 0.0f);

		Form_Triangle tri1;
		tri1.m_points[0] = Vec3f(polygon.m_points[i].x, polygon.m_points[i].y, 0.0f);
		tri1.m_points[1] = Vec3f(polygon.m_points[nextPointIndex].x, polygon.m_points[nextPointIndex].y, 0.0f);
		tri1.m_points[2] = tri1.m_points[1] + Vec3f(0.0f, 0.0f, polygon.m_height);

		for(int j = 0; j < 3; j++)
			tri1.m_normals[j] = normal3D;

		tri1.m_materialIndex = 0;

		Form_Triangle tri2;
		tri2.m_points[0] = tri1.m_points[0];
		tri2.m_points[1] = tri1.m_points[2];
		tri2.m_points[2] = tri1.m_points[0] + Vec3f(0.0f, 0.0f, polygon.m_height);

		for(int j = 0; j < 3; j++)
			tri2.m_normals[j] = normal3D;

		tri2.m_materialIndex = 0;

		m_scene.m_tree.Add(tri1);
		m_scene.m_tree.Add(tri2);
	}
}

void SampleBuffer::Clear()
{
	m_scene.m_tree.Create(m_aabb);

	// Add background
	Form_Triangle tri1;
	tri1.m_points[0] = Vec3f(m_aabb.m_lowerBound.x, m_aabb.m_lowerBound.y, 0.0f);
	tri1.m_points[1] = Vec3f(m_aabb.m_upperBound.x, m_aabb.m_lowerBound.y, 0.0f);
	tri1.m_points[2] = Vec3f(m_aabb.m_upperBound.x, m_aabb.m_upperBound.y, 0.0f);

	for(int j = 0; j < 3; j++)
		tri1.m_normals[j] = Vec3f(0.0f, 0.0f, 1.0f);

	tri1.m_materialIndex = 0;

	Form_Triangle tri2;
	tri2.m_points[0] = Vec3f(m_aabb.m_lowerBound.x, m_aabb.m_lowerBound.y, 0.0f);
	tri2.m_points[1] = Vec3f(m_aabb.m_upperBound.x, m_aabb.m_upperBound.y, 0.0f);
	tri2.m_points[2] = Vec3f(m_aabb.m_lowerBound.x, m_aabb.m_upperBound.y, 0.0f);

	for(int j = 0; j < 3; j++)
		tri2.m_normals[j] = Vec3f(0.0f, 0.0f, 1.0f);

	tri2.m_materialIndex = 0;

	m_scene.m_tree.Add(tri1);
	m_scene.m_tree.Add(tri2);
}

void SampleBuffer::Clear(const AABB2D &sceneBounds)
{
	m_aabb.m_lowerBound = Vec3f(sceneBounds.m_lowerBound.x, sceneBounds.m_lowerBound.y, -1.0f);
	m_aabb.m_upperBound = Vec3f(sceneBounds.m_upperBound.x, sceneBounds.m_upperBound.y, m_sceneHeight);
	m_aabb.CalculateHalfDims();
	m_aabb.CalculateCenter();
	m_scene.m_tree.Create(m_aabb);

	// Add background
	Form_Triangle tri1;
	tri1.m_points[0] = Vec3f(m_aabb.m_lowerBound.x, m_aabb.m_lowerBound.y, 0.0f);
	tri1.m_points[1] = Vec3f(m_aabb.m_upperBound.x, m_aabb.m_lowerBound.y, 0.0f);
	tri1.m_points[2] = Vec3f(m_aabb.m_upperBound.x, m_aabb.m_upperBound.y, 0.0f);

	for(int j = 0; j < 3; j++)
		tri1.m_normals[j] = Vec3f(0.0f, 0.0f, 1.0f);

	tri1.m_materialIndex = 0;

	Form_Triangle tri2;
	tri2.m_points[0] = Vec3f(m_aabb.m_lowerBound.x, m_aabb.m_lowerBound.y, 0.0f);
	tri2.m_points[1] = Vec3f(m_aabb.m_upperBound.x, m_aabb.m_upperBound.y, 0.0f);
	tri2.m_points[2] = Vec3f(m_aabb.m_lowerBound.x, m_aabb.m_upperBound.y, 0.0f);

	for(int j = 0; j < 3; j++)
		tri2.m_normals[j] = Vec3f(0.0f, 0.0f, 1.0f);

	tri2.m_materialIndex = 0;

	m_scene.m_tree.Add(tri1);
	m_scene.m_tree.Add(tri2);
}

void SampleBuffer::Compile()
{
	m_scene.m_tree.Compile();
	m_scene.GenerateSceneData();
}

void SampleBuffer::Accumulate(const Vec2f &cameraPosition, const Vec2f &windowDimensions, float angle)
{
	m_sampleBuffer.Bind();
	m_sampleBuffer.SetViewport();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	m_scene.Render(cameraPosition, windowDimensions, angle);

	m_sampleBuffer.Unbind();

	glDisable(GL_BLEND);

	m_numFramesUnrefreshed++;
}

void SampleBuffer::Refresh()
{
	m_sampleBuffer.Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	FBO::Unbind();

	m_numFramesUnrefreshed = 0;
}

void SampleBuffer::RenderBuffer(const Vec2f &position, const Vec2f &windowDimensions, float angle)
{
	Matrix4x4f projection = Matrix4x4f::GL_Get_Projection();
	Matrix4x4f modelView = Matrix4x4f::GL_Get_Modelview();

	(Matrix4x4f::TranslateMatrix(Vec3f(position.x, position.y, 0.0f)) * Matrix4x4f::RotateMatrix_Z(angle)).GL_Load();

	assert(m_numFramesUnrefreshed > 0);

	float mult = 1.0f / (m_numFramesUnrefreshed);
	glColor3f(mult, mult, mult);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_sampleBuffer.GetTextureID());

	DrawQuadOriginBottomLeft(windowDimensions.x, windowDimensions.y);

	glColor3f(1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	projection.GL_Load();
	glMatrixMode(GL_MODELVIEW);
	modelView.GL_Load();
}