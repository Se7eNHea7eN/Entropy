#pragma once
#include "Common/Scene.hpp"
#include "../../Engine/3rdparty/tiny_obj_loader/tiny_obj_loader.h"
#include "Graphic/Vertex.hpp"

namespace Entropy {
	std::shared_ptr<SceneGeometryNode> ParseObj(std::string path) {
		auto objNode = std::shared_ptr<SceneGeometryNode>(std::make_shared<SceneGeometryNode>());

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

		if (!warn.empty()) {
			Log(warn.c_str());
		}

		if (!err.empty()) {
			Log(err.c_str());
		}

		if (!ret) {
			exit(1);
		}


		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {

			auto mesh = std::make_shared<Mesh>();

			std::vector<Vertex>* vertices = new std::vector<Vertex>;
			std::vector<uint32_t>* indices = new std::vector<uint32_t>;
			// uint16_t index = 0;
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					indices->push_back(indices->size());

					Vertex vertex;
					vertex.m_x = attrib.vertices[3 * idx.vertex_index + 0];
					vertex.m_y = attrib.vertices[3 * idx.vertex_index + 1];
					vertex.m_z = attrib.vertices[3 * idx.vertex_index + 2];
					vertex.m_nx = attrib.normals[3 * idx.normal_index + 0];
					vertex.m_ny = attrib.normals[3 * idx.normal_index + 1];
					vertex.m_nz = attrib.normals[3 * idx.normal_index + 2];
					vertices->push_back(vertex);
					// tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					// tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					// Optional: vertex colors
					// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
					// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
					// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
				}
				index_offset += fv;

				// per-face material
				// shapes[s].mesh.material_ids[f];
			}

			mesh->m_vertexBuffer = &(*vertices)[0];
			mesh->m_vertexCount = vertices->size();
			mesh->m_vertexBufferSize = vertices->size() * 4 * 6;

			//
			mesh->m_indexBuffer = &(*indices)[0];
			mesh->m_indexCount = indices->size();
			mesh->m_indexBufferSize = indices->size() * 4;
			objNode->m_Mesh.push_back(mesh);
		}
		return objNode;
	}
}
