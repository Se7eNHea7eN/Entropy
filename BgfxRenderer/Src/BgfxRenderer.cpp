#include "bgfx_utils.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
// #include <bx/math.h>
//
// #include <bx/pixelformat.h>
// #include <bgfx/bgfx.h>
//
// #include <tinystl/allocator.h>
// #include <tinystl/vector.h>

#include "BgfxRenderer.hpp"

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
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

static const uint16_t s_cubeLineList[] =
{
	0, 1,
	0, 2,
	0, 4,
	1, 3,
	1, 5,
	2, 3,
	2, 6,
	3, 7,
	4, 5,
	4, 6,
	5, 7,
	6, 7,
};

static const uint16_t s_cubeLineStrip[] =
{
	0, 2, 3, 1, 5, 7, 6, 4,
	0, 2, 6, 4, 5, 7, 3, 1,
	0,
};

static const uint16_t s_cubePoints[] =
{
	0, 1, 2, 3, 4, 5, 6, 7
};

// bgfx::VertexBufferHandle m_vbh;
// bgfx::ProgramHandle m_program;
int64_t m_timeOffset;
int32_t m_pt;
Entropy::BgfxRenderer::BgfxRenderer(HWND hwnd) : hwnd(hwnd) {
}

Entropy::BgfxRenderer::~BgfxRenderer() {
	bgfx::shutdown();
}

void Entropy::BgfxRenderer::initialize() {
	bgfx::PlatformData pd;
	pd.ndt = NULL;
	pd.nwh = hwnd;
	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd); // ����ƽ̨��Ϣ�����ϲ� view

	bgfx::renderFrame();


	bgfx::Init init;
	// ѡ��һ����Ⱦ��ˣ�������Ϊ RendererType::Enum::Count ��ʱ��ϵͳ��Ĭ��ѡ��һ��ƽ̨����������Metal��OpenGL ES��Direct ��
	init.type = bgfx::RendererType::Enum::Count;
	// ���ù�Ӧ�̽ӿ�Vendor PCI ID��Ĭ������Ϊ0��ѡ���һ���豸����ʾ��
	// #define BGFX_PCI_ID_NONE                UINT16_C(0x0000) //!< Autoselect adapter.
	// #define BGFX_PCI_ID_SOFTWARE_RASTERIZER UINT16_C(0x0001) //!< Software rasterizer.
	// #define BGFX_PCI_ID_AMD                 UINT16_C(0x1002) //!< AMD adapter.
	// #define BGFX_PCI_ID_INTEL               UINT16_C(0x8086) //!< Intel adapter.
	// #define BGFX_PCI_ID_NVIDIA              UINT16_C(0x10de) //!< nVidia adapter.
	init.vendorId = 0;
	// ���÷ֱ��ʴ�С
	init.resolution.width = 1024;
	init.resolution.height = 768;
	// BGFX_RESET_VSYNC ��������Ҫ�����Կ����������ʾ��ˢ����һ�����ȶ�����Ļ���������
	init.resolution.reset = BGFX_RESET_VSYNC;
	bgfx::init(init);

	// Enable debug text.
	bgfx::setDebug(true);
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	// Create vertex stream declaration.
	// PosColorVertex::init();

	// Create static vertex buffer.
	// m_vbh = bgfx::createVertexBuffer(
	// 	// Static data can be passed with bgfx::makeRef
	// 	bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
	// 	, PosColorVertex::ms_layout
	// );


	// Create program from shaders.
	// m_program = loadProgram("vs_cubes", "fs_cubes");

	//m_timeOffset = bx::getHPCounter();
}

void Entropy::BgfxRenderer::resize(int w, int h) {
	width = w;
	height = h;
	bgfx::reset(w, h, BGFX_RESET_VSYNC);
}

void Entropy::BgfxRenderer::draw() {
	bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

	// bgfx::dbgTextClear();
	//
	// bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
	//
	// bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
	// bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
	//
	// const bgfx::Stats* stats = bgfx::getStats();
	// bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters."
	// 	, stats->width
	// 	, stats->height
	// 	, stats->textWidth
	// 	, stats->textHeight
	// );
	// bgfx::renderFrame();
	bgfx::touch(0);
	bgfx::frame();

}
