#pragma once
class PostProcessing
{
public:

	bool startup();
	unsigned int m_fbo;
	unsigned int m_fboTexture;
	unsigned int m_fboDepth;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int ibo;

};