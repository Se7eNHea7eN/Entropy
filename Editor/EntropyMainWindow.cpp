#include "stdafx.h"
#include "EntropyMainWindow.h"
#include "RenderWidget.h"
#include "GenerateScene.hpp"

EntropyMainWindow::EntropyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	auto renderWidget = new RenderWidget(this);
	engineCore->CreateScene("Test Scene");
	auto scene = engineCore->CurrentScene();
	GenerateScene01(scene);
	renderWidget->renderer = engineCore->CreateRenderer((HWND)renderWidget->winId());

	QApplication::postEvent(
		this,
		new QEvent{ QEvent::UpdateRequest });
	ui.sceneRendererContainer->addWidget(renderWidget);
}
