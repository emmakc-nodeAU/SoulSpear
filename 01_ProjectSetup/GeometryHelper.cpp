#include "GeometryHelper.h"
#include <gl_core_4_4.h>
#include <glm/glm.hpp>

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

					//positions
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

			// 4. SAVE RENDER DATA
			renderData->SetNumberofIndicies(vertices.size());
			dataToReturn.push_back(renderData);


		}
		
		return dataToReturn;
	}
}

/* OUTLINE ~ LOAD AN OBJECT
1. Load OBJ Vertex Data
2. Create Dynamic Array Vector // std::vector<OBJVertex>
- Name:		VERTICES
- Loop of each FACE of OBJ: checks for index value
- FACE index contains:
- 3 position coords (x,y,z)
- 3 normal values (x,y,z)
- 2 texture coords (u,v)
- FACE values pushed into VERTICES array // vertices.push_back(v);
- increment to next face until all faces pushed into VERTICES

3. Generate Render Data: CPU data to send to GPU
4.
*/