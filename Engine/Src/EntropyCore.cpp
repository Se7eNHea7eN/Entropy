#include "EntropyCore.hpp"
#include "Camera.hpp"
using namespace Entropy;

EntropyCore::EntropyCore()
{
	auto camera = new Camera();
	camera->perspective(0, 0, 0, 0);
}

Renderer* EntropyCore::CreateRenderer(HWND hwnd) {
	return RendererFactory::CreateRenderer(renderAPI, hwnd);
}
