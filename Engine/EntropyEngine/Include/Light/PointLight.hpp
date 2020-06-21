#pragma once
#include "AbstractLight.hpp"
#include <vector>

namespace Entropy {
	class PointLight : public AbstractLight {
	public:
		static std::vector<std::shared_ptr<PointLight>> AllPointLights;
		int Initialize() override;
		void Finalize() override;
	};
	
}
