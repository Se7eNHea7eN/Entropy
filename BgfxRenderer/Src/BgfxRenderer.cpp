#include "BgfxRenderer.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

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
	bgfx::setDebug(true);
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0xFF0000ff
		, 1.0f
		, 0
	);

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
