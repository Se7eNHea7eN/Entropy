#pragma once
#include "bgfx/bgfx.h"
#include "Common/Scene.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/StaticMeshComponent.hpp"
#include <map>
namespace Entropy {
	struct BgfxMaterial {
		std::shared_ptr<Material> mat;
		bgfx::ProgramHandle m_program;

		bgfx::UniformHandle u_cameraPos;

		bgfx::UniformHandle u_pointLightCount;
		bgfx::UniformHandle u_lightPosition;
		bgfx::UniformHandle u_lightColor;

		std::map<std::string, bgfx::UniformHandle> uniformMap;
		std::map<std::string, bgfx::TextureHandle> textureMap;
		BgfxMaterial() {
			u_cameraPos = bgfx::createUniform("u_cameraPos", bgfx::UniformType::Vec4);

			u_pointLightCount = bgfx::createUniform("u_pointLightCount", bgfx::UniformType::Vec4);
			u_lightPosition = bgfx::createUniform("u_lightPosition", bgfx::UniformType::Vec4,10);
			u_lightColor = bgfx::createUniform("u_lightColor", bgfx::UniformType::Vec4, 10);
		}
		~BgfxMaterial() {
			bgfx::destroy(m_program);
		}

		void Submit() {
			int index = 0;
			for (auto pair : uniformMap) {
				auto name = pair.first;
				auto uniform = pair.second;
				bgfx::setTexture(index++, uniform, textureMap[name]);
			}
			// bgfx::setTexture(0, iterator->get()->material->s_albedo, iterator->get()->material->t_albedo);
			// bgfx::setTexture(1, iterator->get()->material->s_metallic, iterator->get()->material->t_metallic);
			// bgfx::setTexture(2, iterator->get()->material->s_roughness, iterator->get()->material->t_roughness);
			// bgfx::setTexture(3, iterator->get()->material->s_ao, iterator->get()->material->t_ao);
			// bgfx::setTexture(4, iterator->get()->material->s_normal, iterator->get()->material->t_normal);
		}

		static BgfxMaterial* buildFromMaterial(Entropy::Material* mat);
	};

	BgfxMaterial* BgfxMaterial::buildFromMaterial(Entropy::Material* mat) {
		auto bgfxMat = new BgfxMaterial();
		bgfxMat->m_program = loadProgram(mat->VertexShader().c_str(), mat->FragmentShader().c_str());
		for (auto pair : mat->GetParams()) {
			auto name = pair.first;
			auto mat = pair.second;
			switch (mat.type) {
			case Sampler:
				auto uniform = bgfx::createUniform(name.c_str(), bgfx::UniformType::Sampler);
				auto texture = static_cast<const Texture*>(mat.Value);
				auto textureHandler = createTexture(texture->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);

				bgfxMat->uniformMap[name] = uniform;
				bgfxMat->textureMap[name] = textureHandler;
				break;
				// case Vector4:
				// 	break;
				// case Vector3:
				// 	break;
				// case Float:
				// 	break;
			}
		}
		return bgfxMat;
	}
	
	struct BgfxGeometry {
		std::shared_ptr<StaticMeshComponent> geometry;
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
