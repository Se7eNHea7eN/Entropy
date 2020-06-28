#include "Parser/ObjParser.hpp"
#include "../../../ThirdParty/bgfx/include/bgfx/bgfx.h"
#include <Utils\Debug.hpp>
#include "Common/Scene.hpp"
#include "../../3rdparty/tiny_obj_loader/tiny_obj_loader.h"
#include "Graphic/Vertex.hpp"
#include "Graphic/StaticMeshComponent.hpp"
using namespace Entropy;
std::vector<std::shared_ptr<Mesh>>* Entropy::ParseObj(std::string path) {
	std::vector<std::shared_ptr<Mesh>>* meshes = new std::vector<std::shared_ptr<Mesh>>;

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
		mesh->vertices.resize(shapes[s].mesh.num_face_vertices.size() * 3);

		// std::vector<uint32_t>* indices = new std::vector<uint32_t>(shapes[s].mesh.num_face_vertices.size() * 3);

		int index = 0;
		// uint16_t index = 0;
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv != 3) {
				Log("fv = %d", fv);
			}
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				// indices->push_back(indices->size());
				// indices->at(index) = index;
				Vertex vertex;
				vertex.x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.z = attrib.vertices[3 * idx.vertex_index + 2];
				vertex.normalX = attrib.normals[3 * idx.normal_index + 0];
				vertex.normalY = attrib.normals[3 * idx.normal_index + 1];
				vertex.normalZ = attrib.normals[3 * idx.normal_index + 2];
				if (idx.texcoord_index >= 0) {
					vertex.texCoordX = attrib.texcoords[2 * idx.texcoord_index + 0];
					vertex.texCoordY = attrib.texcoords[2 * idx.texcoord_index + 1];
				}
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
				//
				mesh->vertices.at(index) = vertex;
				// vertices->push_back(vertex);

				index++;
			}
			index_offset += fv;

			// per-face material
			// shapes[s].mesh.material_ids[f];
		}

		mesh->CalculateTangents();

		meshes->push_back(mesh);
	}
	return meshes;
}
