#pragma once
#include "bgfx/bgfx.h"
#include "Common/Scene.hpp"

namespace Entropy {
	struct BgfxMaterial {
		std::shared_ptr<Entropy::Material> mat;
		bgfx::ProgramHandle m_program;
		bgfx::UniformHandle u_albedo;
		bgfx::UniformHandle u_params;
		bgfx::UniformHandle u_cameraPos;
		BgfxMaterial() {
			u_albedo = bgfx::createUniform("u_albedo", bgfx::UniformType::Vec4);
			u_params = bgfx::createUniform("u_params", bgfx::UniformType::Vec4);
		}
		~BgfxMaterial() {
			bgfx::destroy(m_program);
		}
	};
	struct BgfxGeometry {
		std::shared_ptr<Entropy::SceneGeometryNode> geometry;
		std::shared_ptr<BgfxMaterial> material;
		bgfx::VertexBufferHandle vbh;
		bgfx::IndexBufferHandle ibh;
		~BgfxGeometry() {
			bgfx::destroy(vbh);
			bgfx::destroy(ibh);
			geometry.reset();
			material.reset();
		}
	};
}
