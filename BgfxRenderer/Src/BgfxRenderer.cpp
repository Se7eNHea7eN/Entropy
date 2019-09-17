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
	
		bgfx::submit(0, iterator->get()->material->m_program);
	}
	bgfx::frame();

}
