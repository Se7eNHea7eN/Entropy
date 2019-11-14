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
		virtual Vector3f emitted(float u, float v, const Vector3f& p) const {
			return Vector3f(0, 0, 0);
		}
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

	class DiffuseLight : public RTMaterial {
	public:
		DiffuseLight(RTTexture* a) : emit(a) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec,
			Vector3f& attenuation, Ray& scattered) const {
			return false;
		}
		virtual Vector3f emitted(float u, float v, const Vector3f& p) const {
			return emit->value(u, v, p);
		}
		RTTexture* emit;
	};

	class isotropic : public RTMaterial {
	public:
		isotropic(RTTexture* a) : albedo(a) {}
		virtual bool scatter(
			const Ray& r_in,
			const HitRecord& rec,
			Vector3f& attenuation,
			Ray& scattered) const {

			scattered = Ray(rec.p, random_in_unit_sphere());
			attenuation = albedo->value(rec.u, rec.v, rec.p);
			return true;
		}
		RTTexture* albedo;
	};

	class constant_medium : public Hittable {
	public:
		constant_medium(Hittable* b, float d, RTTexture* a) : boundary(b), density(d) {
			phase_function = new isotropic(a);
		}
		virtual bool hit(
			const Ray& r, float t_min, float t_max, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			return boundary->bounding_box(t0, t1, box);
		}
		Hittable* boundary;
		float density;
		RTMaterial* phase_function;
	};

	bool constant_medium::hit(const Ray& r, float t_min, float t_max, HitRecord& rec)
		const {

		// Print occasional samples when debugging. To enable, set enableDebug true.
		const bool enableDebug = false;
		bool debugging = enableDebug && random_double() < 0.00001;

		HitRecord rec1, rec2;

		if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {
			if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {

				if (debugging) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << '\n';

				if (rec1.t < t_min)
					rec1.t = t_min;

				if (rec2.t > t_max)
					rec2.t = t_max;

				if (rec1.t >= rec2.t)
					return false;

				if (rec1.t < 0)
					rec1.t = 0;

				float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().norm();
				float hit_distance = -(1 / density) * log(random_double());

				if (hit_distance < distance_inside_boundary) {

					rec.t = rec1.t + hit_distance / r.direction().norm();
					rec.p = r.point_at_parameter(rec.t);

					if (debugging) {
						std::cerr << "hit_distance = " << hit_distance << '\n'
							<< "rec.t = " << rec.t << '\n'
							<< "rec.p = " << rec.p << '\n';
					}

					rec.normal = Vector3f(1, 0, 0);  // arbitrary
					rec.mat_ptr = phase_function;
					return true;
				}
			}
		}
		return false;
	}
}
