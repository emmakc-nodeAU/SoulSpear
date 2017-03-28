#include "GeometryHelper.h"
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "RenderData.h"

namespace GeometryHelper
{
	std::vector<RenderData*> loadObjFromFile(std::string filepath)
	{
		// 1. Load Object
		std::string path = filepath;
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		bool success = tinyobj::LoadObj(&attribs, &shapes, &materials,
			&err, filepath.c_str());

		std::vector<RenderData*> dataToReturn;
		for (auto& shape : shapes)
		{
			// 2. Create a Vector Dynamic Array
			std::vector<OBJVertex> vertices;
			int index = 0;
			for (auto face : shape.mesh.num_face_vertices)
			{
				for (int i = 0; i < 3; ++i)
				{
					tinyobj::index_t idx = shape.mesh.indices[index + i];
					OBJVertex v = { 0 };
					// positions
					v.x = attribs.vertices[3 * idx.vertex_index + 0];
					v.y = attribs.vertices[3 * idx.vertex_index + 1];
					v.z = attribs.vertices[3 * idx.vertex_index + 2];
					// normals
					if (attribs.normals.size() > 0)
					{
						v.nx = attribs.normals[3 * idx.normal_index + 0];
						v.ny = attribs.normals[3 * idx.normal_index + 1];
						v.nz = attribs.normals[3 * idx.normal_index + 2];
					}
					// texture coordinates
					if (attribs.texcoords.size() > 0)
					{
						v.u = attribs.texcoords[2 * idx.texcoord_index + 0];
						v.v = attribs.texcoords[2 * idx.texcoord_index + 1];
					}
					vertices.push_back(v);
				}
				index += face;
			}
			// 3. Generate Render Object
			RenderData* renderData = new RenderData;
			renderData->GenerateBuffers(false);
			renderData->Bind();

			// PUSH DATA CPU to GPU
			glBufferData(GL_ARRAY_BUFFER, vertices.size() *
				sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW);

			// Specify a CPU slot for Vertex Data
			glEnableVertexAttribArray(0);	// Position (Slot, No. Vars, Data Type, Normalise?, Buffer Size, Locate at Index = DataType size(Float4) * No.Vars)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(OBJVertex), 0);

			glEnableVertexAttribArray(1);	// Normal Data
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,
				sizeof(OBJVertex), (void*)12);

			glEnableVertexAttribArray(2);	// Texture Data
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(OBJVertex), (void*)24);

			// 4. Save Render Data
			renderData->setNumberOfIndicies(vertices.size());
			dataToReturn.push_back(renderData);
		}
		return dataToReturn;
	}
}
	
	//RenderData * GeometryHelper::CreateGrid(
	//	unsigned int rows, 
	//	unsigned int cols, 
	//	float width, 
	//	float height, 
	//	glm::vec4 colour)
	//{
		//// VERTICES ARRAY --------------------------------------------------
		//simpleVertex* vertices = new simpleVertex[rows * cols];
		//// BUILD LOOP ---------------------------------------
		//float rowSpacing = width / (cols - 1);
		//float colSpacing = height / (rows - 1);
		//for (unsigned int r = 0; r < rows; ++r)
		//{
		//	for (unsigned int c = 0; c < cols; ++c)
		//	{
		//		simpleVertex& vert = vertices[r * cols + c];
		//		vert.position = glm::vec4(
		//			-(width / 2) + (colSpacing * c), 0.0f,
		//			-(height / 2) + (rowSpacing * r), 1.0f);
		//		vert.colour = colour;
		//	}
		//}	
		//// INDICES ARRAY --------------------------------------------------
		//unsigned int* indices = new unsigned[(rows - 1) * (cols - 1) * 6];
		//unsigned int index = 0;
		//for (unsigned int r = 0; r < (rows - 1); ++r)
		//{
		//	for (unsigned int c = 0; c < (cols - 1); ++c)
		//	{
		//		// Triangle
		//		indices[index++] = r * cols + c;
		//		indices[index++] = (r + 1) * cols + c;
		//		indices[index++] = (r + 1) * cols + (c + 1);

		//		// Triangle 2
		//		indices[index++] = r * cols + c;
		//		indices[index++] = (r + 1) * cols + (c + 1);
		//		indices[index++] = r * cols + (c + 1);
		//	}
		//}
		//// Index Counter
		//unsigned int numberOfIndices = (rows - 1) * (cols - 1) * 6;

		//// MULTI SHADER ----------------------------------------------------------
		//RenderData* renderData = new RenderData();
		//// CREATE
		//renderData->GenerateBuffers();
		//renderData->Bind();
		//glBufferData(GL_ARRAY_BUFFER,
		//	(rows * cols) * sizeof(simpleVertex), vertices, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		//	numberOfIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(simpleVertex), (char*)offsetof(simpleVertex, position));

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(simpleVertex), (char*)offsetof(simpleVertex, colour));

		//renderData->setNumberOfIndicies(numberOfIndices);
		//renderData->Unbind();

		//delete[] vertices;
		//delete[] indices;
		//return nullptr;
	//}

	//RenderData * GeometryHelper::CreateQuad(float width, float height, glm::vec3 facingDirection)
	//{
	//	return nullptr;
	//}

