#pragma once
//#include "SphereBoundingVolume.h"

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

	void setNumberOfIndicies(unsigned int numIndicies) { m_numberOfIndicies = numIndicies; }
	unsigned int getNumberOfIndicies() const { return m_numberOfIndicies; }

	// CAMERA - Fulstrum Culling
	//const SphereBoundingVolume& GetBounds() const { return m_bounds; }
	//const SphereBoundingVolume& GetBounds() { return m_bounds; }
	//SphereBoundingVolume m_bounds; // Mesh bounded by a sphere
protected:
private:
	unsigned int	m_VAO;
	unsigned int	m_VBO;
	unsigned int	m_IBO;
	unsigned int	m_numberOfIndicies;
	bool			m_hasIndexBuffer;
};
