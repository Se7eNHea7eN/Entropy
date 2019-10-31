#pragma once
#include "Eigen/Core"
using namespace Eigen;

namespace Entropy {
	class RTMaterial;
	class Ray {
	public:
		Ray() {
		}

		Ray(const Vector3f& a, const Vector3f& b) {
			A = a;
			B = b;
		}

		Vector3f origin() const { return A; }
		Vector3f direction() const { return B; }
		Vector3f point_at_parameter(float t) const { return A + t * B; }

		Vector3f A;
		Vector3f B;
	};

	struct HitRecord {
		float t;
		Vector3f p;
		Vector3f normal;
		RTMaterial* mat_ptr;
	};

	class Hittable {
	public:
		virtual bool hit(
			const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	};


	class Sphere : public Hittable {
	public:
		Sphere() {
		}

		Sphere(Vector3f cen, float r, RTMaterial* m) : center(cen), radius(r), mat_ptr(m) {
		};
		virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
		Vector3f center;
		float radius;
		RTMaterial* mat_ptr;

	};

	bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
		Vector3f oc = r.origin() - center;
		float a = r.direction().dot(r.direction());
		float b = oc.dot(r.direction());
		float c = oc.dot(oc) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0) {
			float temp = (-b - sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
			temp = (-b + sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}
		return false;
	}


	class HittableList : public Hittable {
	public:
		HittableList() {}
		HittableList(Hittable** l, int n) { list = l; list_size = n; }
		virtual bool hit(
			const Ray& r, float tmin, float tmax, HitRecord& rec) const;
		Hittable** list;
		int list_size;
	};

	bool HittableList::hit(const Ray& r, float t_min, float t_max,
		HitRecord& rec) const {

		HitRecord temp_rec;
		bool hit_anything = false;
		double closest_so_far = t_max;
		for (int i = 0; i < list_size; i++) {
			if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
	
}
