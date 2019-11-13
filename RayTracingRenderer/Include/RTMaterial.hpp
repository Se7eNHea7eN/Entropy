#pragma once
#include "RTTexture.hpp"
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
		Lambertian(RTTexture* a) : albedo(a) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec,
			Vector3f& attenuation, Ray& scattered) const {
			Vector3f target = rec.p + rec.normal + random_in_unit_sphere();
			scattered = Ray(rec.p, target - rec.p, r_in.time());
			attenuation = albedo->value(0, 0, rec.p);
			return true;
		}

		RTTexture* albedo;
	};


	class Metal : public RTMaterial {
	public:
		Metal(const Vector3f& a, float f) : albedo(a) {
			if (f < 1) fuzz = f; else fuzz = 1;
		}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec,
			Vector3f& attenuation, Ray& scattered) const {
			Vector3f reflected = reflect(r_in.direction().normalized(), rec.normal);
			scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
			attenuation = albedo;
			return scattered.direction().dot(rec.normal) > 0;
		}
		Vector3f albedo;
		float fuzz;
	};


	class Dielectric : public RTMaterial {
	public:
		Dielectric(float ri) : ref_idx(ri) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec,
			Vector3f& attenuation, Ray& scattered) const {
			Vector3f outward_normal;
			Vector3f reflected = reflect(r_in.direction(), rec.normal);
			float ni_over_nt;
			attenuation = Vector3f(1.0, 1.0, 1.0);
			Vector3f refracted;

			float reflect_prob;
			float cosine;
			
			if (r_in.direction().dot(rec.normal) > 0) {
				outward_normal = -rec.normal;
				ni_over_nt = ref_idx;
				cosine = ref_idx * r_in.direction().dot(rec.normal)
					/ r_in.direction().norm();
			}
			else {
				outward_normal = rec.normal;
				ni_over_nt = 1.0 / ref_idx;
				cosine = -r_in.direction().dot(rec.normal)
					/ r_in.direction().norm();
			}

			if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
				reflect_prob = schlick(cosine, ref_idx);
			}
			else {
				reflect_prob = 1.0;
			}

			if (random_double() < reflect_prob) {
				scattered = Ray(rec.p, reflected);
			}	
			else {
				scattered = Ray(rec.p, refracted);
			}

			return true;
		}

		float ref_idx;
	};
}
