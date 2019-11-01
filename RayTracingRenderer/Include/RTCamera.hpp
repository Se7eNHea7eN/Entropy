#pragma once
#include "Ray.hpp"
#include "Utils.hpp"

namespace Entropy {
	class RTCamera {
	public:
		RTCamera(Vector3f lookfrom, Vector3f lookat, Vector3f vup, float vfov, float aspect,
			float aperture, float focus_dist) {
			lens_radius = aperture / 2;
			const float PI = 3.1415926535897932384;
			// horizontal = Vector3f(2.0 * ratio, 0.0, 0.0);
			// lower_left_corner = Vector3f(-1.0 * ratio, -1.0, -1.0);
			// vertical = Vector3f(0.0, 2.0, 0.0);
			// origin = Vector3f(0.0, 0.0, 0.0);

			float theta = vfov * PI / 180;
			float half_height = tan(theta / 2);
			float half_width = aspect * half_height;

			origin = lookfrom;
			w = (lookfrom - lookat).normalized();
			u = vup.cross(w).normalized();
			v = w.cross(u);
			lower_left_corner = origin
				- half_width * focus_dist * u
				- half_height * focus_dist * v
				- focus_dist * w;
			horizontal = 2 * half_width * focus_dist * u;
			vertical = 2 * half_height * focus_dist * v;
		}
		Vector3f random_in_unit_disk() {
			Vector3f p;
			do {
				p = 2.0 * Vector3f(random_double(), random_double(), 0) - Vector3f(1, 1, 0);
			} while (p.squaredNorm() >= 1.0);
			return p;
		}
		Ray get_ray(float s, float t) {
			Vector3f rd = lens_radius * random_in_unit_disk();
			Vector3f offset = u * rd.x() + v * rd.y();
			return Ray(origin + offset,
				lower_left_corner + s * horizontal + t * vertical
				- origin - offset);
		}

		Vector3f origin;
		Vector3f lower_left_corner;
		Vector3f horizontal;
		Vector3f vertical;
		Vector3f u, v, w;
		float lens_radius;
	};
}
