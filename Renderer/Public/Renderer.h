#pragma once
class Renderer {
public:
	Renderer() = default;
	virtual ~Renderer() = default;
	virtual void initialize() = 0;
	virtual void resize(int w, int h) = 0;
	virtual void draw() = 0;
protected:
	int width;
	int height;
};