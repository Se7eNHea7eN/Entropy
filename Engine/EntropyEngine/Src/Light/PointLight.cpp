#include "Light/PointLight.hpp"


using namespace Entropy;
std::vector<std::shared_ptr<PointLight>> PointLight::AllPointLights;

int PointLight::Initialize() {
	AllPointLights.push_back(std::dynamic_pointer_cast<PointLight>(SharedPtr()));
	return 0;
}

void PointLight::Finalize() {
	for (auto it = AllPointLights.begin(); it != AllPointLights.end(); ++it) {
		if (it->get() == this) {
			AllPointLights.erase(it);
			return;
		}
	}
}
