#ifndef ENTROPYOPENGLWIDGET_H
#define ENTROPYOPENGLWIDGET_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <QOpenGLWidget>
#include "QOpenGLFunctions"

class EntropyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
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
};

#endif ENTROPYOPENGLWIDGET_H
