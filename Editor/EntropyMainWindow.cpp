#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "RenderWidget.h"
#include "GenerateScene.hpp"
#include "RayTracingRenderer.hpp"
using namespace Entropy;
RayTracingRenderer* rtRenderer;
EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.buttonRender, SIGNAL(clicked()), this, SLOT(ClickRender()));
	
	engineCore->CreateScene("Test Scene");
	auto scene = engineCore->CurrentScene();
	// GenerateScene02(scene);
	
	ui.previewRenderView->renderer = engineCore->CreateRenderer((HWND)ui.previewRenderView->winId());
	rtRenderer = new RayTracingRenderer((HWND)ui.rayTracingRenderView->winId());
	ui.rayTracingRenderView->renderer = rtRenderer;
	rtRenderer->SetOnUpdate([=]
		{
			ui.rayTracingRenderView->render();
		});
	ui.treeWidget->SetScene(scene);
}

void EntropyMainWindow::ClickRender() {
	rtRenderer->Render();
}
