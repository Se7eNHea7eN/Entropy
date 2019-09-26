#pragma once
#include "bgfx/bgfx.h"
#include "Common/Scene.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/StaticMeshComponent.hpp"
#include <map>
namespace Entropy {

	struct BgfxUniform {
		bgfx::UniformHandle uniformHandle;
		bgfx::UniformType::Enum uniformType;
		bgfx::TextureHandle textureHandle;
		int textureIndex;
	};
	
	struct BgfxMaterial {
		std::shared_ptr<Material> mat;
		bgfx::ProgramHandle m_program;

		bgfx::UniformHandle u_cameraPos;

		bgfx::UniformHandle u_pointLightCount;
		bgfx::UniformHandle u_lightPosition;
		bgfx::UniformHandle u_lightColor;

		std::map<std::string,BgfxUniform> uniformMap;
		BgfxMaterial() {
			u_cameraPos = bgfx::createUniform("u_cameraPos", bgfx::UniformType::Vec4);

			u_pointLightCount = bgfx::createUniform("u_pointLightCount", bgfx::UniformType::Vec4);
			u_lightPosition = bgfx::createUniform("u_lightPosition", bgfx::UniformType::Vec4,10);
			u_lightColor = bgfx::createUniform("u_lightColor", bgfx::UniformType::Vec4, 10);
		}
		~BgfxMaterial() {
			bgfx::destroy(m_program);
		}

		void Submit(Scene* scene) {
			for (auto pair : uniformMap) {
				auto name = pair.first;
				auto uniform = pair.second;
				bgfx::setTexture(uniform.textureIndex, uniform.uniformHandle, uniform.textureHandle);
			}
	
			bgfx::setUniform(u_cameraPos, scene->MainCamera->GetTransform()->Position().data());
			static float* pointLightCount = new float[4]{ 1, 0, 0, 0 };
			bgfx::setUniform(u_pointLightCount, pointLightCount);

			static float* u_lightPosition0 = new float[4]{ 5, 5, 0, 0 };
			bgfx::setUniform(u_lightPosition, u_lightPosition0);

			static float* u_lightColor0 = new float[4]{ 20, 20, 20, 1 };
			bgfx::setUniform(u_lightColor, u_lightColor0);

			bgfx::submit(0, m_program);
		}

		static BgfxMaterial* buildFromMaterial(std::shared_ptr<Material> mat);
	};

	BgfxMaterial* BgfxMaterial::buildFromMaterial(std::shared_ptr<Material> mat) {
		auto bgfxMat = new BgfxMaterial();
		bgfxMat->m_program = loadProgram(mat->VertexShader().c_str(), mat->FragmentShader().c_str());
		for (auto pair : mat->GetParams()) {
			auto name = pair.first;
			auto mat = pair.second;
			BgfxUniform bgfxUniform;
			switch (mat.type) {
			case Sampler:
				auto texture = static_cast<const Texture*>(mat.Value);
				bgfxUniform.textureHandle = createTexture(texture->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
				bgfxUniform.uniformHandle = bgfx::createUniform(name.c_str(), bgfx::UniformType::Sampler);
				bgfxUniform.uniformType = bgfx::UniformType::Sampler;
				bgfxUniform.textureIndex = mat.index;
				bgfxMat->uniformMap[name] = bgfxUniform;
				break;
				// case Vector4:
				// 	break;
				// case Vector3:
				// 	break;
				// case Float:
				// 	break;
			}
		}
		bgfxMat->mat = mat;
		return bgfxMat;
	}
	
	struct BgfxGeometry {
		std::shared_ptr<StaticMeshComponent> meshComponent;
		std::shared_ptr<BgfxMaterial> material;
		bgfx::VertexBufferHandle vbh;
		bgfx::IndexBufferHandle ibh;
		~BgfxGeometry() {
			bgfx::destroy(vbh);
			bgfx::destroy(ibh);
			meshComponent.reset();
			material.reset();
		}

		void Submit(Scene* scene) {
			auto transformMatrix = meshComponent->GetNode()->GetTransform()->ModelMatrix();
			float* transformMatrixArray = new float[transformMatrix.size()];
			// Log("transformMatrix = \n %s", DebugString(transformMatrix));

			Map<Matrix4f>(transformMatrixArray, transformMatrix.rows(), transformMatrix.cols()) = transformMatrix;
			bgfx::setTransform(transformMatrixArray);
			bgfx::setVertexBuffer(0, vbh);
			bgfx::setIndexBuffer(ibh);
			material->Submit(scene);
		}
	};
}
