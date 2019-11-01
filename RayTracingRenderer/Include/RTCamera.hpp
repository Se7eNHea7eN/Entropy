#pragma once
#include "Ray.hpp"
namespace Entropy {
	class RTCamera {
	public:
		RTCamera(Vector3f lookfrom, Vector3f lookat, Vector3f vup, float vfov, float aspect) {
			const float PI = 3.1415926535897932384;
			// horizontal = Vector3f(2.0 * ratio, 0.0, 0.0);
			// lower_left_corner = Vector3f(-1.0 * ratio, -1.0, -1.0);
			// vertical = Vector3f(0.0, 2.0, 0.0);
			// origin = Vector3f(0.0, 0.0, 0.0);

			float theta = vfov * PI / 180;
			float half_height = tan(theta / 2);
			float half_width = aspect * half_height;

			Vector3f u, v, w;
			origin = lookfrom;
			w = (lookfrom - lookat).normalized();
			u = vup.cross(w).normalized();
			v = w.cross(u);
			lower_left_corner = origin - half_width * u - half_height * v - w;
			horizontal = 2 * half_width * u;
			vertical = 2 * half_height * v;
		}
		Ray get_ray(float u, float v) {
			return Ray(origin,
				lower_left_corner + u * horizontal + v * vertical - origin);
		}

		Vector3f origin;
		Vector3f lower_left_corner;
		Vector3f horizontal;
		Vector3f vertical;

	};
}