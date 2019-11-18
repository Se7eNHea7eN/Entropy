#pragma once
#include "RTTexture.hpp"
#include "Eigen/Core"
#include "Ray.hpp"

#include "Utils.hpp"
using namespace Eigen;
//const float PI = 3.141592657f;

namespace Entropy {

	class pdf {
	public:
		virtual float value(const Vector3f& direction) const = 0;
		virtual Vector3f generate() const = 0;
	};

	class cosine_pdf : public pdf {
	public:
		cosine_pdf(const Vector3f& w) { uvw.build_from_w(w); }
		virtual float value(const Vector3f& direction) const {
			float cosine = direction.normalized().dot(uvw.w());
			if (cosine > 0)
				return cosine / PI;
			else
				return 0;
		}
		virtual Vector3f generate() const {
			return uvw.local(random_cosine_direction());
		}
		onb uvw;

	};

	class hittable_pdf : public pdf {
	public:
		hittable_pdf(Hittable* p, const Vector3f& origin) : ptr(p), o(origin) {}
		virtual float value(const Vector3f& direction) const {
			return ptr->pdf_value(o, direction);
		}
		virtual Vector3f generate() const {
			return ptr->random(o);
		}
		Vector3f o;
		Hittable* ptr;
	};
	class mixture_pdf : public pdf {
	public:
		mixture_pdf(pdf* p0, pdf* p1) { p[0] = p0; p[1] = p1; }
		virtual float value(const Vector3f& direction) const {
			return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
		}
		virtual Vector3f generate() const {
			if (random_double() < 0.5)
				return p[0]->generate();
			else
				return p[1]->generate();
		}
		pdf* p[2];
	};


	class RTMaterial {
	public:
		virtual bool scatter(
			const Ray& r_in,
			const HitRecord& hrec, scatter_record& srec) const {
			return false;
		}

		virtual float scattering_pdf(const Ray& r_in, const HitRecord& rec,
			const Ray& scattered) const {
			return 0;
		}

		virtual Vector3f emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vector3f& p) const {
			return Vector3f(0, 0, 0);
		}
	};

	class Lambertian : public RTMaterial {
	public:
		Lambertian(RTTexture* a) : albedo(a) {}
		virtual bool scatter(
			const Ray& r_in,
			const HitRecord& hrec, scatter_record& srec) const {
			srec.is_specular = false;
			srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
			srec.pdf_ptr = new cosine_pdf(hrec.normal);
			return true;
		}
		float scattering_pdf(const Ray& r_in,
			const HitRecord& rec, const Ray& scattered) const {
			float cosine = rec.normal.dot(scattered.direction().normalized());
			if (cosine < 0)
				return 0;
			return cosine / PI;
		}

		RTTexture* albedo;
	};


	class Metal : public RTMaterial {
	public:
		Metal(const Vector3f& a, float f) : albedo(a) {
			if (f < 1) fuzz = f; else fuzz = 1;
		}
		virtual bool scatter(
			const Ray& r_in,
			const HitRecord& hrec, scatter_record& srec) const {
			Vector3f reflected = reflect(r_in.direction().normalized(), hrec.normal);
			srec.specular_ray = Ray(hrec.p, reflected + fuzz * random_in_unit_sphere());
			srec.attenuation = albedo;
			srec.is_specular = true;
			srec.pdf_ptr = nullptr;
			return true;
		}
		Vector3f albedo;
		float fuzz;
	};


	class Dielectric : public RTMaterial {
	public:
		Dielectric(float ri) : ref_idx(ri) {}
		virtual bool scatter(const Ray& r_in,
			const HitRecord& hrec, scatter_record& srec) const {
			srec.is_specular = true;
			srec.pdf_ptr = 0;
			srec.attenuation = Vector3f(1.0, 1.0, 1.0);
			Vector3f outward_normal;
			Vector3f reflected = reflect(r_in.direction(), hrec.normal);
			Vector3f refracted;
			float ni_over_nt;
			float reflect_prob;
			float cosine;
			if (r_in.direction().dot(hrec.normal) > 0) {
				outward_normal = -hrec.normal;
				ni_over_nt = ref_idx;
				cosine = ref_idx * r_in.direction().dot(hrec.normal) / r_in.direction().norm();
			}
			else {
				outward_normal = hrec.normal;
				ni_over_nt = 1.0 / ref_idx;
				cosine = -r_in.direction().dot( hrec.normal) / r_in.direction().norm();
			}
			if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
				reflect_prob = schlick(cosine, ref_idx);
			}
			else {
				reflect_prob = 1.0;
			}
			if (random_double() < reflect_prob) {
				srec.specular_ray = Ray(hrec.p, reflected);
			}
			else {
				srec.specular_ray = Ray(hrec.p, refracted);
			}
			return true;
		}

		float ref_idx;
	};

	class DiffuseLight : public RTMaterial {
	public:
		DiffuseLight(RTTexture* a) : emit(a) {}

		virtual Vector3f emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Vector3f& p) const {
			if (rec.normal.dot(r_in.direction()) < 0.0)
				return emit->value(u, v, p);
			else
				return Vector3f(0, 0, 0);
		}
		RTTexture* emit;
	};

	class isotropic : public RTMaterial {
	public:
		isotropic(RTTexture* a) : albedo(a) {}
		//virtual bool scatter(
		//	const Ray& r_in,

		//	const HitRecord& rec,

		//	Vector3f& attenuation,
		//	Ray& scattered, float& pdf) const {

		//	scattered = Ray(rec.p, random_in_unit_sphere());
		//	attenuation = albedo->value(rec.u, rec.v, rec.p);
		//	return true;
		//}
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
