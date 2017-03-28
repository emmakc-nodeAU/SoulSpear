#include "RenderModel.h"
#include "RenderData.h"

std::vector<RenderData*> RenderModel::LoadOBJFromDisk(std::string path)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool loadedSuccess = tinyobj::LoadObj(&attributes, &shapes, &materials,
		&err, path.c_str());
	assert(loadedSuccess && "Could not load the OBJ file - did not get the right path?");

	std::vector<RenderData*> modelRenderData;

	for (auto shape : shapes)
	{
		std::vector<OBJVertex> vertices;
		vertices.reserve(shape.mesh.num_face_vertices.size() * 3);

		int index = 0;
		for (auto& face : shape.mesh.num_face_vertices)
		{
			for (int i = 0; i < 3; ++i)
			{
				tinyobj::index_t myIndex = shape.mesh.indices[index + i];

				OBJVertex vertex;
				// LOCAL SPACE
				vertex.position.x = attributes.vertices[3 * myIndex.vertex_index + 0];
				vertex.position.y = attributes.vertices[3 * myIndex.vertex_index + 1];
				vertex.position.z = attributes.vertices[3 * myIndex.vertex_index + 2];
				vertex.position.w = 1.0f;
				// NORMALS
				if (!attributes.normals.empty())
				{
					vertex.normal.x = attributes.normals[3 * myIndex.normal_index + 0];
					vertex.normal.y = attributes.normals[3 * myIndex.normal_index + 1];
					vertex.normal.z = attributes.normals[3 * myIndex.normal_index + 2];
				}
				// UV
				if (!attributes.texcoords.empty())
				{
					vertex.uv.s = attributes.texcoords[2 * myIndex.texcoord_index + 0];
					vertex.uv.t = attributes.texcoords[2 * myIndex.texcoord_index + 1];
				}
				vertices.push_back(vertex);
			}
			index += face;
		}

		RenderData* renderData = new RenderData();
		renderData->GenerateBuffers(false);
		renderData->Bind();

		glBufferData(
			GL_ARRAY_BUFFER, 
			vertices.size() * sizeof(OBJVertex), 
			vertices.data(),
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(OBJVertex), (char*)offsetof(OBJVertex, position));
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(OBJVertex), (char*)offsetof(OBJVertex, normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(OBJVertex), (char*)offsetof(OBJVertex, uv));

		renderData->setNumberOfIndicies(shape.mesh.num_face_vertices.size() * 3);
		renderData->Unbind();
		modelRenderData.push_back(renderData);
	}
	return modelRenderData;	// std::vector<RenderData*>();
}
