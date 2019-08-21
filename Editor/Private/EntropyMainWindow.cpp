#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "RenderWidget.h"
#include "../../GLRenderer/Public/GLRenderer.h"
#include "../../Direct3DRenderer/Public/Direct3DRenderer.h"

EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	auto renderWidget = new RenderWidget(this);
	renderWidget->renderer = engineCore->CreateRenderer((HWND)renderWidget->winId());
	ui.sceneRendererContainer->addWidget(renderWidget);
}
