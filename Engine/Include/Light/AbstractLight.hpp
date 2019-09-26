#pragma once
#include "Common/Component.hpp"
#include "Eigen/Core"
namespace Entropy {
	using namespace Eigen;
	class AbstractLight : public Component {
	protected:
		Vector3f lightColor = Vector3f::Identity();
		float lightIntensive = 1;
	public :
		Vector3f GetLightColor() {
			return lightColor;
		}

		float GetInstensive() {
			return lightIntensive;
		}
	};
}

