#pragma once

class RenderData
{
public:
	RenderData();
	~RenderData();
	
	RenderData(const RenderData& other) = delete;
	RenderData(RenderData&& other);

	RenderData& operator=(const RenderData& other) = delete;

	void GenerateBuffers(bool generateIndexBuffer = true);

	void Bind() const;
	void Unbind() const;

	void Render() const;

	unsigned int& GetVAO() { return m_VAO; }
	unsigned int& GetVBO() { return m_VBO; }
	unsigned int& GetIBO() { return m_IBO; }

	unsigned int GetVAO() const { return m_VAO; }
	unsigned int GetVBO() const { return m_VBO; }
	unsigned int GetIBO() const { return m_IBO; }

	void SetNumberofIndicies(unsigned int numIndicies) {
		m_numberofIndices = numIndicies;
	};

	unsigned int GetNumberOfIndicies() const { return m_numberofIndices; }

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_numberofIndices;

	bool		 m_hasIndexBuffer;
};