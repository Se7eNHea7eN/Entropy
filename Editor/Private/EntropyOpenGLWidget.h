#ifndef ENTROPYOPENGLWIDGET_H
#define ENTROPYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include "../../Renderer/Public/Renderer.h"

class EntropyOpenGLWidget : public QOpenGLWidget
{

private:
    Q_OBJECT
public:
	explicit EntropyOpenGLWidget(QWidget *parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

signals:

public slots:

protected:
	Renderer* renderer;
};

#endif ENTROPYOPENGLWIDGET_H
