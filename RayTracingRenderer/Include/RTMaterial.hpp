#pragma once
#include "Eigen/Core"
#include "Ray.hpp"

#include "Utils.hpp"
using namespace Eigen;

namespace Entropy {
	class RTMaterial {
	public:
		virtual bool scatter(
			const Ray& r_in, const HitRecord& rec, Vector3f& attenuation,
			Ray& scattered) const = 0;
	};

	class Lambertian : public RTMaterial {
	public:
		Lambertian(const Vector3f& a) : albedo(a) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec,
			Vector3f& attenuation, Ray& scattered) const {
			Vector3f target = rec.p + rec.normal + random_in_unit_sphere();
			scattered = Ray(rec.p, target - rec.p);
			attenuation = albedo;
			return true;
		}

		Vector3f albedo;
	};


	class Metal : public RTMaterial {
	public:
		Metal(const Vector3f& a) : albedo(a) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec,
			Vector3f& attenuation, Ray& scattered) const {
			Vector3f reflected = reflect(r_in.direction().normalized(), rec.normal);
			scattered = Ray(rec.p, reflected);
			attenuation = albedo;
			return scattered.direction().dot(rec.normal) > 0;
		}
		Vector3f albedo;
	};
	
}
