#include "stdafx.h"
#include "EntropyDirect3DWidget.h"
#include "D3D.h"
#include <QTimer>
EntropyDirect3DWidget::EntropyDirect3DWidget(QWidget* parent) {
	QWidget::setAttribute(Qt::WA_PaintOnScreen); // ����DX��Ⱦ ...
	setFocusPolicy(Qt::WheelFocus); // ������ʱ�� ...
	setMouseTracking(true); // ��������ƶ� ...

	mTimer.setInterval(0);
	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(Idle()));
	mTimer.start();
}

EntropyDirect3DWidget::~EntropyDirect3DWidget() {
	mTimer.stop();
}

void EntropyDirect3DWidget::Idle() {
	RenderScene();
}

void EntropyDirect3DWidget::resizeEvent(QResizeEvent* event) {
	QSize size = event->size();
	int width = size.width();
	int height = size.height();
	// mD3DApp->SetClientWidth(width);
	// mD3DApp->SetClientHeight(height);
	// mD3DApp->ResetQt();
}

void EntropyDirect3DWidget::mousePressEvent(QMouseEvent* event) {
}

void EntropyDirect3DWidget::mouseReleaseEvent(QMouseEvent* event) {
}

void EntropyDirect3DWidget::mouseMoveEvent(QMouseEvent* event) {
}

void EntropyDirect3DWidget::CreateD3DApp(HWND hwnd) {
	RECT rc;
	::GetClientRect(hwnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	//
	// mD3DApp = new D3DApplication(nullptr);
	// mD3DApp->SetClientWidth(width);
	// mD3DApp->SetClientHeight(height);
	// mD3DApp->InitQt(hwnd);
}

void EntropyDirect3DWidget::RenderScene() {
	// mD3DApp->RenderQt();
}


