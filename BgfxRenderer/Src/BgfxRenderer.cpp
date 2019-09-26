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

#include "BgfxGeometry.hpp"
#include "BgfxRenderer.hpp"
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

	for (auto obj : StaticMeshComponent::AllStaticMeshComponents) {
	
		for (auto mesh : obj->GetMeshes()) {
			auto geo = std::make_shared<BgfxGeometry>();
			geo->meshComponent = obj;
			geo->vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(mesh->m_vertexBuffer, mesh->m_vertexBufferSize)
				, SimpleVertexLayout::ms_layout
			);
	
			geo->ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(mesh->m_indexBuffer, mesh->m_indexBufferSize),
				BGFX_BUFFER_INDEX32
			);
			auto bgfxMaterial = BgfxMaterial::buildFromMaterial(obj->GetMaterials()[mesh->m_materialIndex]);
			geo->material = std::shared_ptr<BgfxMaterial>(bgfxMaterial);
			geo->indiceType = mesh->m_indiceType;
			geometries.push_back(geo);
		}
	}

	auto camera = engine->CurrentScene()->MainCamera;
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
		// | BGFX_STATE_LINEAA
		// | BGFX_STATE_PT_TRISTRIP
		// | BGFX_STATE_PT_LINESTRIP
	;
	
	for(auto g : geometries) {
		bgfx::setState(state);
		g->Submit(engine->CurrentScene());
	}
	bgfx::frame();

}
