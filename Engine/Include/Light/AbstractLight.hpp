#pragma once
#include "Common/Component.hpp"
#include "Eigen/Core"
namespace Entropy {
	using namespace Eigen;
	class AbstractLight : public Component {
	protected:
		Vector3f lightColor = Vector3f(1,1,1);
		float lightIntensive = 1;
	public :
		Vector3f GetLightColor() {
			return lightColor;
		}

		float GetInstensive() {
			return lightIntensive;
		}

		void SetLightColor(Vector3f lightColor) {
			this->lightColor = lightColor;
		}

		void SetIntensive(float intensive) {
			this->lightIntensive = intensive;
		}
	};
}

