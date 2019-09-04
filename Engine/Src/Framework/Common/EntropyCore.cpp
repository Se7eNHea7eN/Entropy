#include "Framework/Common/EntropyCore.hpp"

using namespace Entropy;

EntropyCore::EntropyCore()
{

}

Renderer* EntropyCore::CreateRenderer(HWND hwnd) {
	return RendererFactory::CreateRenderer(renderAPI, hwnd);
}
