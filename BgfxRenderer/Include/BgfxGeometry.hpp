#pragma once
#include <map>

#include "bgfx/bgfx.h"
#include "Common/Scene.hpp"
#include "Common/Camera.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/StaticMeshComponent.hpp"
#include "Light/PointLight.hpp"

namespace Entropy {

	struct BgfxUniform {
		bgfx::UniformHandle uniformHandle;
		bgfx::UniformType::Enum uniformType;
		bgfx::TextureHandle textureHandle;
		int textureIndex;

		const void* value;
		uint32_t num;
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

		float lightCount[4];
		float lightPosition[10][4];
		float lightColor[10][4];
		void Submit(Scene* scene,bool enableLighting = true) {
			for (auto pair : uniformMap) {
				auto name = pair.first;
				auto uniform = pair.second;
				switch(uniform.uniformType) {

				case bgfx::UniformType::Sampler:
					bgfx::setTexture(uniform.textureIndex, uniform.uniformHandle, uniform.textureHandle);
					break;
				case bgfx::UniformType::Vec4:
					bgfx::setUniform(uniform.uniformHandle, uniform.value, uniform.num);
					break;
				}
			}
	
			bgfx::setUniform(u_cameraPos, scene->MainCamera->GetNode()->GetTransform()->Position().data());


			if(enableLighting) {
				lightCount[0] = PointLight::AllPointLights.size();
				for (int i = 0; i < PointLight::AllPointLights.size(); ++i) {
					auto light = PointLight::AllPointLights[i];
					lightPosition[i][0] = light->GetNode()->GetTransform()->Position().x();
					lightPosition[i][1] = light->GetNode()->GetTransform()->Position().y();
					lightPosition[i][2] = light->GetNode()->GetTransform()->Position().z();
					lightPosition[i][3] = 0;
					bgfx::setUniform(u_lightPosition, lightPosition);

					auto realColor = light->GetInstensive() * light->GetLightColor();
					lightColor[i][0] = realColor.x();
					lightColor[i][1] = realColor.y();
					lightColor[i][2] = realColor.z();
					lightColor[i][3] = 1;
					bgfx::setUniform(u_lightPosition, lightPosition);
					bgfx::setUniform(u_lightColor, lightColor);
				}
			}else {
				lightCount[0] = 0;
			}

			bgfx::setUniform(u_pointLightCount, lightCount);

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
			switch (mat.type) {
			case Sampler: {
				BgfxUniform bgfxUniform;
				auto texture = static_cast<const Texture*>(mat.Value);
				bgfxUniform.textureHandle = createTexture(texture->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
				bgfxUniform.uniformHandle = bgfx::createUniform(name.c_str(), bgfx::UniformType::Sampler);
				bgfxUniform.uniformType = bgfx::UniformType::Sampler;
				bgfxUniform.textureIndex = mat.index;
				bgfxMat->uniformMap[name] = bgfxUniform;
			}
				break;
			case Vector4: {
				BgfxUniform bgfxUniform;
				bgfxUniform.uniformHandle = bgfx::createUniform(name.c_str(), bgfx::UniformType::Vec4);
				bgfxUniform.uniformType = bgfx::UniformType::Vec4;
				bgfxUniform.value = mat.Value;
				bgfxUniform.num = mat.num;
				bgfxMat->uniformMap[name] = bgfxUniform;
			}
			
				break;
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
		uint64_t indiceType;
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
			if(ibh.idx > 0)
				bgfx::setIndexBuffer(ibh);
			material->Submit(scene,meshComponent->EnableLighting());
		}
	};
}
