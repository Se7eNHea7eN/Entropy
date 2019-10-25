#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "RenderWidget.h"
#include "GenerateScene.hpp"
#include "RayTracingRenderer.hpp"
using namespace Entropy;

EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	engineCore->CreateScene("Test Scene");
	auto scene = engineCore->CurrentScene();
	GenerateScene02(scene);
	ui.previewRenderView->renderer = engineCore->CreateRenderer((HWND)ui.previewRenderView->winId());
	ui.rayTracingRenderView->renderer = new RayTracingRenderer((HWND)ui.rayTracingRenderView->winId());
	ui.treeWidget->SetScene(scene);
}
