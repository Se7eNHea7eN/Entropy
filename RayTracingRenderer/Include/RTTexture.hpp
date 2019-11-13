#pragma once
#include "Eigen/Core"
using namespace Eigen;

namespace Entropy {
	class RTTexture{
	public:
		virtual Vector3f value(float u, float v, const Vector3f& p) const = 0 ;
	};

	class ConstantTexture : public RTTexture {
	public:
		ConstantTexture() {}
		ConstantTexture(Vector3f c) : color(c) {}
		virtual Vector3f value(float u, float v, const Vector3f& p) const {
			return color;
		}
		Vector3f color;
	};

	class CheckerTexture : public RTTexture {
	public:
		CheckerTexture() {}
		CheckerTexture(RTTexture* t0, RTTexture* t1) : even(t0), odd(t1) {}
		virtual Vector3f value(float u, float v, const Vector3f& p) const {
			float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
			if (sines < 0)
				return odd->value(u, v, p);
			else
				return even->value(u, v, p);
		}
		RTTexture* odd;
		RTTexture* even;
	};
}