#include "../Public/EntropyCore.h"

EntropyCore::EntropyCore()
{
}

Renderer* EntropyCore::CreateRenderer(HWND hwnd) {
	return RendererFactory::CreateRenderer(renderAPI, hwnd);
}
