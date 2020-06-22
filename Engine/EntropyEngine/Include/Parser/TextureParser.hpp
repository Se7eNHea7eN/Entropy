#pragma once
#include <vector>
class Texture;
namespace Entropy {
	std::vector<std::shared_ptr<Texture2D>>* ParseTexture2D(std::string path);
}