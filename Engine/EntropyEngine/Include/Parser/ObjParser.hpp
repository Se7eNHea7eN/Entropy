#pragma once
#include <Resource\Mesh.hpp>

namespace Entropy {
	std::vector<std::shared_ptr<Mesh>>* ParseObj(std::string path);
}
