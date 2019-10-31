#pragma once
#include "Ray.hpp"
namespace Entropy {
	class RTCamera {
	public:
		RTCamera() {
			lower_left_corner = Vector3f(-1.0, -1.0, -1.0);
			horizontal = Vector3f(2.0, 0.0, 0.0);
			vertical = Vector3f(0.0, 2.0, 0.0);
			origin = Vector3f(0.0, 0.0, 0.0);
		}
		Ray get_ray(float u, float v) {
			return Ray(origin,
				lower_left_corner + u * horizontal + v * vertical - origin);
		}

		Vector3f origin;
		Vector3f lower_left_corner;
		Vector3f horizontal;
		Vector3f vertical;

		void setRatio(float ratio) {
			horizontal = Vector3f(2.0 * ratio, 0.0, 0.0);
			lower_left_corner = Vector3f(-1.0 * ratio, -1.0, -1.0);
		}
	};
}