#include "bgfx_utils.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include <bx/math.h>

#include "bx/timer.h"
#include "Common/EntropyCore.hpp"
#include "Common/Scene.hpp"
#include "Common/Transform.hpp"

#include "Graphic/StaticMeshComponent.hpp"
#include "Graphic/Mesh.hpp"
#include "Graphic/SkyBox.hpp"

#include "BgfxGeometry.hpp"
#include "BgfxRenderer.hpp"
#include "Graphic/Vertex.hpp"
using namespace Eigen;

struct StandardVertexLayout {
	static void init() {
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout StandardVertexLayout::ms_layout;


struct SimpleVertexLayout {
	static void init() {
		simple_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexLayout simple_layout;
};

bgfx::VertexLayout SimpleVertexLayout::simple_layout;


const int VIEWID_SCENE = 0;
const int VIEWID_SKYMAP = 32;

Entropy::BgfxRenderer::BgfxRenderer(HWND hwnd) : hwnd(hwnd) {

}

Entropy::BgfxRenderer::~BgfxRenderer() {
	geometries.clear();
	bgfx::shutdown();
}


bgfx::ProgramHandle skyProgram;
bgfx::UniformHandle s_skybox;
bgfx::TextureHandle cubeTexture;
bgfx::TextureHandle irradianceTexture;

bgfx::VertexBufferHandle cubeVbh;
bgfx::IndexBufferHandle cubeIbh;

bool init = true;
void Entropy::BgfxRenderer::Initialize() {
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
	init.type = bgfx::RendererType::Enum::Direct3D11;
	// 设置供应商接口Vendor PCI ID，默认设置为0将选择第一个设备来显示。
	// #define BGFX_PCI_ID_NONE                UINT16_C(0x0000) //!< Autoselect adapter.
	// #define BGFX_PCI_ID_SOFTWARE_RASTERIZER UINT16_C(0x0001) //!< Software rasterizer.
	// #define BGFX_PCI_ID_AMD                 UINT16_C(0x1002) //!< AMD adapter.
	// #define BGFX_PCI_ID_INTEL               UINT16_C(0x8086) //!< Intel adapter.
	// #define BGFX_PCI_ID_NVIDIA              UINT16_C(0x10de) //!< nVidia adapter.
	init.vendorId = 0;
	// 设置分辨率大小
	init.resolution.width = 2560;
	init.resolution.height = 1440;
	// BGFX_RESET_VSYNC 其作用主要是让显卡的运算和显示器刷新率一致以稳定输出的画面质量。
	init.resolution.reset = BGFX_RESET_VSYNC;
	bgfx::init(init);

	// Enable debug text.
	bgfx::setViewClear(VIEWID_SCENE
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	StandardVertexLayout::init();
	SimpleVertexLayout::init();

	
	bgfx::setDebug(engine->debugMode);

	for (auto obj : StaticMeshComponent::AllStaticMeshComponents) {

		for (auto mesh : obj->GetMeshes()) {
			auto geo = std::make_shared<BgfxGeometry>();
			geo->meshComponent = obj;
			geo->vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(&mesh->vertices[0], mesh->vertices.size() * sizeof(Vertex))
				, StandardVertexLayout::ms_layout
			);
			if (mesh->indices.empty()) {
				mesh->indices.resize(mesh->vertices.size());
				for (int i = 0; i < mesh->vertices.size(); i++) {
					mesh->indices.at(i) = i;
				}
			}

			geo->ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(&mesh->indices[0], mesh->indices.size() * sizeof(uint32_t)),
				BGFX_BUFFER_INDEX32
			);
			auto bgfxMaterial = BgfxMaterial::buildFromMaterial(obj->GetMaterials()[mesh->m_materialIndex]);
			geo->material = std::shared_ptr<BgfxMaterial>(bgfxMaterial);
			geo->indiceType = mesh->m_indiceType;
			geometries.push_back(geo);
		}
	}


	auto skybox = engine->CurrentScene()->GetSkybox();
	if (skybox != nullptr) {
		const uint16_t cubeTextureSize = 4096;

		skyProgram = loadProgram("vs_skybox", "fs_skybox_hdr");
		s_skybox = bgfx::createUniform("s_skybox", bgfx::UniformType::Sampler);
		cubeTexture = bgfx::createTextureCube(cubeTextureSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
		auto hdrTexture = createTexture(skybox->HdrTexture()->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);


		
		float captureProjection[16];
		bx::mtxProj(captureProjection, 90.0f, 1.0f, 0.1f, 10.0f, bgfx::getCaps()->homogeneousDepth);
		Log("homogeneousDepth = %s", bgfx::getCaps()->homogeneousDepth ? "true" : "false");
		float captureViews[6][16];
		if (bgfx::getCaps()->homogeneousDepth) {
			//右
			bx::mtxLookAt(captureViews[0], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
			//左
			bx::mtxLookAt(captureViews[1], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(-1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
			//下
			bx::mtxLookAt(captureViews[2], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f));
			//上
			bx::mtxLookAt(captureViews[3], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f));

			//后
			bx::mtxLookAt(captureViews[4], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
			//前
			bx::mtxLookAt(captureViews[5], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			//右
			bx::mtxLookAt(captureViews[1], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
			//左
			bx::mtxLookAt(captureViews[0], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(-1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
			//下
			bx::mtxLookAt(captureViews[2], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f));
			//上
			bx::mtxLookAt(captureViews[3], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f));

			//后
			bx::mtxLookAt(captureViews[4], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
			//前
			bx::mtxLookAt(captureViews[5], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
		}


		float skyboxVertices[] = {
			// positions          
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		cubeVbh = createVertexBuffer(
			bgfx::makeRef(skyboxVertices, sizeof(skyboxVertices))
			, SimpleVertexLayout::simple_layout
		);

		bgfx::ProgramHandle equirectangularToCubemap = loadProgram("vs_cubemap", "fs_equirectangular");

		bgfx::UniformHandle equirectangularMapHandle = bgfx::createUniform("equirectangularMap", bgfx::UniformType::Sampler);

		bgfx::FrameBufferHandle frameBuffer[6];

		for (uint32_t ii = 0; ii < 6; ++ii) {
			bgfx::Attachment at;
			at.init(cubeTexture, bgfx::Access::Write, uint16_t(ii));
			frameBuffer[ii] = bgfx::createFrameBuffer(1, &at);
		}
		
		for (unsigned int i = 0; i < 6; ++i) {
			bgfx::ViewId viewId = bgfx::ViewId(VIEWID_SKYMAP + i);
			bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL);
			bgfx::setViewClear(viewId
			                   , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			                   , 0xFF00FFff
			                   , 1.0f
			                   , 0
			);
			bgfx::setTexture(0, equirectangularMapHandle, hdrTexture);

			bgfx::setViewRect(viewId, 0, 0, cubeTextureSize, cubeTextureSize);
			bgfx::setViewFrameBuffer(viewId, frameBuffer[i]);
			bgfx::setViewTransform(viewId, captureViews[i], captureProjection);
			bgfx::setVertexBuffer(0, cubeVbh);

			bgfx::submit(viewId, equirectangularToCubemap);

			bgfx::touch(viewId);
		}


		// bgfx::frame();
		
		// bgfx::destroy(equirectangularToCubemap);
		// bgfx::destroy(equirectangularMapHandle);
		// for (uint32_t ii = 0; ii < 6; ++ii) {
		// 	bgfx::destroy(frameBuffer[ii]);
		// // }
		// const uint16_t irradianceTextureSize = 512;
		//
		// bgfx::ProgramHandle irradianceConvolution = loadProgram("vs_cubemap", "fs_irradiance_convolution");
		//
		// bgfx::UniformHandle environmentMapHandle = bgfx::createUniform("environmentMap", bgfx::UniformType::Sampler);
		//
		// bgfx::FrameBufferHandle irradianceFrameBuffer[6];
		// irradianceTexture = bgfx::createTextureCube(irradianceTextureSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
		//
		// for (uint32_t ii = 0; ii < 6; ++ii) {
		// 	bgfx::Attachment at;
		// 	at.init(irradianceTexture, bgfx::Access::Write, uint16_t(ii));
		// 	irradianceFrameBuffer[ii] = bgfx::createFrameBuffer(1, &at);
		// }
		// for (unsigned int i = 0; i < 6; ++i) {
		// 	bgfx::ViewId viewId = bgfx::ViewId(VIEWID_SKYMAP + i +6 );
		// 	bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL);
		// 	bgfx::setViewClear(viewId
		// 		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		// 		, 0xFF00FFff
		// 		, 1.0f
		// 		, 0
		// 	);
		// 	bgfx::setTexture(0, environmentMapHandle, cubeTexture);
		//
		// 	bgfx::setViewRect(viewId, 0, 0, irradianceTextureSize, irradianceTextureSize);
		// 	bgfx::setViewFrameBuffer(viewId, irradianceFrameBuffer[i]);
		// 	bgfx::setViewTransform(viewId, captureViews[i], captureProjection);
		// 	bgfx::setVertexBuffer(0, cubeVbh);
		//
		// 	bgfx::submit(viewId, irradianceConvolution);
		//
		// 	bgfx::touch(viewId);
		// }
		bgfx::frame();
	}

}

void Entropy::BgfxRenderer::Resize(int w, int h) {
	width = w;
	height = h;

}

int lastResetWidth = 0;
int lastResetHeight = 0;
void Entropy::BgfxRenderer::Draw() {
	if(init) {
		init = false;

	}

	if(lastResetWidth != width || lastResetHeight != height) {
		bgfx::reset(width, height, BGFX_RESET_VSYNC);
		auto camera = engine->CurrentScene()->MainCamera;
		camera->SetViewport(width, height);
		lastResetWidth = width;
		lastResetHeight = height;
	}
	bgfx::touch(VIEWID_SCENE);

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

	uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LEQUAL
			// | BGFX_STATE_DEPTH_TEST_LESS
			// | BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
		// | BGFX_STATE_LINEAA
		// | BGFX_STATE_PT_TRISTRIP
		// | BGFX_STATE_PT_LINESTRIP
		;
	bgfx::setVertexBuffer(0, cubeVbh);
	bgfx::setTexture(0, s_skybox, cubeTexture);
	bgfx::setViewFrameBuffer(VIEWID_SCENE, BGFX_INVALID_HANDLE);
	bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL);

	bgfx::submit(0, skyProgram);
	for (auto g : geometries) {
		bgfx::setState(state | g->indiceType);
		g->Submit(engine->CurrentScene());
	}
	bgfx::frame();

}

void Entropy::BgfxRenderer::AwaitRenderFrame() {
	bgfx::renderFrame();
}
