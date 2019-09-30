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
struct SimpleVertexLayout {
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

bgfx::VertexLayout SimpleVertexLayout::ms_layout;


const int VIEWID_SCENE = 0;
const int VIEWID_SKYMAP = 1;

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
	init.type = bgfx::RendererType::Enum::OpenGL;
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

	SimpleVertexLayout::init();
}

Entropy::BgfxRenderer::~BgfxRenderer() {
	geometries.clear();
	bgfx::shutdown();
}


bgfx::ProgramHandle skyProgram;
bgfx::UniformHandle s_skybox;
bgfx::TextureHandle cubeTexture;
bgfx::VertexBufferHandle cubeVbh;
bgfx::IndexBufferHandle cubeIbh;

void Entropy::BgfxRenderer::Initialize() {

	bgfx::setDebug(engine->debugMode);

	for (auto obj : StaticMeshComponent::AllStaticMeshComponents) {
	
		for (auto mesh : obj->GetMeshes()) {
			auto geo = std::make_shared<BgfxGeometry>();
			geo->meshComponent = obj;
			geo->vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(&mesh->vertices[0], mesh->vertices.size() * sizeof(Vertex))
				, SimpleVertexLayout::ms_layout
			);
			if(mesh->indices.empty()) {
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

	bgfx::touch(VIEWID_SCENE);
	
	auto skybox = engine->CurrentScene()->GetSkybox();
	if(skybox != nullptr) {
		skyProgram = loadProgram("vs_skybox", "fs_skybox_hdr");
		 s_skybox = bgfx::createUniform("s_skybox", bgfx::UniformType::Sampler);
		cubeTexture = bgfx::createTextureCube(512, true, 1, bgfx::TextureFormat::RGBA16F);
		auto hdrTexture =  createTexture(skybox->HdrTexture()->m_pImage.get(), BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_W_CLAMP);

		auto frameBuffer = bgfx::createFrameBuffer(1,&cubeTexture);
		
		float captureProjection[16];
		bx::mtxProj(captureProjection, 90.0f, 1.0f, 0.1f, 10.0f, bgfx::getCaps()->homogeneousDepth);

		float captureViews[6][16];
		bx::mtxLookAt(captureViews[0], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(1.0f, 0.0f, 0.0f)	,bx::Vec3(0.0f, -1.0f, 0.0f));
		bx::mtxLookAt(captureViews[1], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(-1.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
		bx::mtxLookAt(captureViews[2], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f));
		bx::mtxLookAt(captureViews[3], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, -1.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f));
		bx::mtxLookAt(captureViews[4], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, 1.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
		bx::mtxLookAt(captureViews[5], bx::Vec3(0.0f, 0.0f, 0.0f), bx::Vec3(0.0f, 0.0f, -1.0f), bx::Vec3(0.0f, -1.0f, 0.0f));
		
		bgfx::ProgramHandle equirectangularToCubemap = loadProgram("vs_cubemap", "fs_equirectangular");
		bgfx::UniformHandle uniformHandle = bgfx::createUniform("equirectangularMap", bgfx::UniformType::Sampler);;

		static float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-left
			// front face										 0.0f, 0.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left
			// left face										 0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // top-right
			// right face										 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face										 0.0f, 0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right
			// top face											 0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,0.0f, 0.0f, 0.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f  // bottom-left        
		};
		cubeVbh = createVertexBuffer(
			bgfx::makeRef(vertices, sizeof(vertices))
			, SimpleVertexLayout::ms_layout
		);
		// cubeIbh = = bgfx::createIndexBuffer(
		// 	bgfx::makeRef(&mesh->indices[0], mesh->indices.size() * sizeof(uint32_t)),
		// 	BGFX_BUFFER_INDEX32
		// );
		for (unsigned int i = 0; i < 6; ++i)
		{
	
			bgfx::setTexture(0, uniformHandle, hdrTexture);

			bgfx::setViewRect(VIEWID_SKYMAP, 0, 0, 512, 512);
			bgfx::setViewFrameBuffer(VIEWID_SKYMAP, frameBuffer);
			bgfx::setViewTransform(VIEWID_SKYMAP, captureViews[i], captureProjection);
			bgfx::setVertexBuffer(0, cubeVbh);

			bgfx::submit(VIEWID_SKYMAP, equirectangularToCubemap);
		}
		bgfx::frame();

	}


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

	uint64_t state = 0
		| BGFX_STATE_WRITE_R
		| BGFX_STATE_WRITE_G
		| BGFX_STATE_WRITE_B
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		// | BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
		// | BGFX_STATE_LINEAA
		// | BGFX_STATE_PT_TRISTRIP
		// | BGFX_STATE_PT_LINESTRIP
	;
	bgfx::setVertexBuffer(0, cubeVbh);
	bgfx::setTexture(0,s_skybox, cubeTexture);
	bgfx::submit(0,skyProgram);
	for(auto g : geometries) {
		bgfx::setState(state | g->indiceType);
		g->Submit(engine->CurrentScene());
	}
	bgfx::frame();

}
