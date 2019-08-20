#pragma once

#include <QWidget>
#include "../../Direct3DRenderer/Public/Direct3DRenderer.h"
#include "../../Direct3DRenderer/Public/D3DApplication.h"

class EntropyDirect3DWidget : public QWidget
{
	Q_OBJECT
public:
	EntropyDirect3DWidget(QWidget* parent);
	virtual ~EntropyDirect3DWidget();
public slots:
	void Idle();
protected:
	// event
	QPaintEngine* paintEngine() const { return 0; }	// ‘ –Ìdx‰÷»æ

	virtual void resizeEvent(QResizeEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);

	QTimer mTimer;
	QPoint mCurMousePt;

public:
	void CreateD3DApp(HWND hwnd);

	virtual void RenderScene();
private:
	Direct3DRenderer* renderer;
	D3DApplication* mD3DApp;
};
