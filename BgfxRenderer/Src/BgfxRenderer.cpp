#include "bgfx_utils.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include <bx/math.h>

#include <bx/pixelformat.h>
#include <bgfx/bgfx.h>

#include <tinystl/allocator.h>
#include <tinystl/vector.h>

#include "BgfxRenderer.hpp"
#include "bx/timer.h"
#include "Common/EntropyCore.hpp"
#include "Common/Scene.hpp"

struct PosColorVertex {
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init() {
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};


struct SimpleVertexLayout {
	float m_x;
	float m_y;
	float m_z;

	static void init() {
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f, 1.0f, 1.0f, 0xff000000},
	{1.0f, 1.0f, 1.0f, 0xff0000ff},
	{-1.0f, -1.0f, 1.0f, 0xff00ff00},
	{1.0f, -1.0f, 1.0f, 0xff00ffff},
	{-1.0f, 1.0f, -1.0f, 0xffff0000},
	{1.0f, 1.0f, -1.0f, 0xffff00ff},
	{-1.0f, -1.0f, -1.0f, 0xffffff00},
	{1.0f, -1.0f, -1.0f, 0xffffffff},
};


static const uint16_t s_cubeTriStrip[] =
{
	0, 1, 2,
	3,
	7,
	1,
	5,
	0,
	4,
	2,
	6,
	7,
	4,
	5,
};

// bgfx::VertexBufferHandle m_vbh;
// bgfx::IndexBufferHandle m_ibh;

bgfx::ProgramHandle m_program;
int64_t m_timeOffset;

std::list<bgfx::VertexBufferHandle> vbhs;
std::list<bgfx::IndexBufferHandle> ibhs;


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
	PosColorVertex::init();
	SimpleVertexLayout::init();
}

Entropy::BgfxRenderer::~BgfxRenderer() {
	// bgfx::destroy(m_ibh);
	// bgfx::destroy(m_vbh);
	bgfx::destroy(m_program);
	bgfx::shutdown();
}

void Entropy::BgfxRenderer::Initialize() {

	for (auto obj : engine->CurrentScene()->Geometries) {
		auto meshes = obj->GetMesh();

		for (auto mesh : meshes) {
			auto vbh = bgfx::createVertexBuffer(
				// Static data can be passed with bgfx::makeRef
				bgfx::makeRef(mesh->m_vertexBuffer, sizeof(mesh->m_vertexCount))
				, PosColorVertex::ms_layout
			);
			vbhs.push_back(vbh);


			auto ibh = bgfx::createIndexBuffer(
				// Static data can be passed with bgfx::makeRef
				bgfx::makeRef(mesh->m_indexBuffer, sizeof(mesh->m_indexCount))
			);
			ibhs.push_back(ibh);
		}

	}
	// Create vertex stream declaration.

	// Create static vertex buffer.
	// m_vbh = bgfx::createVertexBuffer(
	// 	// Static data can be passed with bgfx::makeRef
	// 	bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
	// 	, SimpleVertexLayout::ms_layout
	// );
	//
	// m_ibh = bgfx::createIndexBuffer(
	// 	// Static data can be passed with bgfx::makeRef
	// 	bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip))
	// );

	// Create program from shaders.
	m_program = loadProgram("vs_cubes", "fs_cubes");

	m_timeOffset = bx::getHPCounter();
}

void Entropy::BgfxRenderer::Resize(int w, int h) {
	width = w;
	height = h;
	bgfx::reset(w, h, BGFX_RESET_VSYNC);
}

void Entropy::BgfxRenderer::Draw() {
	// bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
	float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));
	// printf("%f", time);
	// std::cout << time;
	const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
	const bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

	// Set view and projection matrix for view 0.
	{
		float view[16];
		bx::mtxLookAt(view, eye, at);
		engine->CurrentScene()->MainCamera->lookAt()
		float proj[16];
		bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
	}

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	// bgfx::IndexBufferHandle ibh = m_ibh[m_pt];
	uint64_t state = 0
		| BGFX_STATE_WRITE_R
		| BGFX_STATE_WRITE_G
		| BGFX_STATE_WRITE_B
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
		| BGFX_STATE_PT_TRISTRIP;

	// // Submit 11x11 cubes.
	// for (uint32_t yy = 0; yy < 11; ++yy)
	// {
	// 	for (uint32_t xx = 0; xx < 11; ++xx)
	// 	{
	// 		float mtx[16];
	// 		bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
	// 		mtx[12] = -15.0f + float(xx) * 3.0f;
	// 		mtx[13] = -15.0f + float(yy) * 3.0f;
	// 		mtx[14] = 0.0f;
	//
	// 		// Set model matrix for rendering.
	// 		bgfx::setTransform(mtx);
	//
	// 		// Set vertex and index buffer.
	// 		bgfx::setVertexBuffer(0, m_vbh);
	// 		bgfx::setIndexBuffer(m_ibh);
	//
	// 		// Set render states.
	// 		bgfx::setState(state);
	//
	// 		// Submit primitive for rendering to view 0.
	// 		bgfx::submit(0, m_program);
	// 	}
	// }

	bgfx::frame();

}
