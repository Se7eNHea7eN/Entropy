#pragma once
#include "bgfx/bgfx.h"
#include "Common/Scene.hpp"

namespace Entropy {
	struct BgfxMaterial {
		std::shared_ptr<Entropy::Material> mat;
		bgfx::ProgramHandle m_program;
		bgfx::UniformHandle s_albedo;
		bgfx::UniformHandle s_metallic;
		bgfx::UniformHandle s_roughness;
		bgfx::UniformHandle s_ao;
		bgfx::UniformHandle u_cameraPos;
		BgfxMaterial() {
			s_albedo = bgfx::createUniform("s_albedo", bgfx::UniformType::Sampler);
			s_metallic = bgfx::createUniform("s_metallic", bgfx::UniformType::Sampler);
			s_roughness = bgfx::createUniform("s_roughness", bgfx::UniformType::Sampler);
			s_ao = bgfx::createUniform("s_ao", bgfx::UniformType::Sampler);
			u_cameraPos = bgfx::createUniform("u_cameraPos", bgfx::UniformType::Vec4);
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
