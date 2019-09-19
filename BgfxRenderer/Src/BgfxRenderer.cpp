#include "bgfx_utils.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include <bx/math.h>
#include "BgfxGeometry.hpp"
#include "BgfxRenderer.hpp"
#include "bx/timer.h"
#include "Common/EntropyCore.hpp"
#include "Common/Scene.hpp"

#include "Utils/Debug.hpp"
using namespace Eigen;
struct SimpleVertexLayout {
	static void init() {
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout SimpleVertexLayout::ms_layout;
std::list<std::unique_ptr<Entropy::BgfxGeometry>> geometries;


Entropy::BgfxRenderer::BgfxRenderer(HWND hwnd) : hwnd(hwnd) {
	bgfx::PlatformData pd;
	pd.ndt = NULL;
	pd.nwh = hwnd;
	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd); // 设置平台信息，绑定上层 view

	bgfx::renderFrame();

	bgfx::Init init;
	// 选择一个渲染后端，当设置为 RendererType::Enum::Count 的时候，系统将默认选择一个平台，可以设置Metal，OpenGL ES，Direct 等
	init.type = bgfx::RendererType::Enum::Count;
	// 设置供应商接口Vendor PCI ID，默认设置为0将选择第一个设备来显示。
	// #define BGFX_PCI_ID_NONE                UINT16_C(0x0000) //!< Autoselect adapter.
	// #define BGFX_PCI_ID_SOFTWARE_RASTERIZER UINT16_C(0x0001) //!< Software rasterizer.
	// #define BGFX_PCI_ID_AMD                 UINT16_C(0x1002) //!< AMD adapter.
	// #define BGFX_PCI_ID_INTEL               UINT16_C(0x8086) //!< Intel adapter.
	// #define BGFX_PCI_ID_NVIDIA              UINT16_C(0x10de) //!< nVidia adapter.
	init.vendorId = 0;
	// 设置分辨率大小
	init.resolution.width = 1024;
	init.resolution.height = 768;
	// BGFX_RESET_VSYNC 其作用主要是让显卡的运算和显示器刷新率一致以稳定输出的画面质量。
	init.resolution.reset = BGFX_RESET_VSYNC;
	bgfx::init(init);

	// Enable debug text.
	// bgfx::setDebug(true);
	bgfx::setViewClear(0
	                   , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
	                   , 0x303030ff
	                   , 1.0f
	                   , 0
	);

	SimpleVertexLayout::init();
}

Entropy::BgfxRenderer::~BgfxRenderer() {
	geometries.clear();
	bgfx::shutdown();
}

void Entropy::BgfxRenderer::Initialize() {

	for (auto obj : engine->CurrentScene()->Geometries) {
	
		for (auto mesh : obj->m_Mesh) {
			auto geo = std::make_unique<BgfxGeometry>();
			geo->geometry = obj;
			geo->vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(mesh->m_vertexBuffer, mesh->m_vertexBufferSize)
				, SimpleVertexLayout::ms_layout
			);
	
			geo->ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(mesh->m_indexBuffer, mesh->m_indexBufferSize),
				BGFX_BUFFER_INDEX32
			);
			auto bgfxMaterial = std::make_shared<BgfxMaterial>();
			bgfxMaterial->mat = obj->m_Materials[mesh->m_materialIndex];
			bgfxMaterial->m_program = loadProgram(bgfxMaterial->mat->VertexShader().c_str(), bgfxMaterial->mat->FragmentShader().c_str());
			if(bgfxMaterial->mat->m_Albedo.ValueMap == nullptr) {
				bgfxMaterial->t_albedo = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8, 
					BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
					bgfx::makeRef(&bgfxMaterial->mat->m_Albedo.Value, 4));

			}else {
				bgfxMaterial->t_albedo = loadTexture(bgfxMaterial->mat->m_Albedo.ValueMap->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
			}

			if (bgfxMaterial->mat->m_Metallic.ValueMap == nullptr) {
				bgfxMaterial->t_metallic = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::R8,
					BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
					bgfx::makeRef(new uint8_t(bgfxMaterial->mat->m_Metallic.Value * 255), 1));
			}else {
				bgfxMaterial->t_metallic = loadTexture(bgfxMaterial->mat->m_Metallic.ValueMap->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
			}

			if (bgfxMaterial->mat->m_Roughness.ValueMap == nullptr) {

				bgfxMaterial->t_roughness = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::R8,
					BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
					bgfx::makeRef(new uint8_t(bgfxMaterial->mat->m_Roughness.Value * 255), 1));
			}else {
				bgfxMaterial->t_roughness = loadTexture(bgfxMaterial->mat->m_Roughness.ValueMap->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
			}

			if (bgfxMaterial->mat->m_Normal.ValueMap == nullptr) {

				bgfxMaterial->t_normal = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::R8,
					BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
					bgfx::makeRef(&bgfxMaterial->mat->m_Normal.Value, 4));
			}
			else {
				bgfxMaterial->t_normal = loadTexture(bgfxMaterial->mat->m_Normal.ValueMap->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
			}

			
			bgfxMaterial->t_ao = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::R8,
				BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
				bgfx::makeRef(new uint8_t(bgfxMaterial->mat->m_AmbientOcclusion.Value * 255), 1));

			// texture = loadTexture("Textures/test.png", BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);
			geo->material = bgfxMaterial;
			geometries.push_back(std::move(geo));
		}
	}
	// Create program from shaders.
	// m_program = loadProgram("vs_mesh", "fs_mesh");;

	auto camera = engine->CurrentScene()->MainCamera;
	// bgfx::createUniform("",bgfx::UniformType::Count)
}

void Entropy::BgfxRenderer::Resize(int w, int h) {
	width = w;
	height = h;
	bgfx::reset(w, h, BGFX_RESET_VSYNC);
	auto camera = engine->CurrentScene()->MainCamera;
	camera->SetViewport(w, h);
}


void Entropy::BgfxRenderer::Draw() {
	{
		auto camera = engine->CurrentScene()->MainCamera;
		auto viewMatrix = camera->ViewMatrix().matrix();
		
		Map<Matrix4f>(viewMatrixArray, viewMatrix.rows(), viewMatrix.cols()) = viewMatrix;
		

		bx::mtxProj(projectionMatrixArray, bx::toDeg(camera->FovY()), float(width) / float(height), camera->NearDistance(), camera->FarDistance(), bgfx::getCaps()->homogeneousDepth);
		

		//bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, viewMatrixArray, projectionMatrixArray);
		
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
	}

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	uint64_t state = 0
		| BGFX_STATE_WRITE_R
		| BGFX_STATE_WRITE_G
		| BGFX_STATE_WRITE_B
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		// | BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
		// | BGFX_STATE_PT_LINESTRIP
	;

	for (auto iterator = geometries.begin(); iterator != geometries.end(); ++iterator){
		auto transformMatrix = iterator->get()->geometry->GetTransform()->ModelMatrix();
		float* transformMatrixArray = new float[transformMatrix.size()];
		// Log("transformMatrix = \n %s", DebugString(transformMatrix));

		Map<Matrix4f>(transformMatrixArray, transformMatrix.rows(), transformMatrix.cols()) = transformMatrix;
	
		bgfx::setTransform(transformMatrixArray);
		bgfx::setVertexBuffer(0, iterator->get()->vbh);
		bgfx::setIndexBuffer(iterator->get()->ibh);
		bgfx::setState(state);
		bgfx::setTexture(0, iterator->get()->material->s_albedo, iterator->get()->material->t_albedo);
		bgfx::setTexture(1, iterator->get()->material->s_metallic, iterator->get()->material->t_metallic);
		bgfx::setTexture(2, iterator->get()->material->s_roughness, iterator->get()->material->t_roughness);
		bgfx::setTexture(3, iterator->get()->material->s_ao, iterator->get()->material->t_ao);
		bgfx::setTexture(4, iterator->get()->material->s_normal, iterator->get()->material->t_normal);
		bgfx::setUniform(iterator->get()->material->u_cameraPos, engine->CurrentScene()->MainCamera->GetTransform()->Position().data());
		static float* u_pointLightCount = new float[4] {1, 0, 0, 0};
		bgfx::setUniform(iterator->get()->material->u_pointLightCount, u_pointLightCount);

		static float* u_lightPosition0 = new float[4] {5, 5, 0, 0};
		bgfx::setUniform(iterator->get()->material->u_lightPosition, u_lightPosition0);

		static float* u_lightColor0 = new float[4] {10, 10, 10, 1};
		bgfx::setUniform(iterator->get()->material->u_lightColor, u_lightColor0);
		
		bgfx::submit(0, iterator->get()->material->m_program);
	}
	bgfx::frame();

}
