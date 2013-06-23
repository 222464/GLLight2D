#include <Renderer/TracerScene.h>

#include <Renderer/RenderUtils.h>

#include <Utilities/UtilFuncs.h>

#include <assert.h>

#include <sstream>

TracerScene::TracerScene()
	: m_treeBuffer(0), m_triBuffer(0), m_materialBuffer(0)
{
	m_tree.Create(AABB(Vec3f(-1000.0f, -1000.0f, -1.0f), Vec3f(1000.0f, 1000.0f, 64.0f)));
}

TracerScene::~TracerScene()
{
	if(m_treeBuffer != 0)
	{
		glDeleteBuffers(1, &m_treeBuffer);
		glDeleteBuffers(1, &m_triBuffer);
		glDeleteBuffers(1, &m_materialBuffer);

		glDeleteTextures(1, &m_treeTexture);
		glDeleteTextures(1, &m_triTexture);
		glDeleteTextures(1, &m_materialTexture);
	}
}

bool TracerScene::Create(const std::string &shaderName)
{
	return m_traceShader.CreateAsset(shaderName);
}

void TracerScene::GenerateBufferData(
		std::vector<BVHNodeBufferData> &treeBuffer,
		std::vector<TriBufferData> &triBuffer,
		std::vector<MaterialBufferData> &materialBuffer)
{
	if(m_treeBuffer != 0)
	{
		glDeleteBuffers(1, &m_treeBuffer);
		glDeleteBuffers(1, &m_triBuffer);
		glDeleteBuffers(1, &m_materialBuffer);

		glDeleteTextures(1, &m_treeTexture);
		glDeleteTextures(1, &m_triTexture);
		glDeleteTextures(1, &m_materialTexture);
	}

	glGenBuffers(1, &m_treeBuffer);
	glGenBuffers(1, &m_triBuffer);
	glGenBuffers(1, &m_materialBuffer);
	glGenTextures(1, &m_treeTexture);
	glGenTextures(1, &m_triTexture);
	glGenTextures(1, &m_materialTexture);

	glBindBuffer(GL_TEXTURE_BUFFER, m_treeBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(BVHNodeBufferData) * treeBuffer.size(), &treeBuffer[0], GL_STATIC_DRAW);
	glBindTexture(GL_TEXTURE_BUFFER, m_treeTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_treeBuffer);

	glBindBuffer(GL_TEXTURE_BUFFER, m_triBuffer);	
	glBufferData(GL_TEXTURE_BUFFER, sizeof(TriBufferData) * triBuffer.size(), &triBuffer[0], GL_STATIC_DRAW);
	glBindTexture(GL_TEXTURE_BUFFER, m_triTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_triBuffer);

	glBindBuffer(GL_TEXTURE_BUFFER, m_materialBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(MaterialBufferData) * materialBuffer.size(), &materialBuffer[0], GL_STATIC_DRAW);
	glBindTexture(GL_TEXTURE_BUFFER, m_materialTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_materialBuffer);

	glBindBuffer(GL_TEXTURE_BUFFER, 0);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

void TracerScene::GenerateSceneData()
{
	std::vector<BVHNodeBufferData> treeBuffer;
	std::vector<TriBufferData> triBuffer;
	std::vector<MaterialBufferData> materialBuffer;
	
	m_tree.CompileBuffers(treeBuffer, triBuffer);
	
	// Create material buffer
	materialBuffer.reserve(m_materials.size());

	for(size_t i = 0, numMaterials = m_materials.size(); i < numMaterials; i++)
	{
		MaterialBufferData data;

		data.m_diffuseColor = m_materials[i].m_diffuseColor;
		data.m_specularColor = m_materials[i].m_specularColor;
		data.m_shininess = m_materials[i].m_shininess;

		materialBuffer.push_back(data);
	}

	GenerateBufferData(treeBuffer, triBuffer, materialBuffer);
}

void TracerScene::Render(const Vec2f &position, const Vec2f &dims, float angle)
{
	assert(m_traceShader.Created());

	m_traceShader.Bind();

	m_traceShader.SetShaderTexture("treeBuffer", m_treeTexture, GL_TEXTURE_BUFFER);
	m_traceShader.SetShaderTexture("triBuffer", m_triTexture, GL_TEXTURE_BUFFER);
	m_traceShader.SetShaderTexture("materialBuffer", m_materialTexture, GL_TEXTURE_BUFFER);
	m_traceShader.BindShaderTextures();

	m_traceShader.SetUniformf("randSeed", Randf() * 531.0f);

	m_traceShader.SetUniformv2f("cameraDims", dims);
	m_traceShader.SetUniformv3f("cameraDir", Vec3f(0.0f, 0.0f, -1.0f));
	m_traceShader.SetUniformv3f("cameraPosition", Vec3f(position.x, position.y, 999.0f));
	m_traceShader.SetUniformv4f("cameraRotation", Quaternion(angle, Vec3f(0.0f, 0.0f, 1.0f)).GetVec4f());

	for(size_t i = 0, numLights = m_lights.size(); i < numLights; i++)
	{
		std::ostringstream os;

		os << "lightPositions[" << i << "]";
		m_traceShader.SetUniformv3f(os.str(), m_lights[i].m_position);

		os.str("");

		os << "lightColors[" << i << "]";
		m_traceShader.SetUniformv3f(os.str(), m_lights[i].m_color);
	}

	m_traceShader.SetUniformi("numLights", static_cast<int>(m_lights.size()));

	Matrix4x4f projection = Matrix4x4f::GL_Get_Projection();
	Matrix4x4f modelView = Matrix4x4f::GL_Get_Modelview();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawNormalizedQuad();

	glMatrixMode(GL_PROJECTION);
	projection.GL_Load();
	glMatrixMode(GL_MODELVIEW);
	modelView.GL_Load();

	Shader::Unbind();
}