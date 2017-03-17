#include "RenderData.h"
#include <gl_core_4_4.h>
#include <assert.h>

RenderData::RenderData()
	: m_VAO(-1)
	, m_VBO(-1)
	, m_IBO(-1)
	, m_numberofIndices(0)
	, m_hasIndexBuffer(true)
{	

}

RenderData::~RenderData()
{
	if(m_IBO != -1)
		glDeleteBuffers(1, &m_IBO);
	if(m_VBO != -1)
		glDeleteBuffers(1, &m_VBO);
	if(m_VAO != -1)
		glDeleteVertexArrays(1, &m_VAO);
}


RenderData::RenderData(RenderData&& other)
{
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_IBO = other.m_IBO;
	m_numberofIndices = other.m_numberofIndices;
	m_hasIndexBuffer = other.m_hasIndexBuffer;

	other.m_VAO = -1;
	other.m_VBO = -1;
	other.m_IBO = -1;

}



void RenderData::GenerateBuffers(bool generateIndexBuffer /* = true */)
{
	assert(m_VAO == -1 && "Buffers have already been created for this object!");
	assert(m_VBO == -1);
	assert(m_IBO == -1);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	if(generateIndexBuffer) glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if(generateIndexBuffer) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBindVertexArray(0);

	m_hasIndexBuffer = generateIndexBuffer;
}

void RenderData::Bind() const
{
	assert(m_VAO != -1 && "You are binding a invalid array!");
	glBindVertexArray(m_VAO);
}

void RenderData::Unbind() const
{
	glBindVertexArray(0);
}

void RenderData::Render() const
{
	Bind();
	if (m_hasIndexBuffer)
	{
		glDrawElements(GL_TRIANGLES, m_numberofIndices,
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_numberofIndices);
	}
	Unbind();
}
