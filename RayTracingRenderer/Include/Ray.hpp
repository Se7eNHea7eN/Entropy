#pragma once
#include "Utils.hpp"

#include "Eigen/Core"
#include <iostream>
using namespace Eigen;

namespace Entropy {
	class RTMaterial;
	class Ray {
	public:
		Ray() {
		}

		Ray(const Vector3f& a, const Vector3f& b, float ti = 0.0) {
			A = a;
			B = b;
			_time = ti;
		}

		Vector3f origin() const { return A; }
		Vector3f direction() const { return B; }
		Vector3f point_at_parameter(float t) const { return A + t * B; }
		float time() const { return _time; }
		Vector3f A;
		Vector3f B;
		float _time;
	};



	struct HitRecord {
		float t;
		Vector3f p;
		Vector3f normal;
		RTMaterial* mat_ptr;
	};


	class AABB {
	public:
		AABB() {}
		AABB(const Vector3f& a, const Vector3f& b) { _min = a; _max = b; }

		Vector3f Min() const { return _min; }
		Vector3f Max() const { return _max; }

		bool hit(const Ray& r, float tmin, float tmax) const {
			for (int a = 0; a < 3; a++) {
				float invD = 1.0f / r.direction()[a];
				float t0 = (Min()[a] - r.origin()[a]) * invD;
				float t1 = (Max()[a] - r.origin()[a]) * invD;
				if (invD < 0.0f)
					std::swap(t0, t1);
				tmin = t0 > tmin ? t0 : tmin;
				tmax = t1 < tmax ? t1 : tmax;
				if (tmax <= tmin)
					return false;
			}
			return true;
		}

		Vector3f _min;
		Vector3f _max;
	};

	AABB surrounding_box(AABB box0, AABB box1) {
		Vector3f small_(ffmin(box0.Min().x(), box1.Min().x()),
			ffmin(box0.Min().y(), box1.Min().y()),
			ffmin(box0.Min().z(), box1.Min().z()));
		Vector3f big(ffmax(box0.Max().x(), box1.Max().x()),
			ffmax(box0.Max().y(), box1.Max().y()),
			ffmax(box0.Max().z(), box1.Max().z()));
		return AABB(small_, big);


	}
	class Hittable {
	public:
		virtual bool hit(
			const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
		virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
	};


	class Sphere : public Hittable {
	public:
		Sphere() {
		}

		Sphere(Vector3f cen, float r, RTMaterial* m) : center(cen), radius(r), mat_ptr(m) {
		};
		virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const;
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

	inline bool Sphere::bounding_box(float t0, float t1, AABB& box) const
	{
		box = AABB(center - Vector3f(radius, radius, radius),
			center + Vector3f(radius, radius, radius));
		return true;
	}

	class MovingSphere : public Hittable {
	public:
		MovingSphere() {}
		MovingSphere(Vector3f cen0, Vector3f cen1, float t0, float t1, float r, RTMaterial* m)
			: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
		{};
		virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const;
		Vector3f center(float time) const;
		Vector3f center0, center1;
		float time0, time1;
		float radius;
		RTMaterial* mat_ptr;
	};

	Vector3f MovingSphere::center(float time) const {
		return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
	}

	bool MovingSphere::hit(
		const Ray& r, float t_min, float t_max, HitRecord& rec) const {
		
		Vector3f oc = r.origin() - center(r.time());
		float a = r.direction().squaredNorm();
		float b = oc.dot(r.direction());
		float c = oc.squaredNorm() - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0) {
			float temp = (-b - sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center(r.time())) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
			temp = (-b + sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center(r.time())) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}
		return false;
	}

	inline bool MovingSphere::bounding_box(float t0, float t1, AABB& box) const
	{
		AABB box0(center(t0) - Vector3f(radius, radius, radius),
			center(t0) + Vector3f(radius, radius, radius));
		AABB box1(center(t1) - Vector3f(radius, radius, radius),
			center(t1) + Vector3f(radius, radius, radius));
		box = surrounding_box(box0, box1);
		return true;
	}

	class HittableList : public Hittable {
	public:
		HittableList() {}
		HittableList(Hittable** l, int n) { list = l; list_size = n; }
		virtual bool bounding_box(float t0, float t1, AABB& box) const;

		virtual bool hit(
			const Ray& r, float tmin, float tmax, HitRecord& rec) const;
		Hittable** list;
		int list_size;
	};

	inline bool HittableList::bounding_box(float t0, float t1, AABB& box) const
	{
		if (list_size < 1) return false;
		AABB temp_box;
		bool first_true = list[0]->bounding_box(t0, t1, temp_box);
		if (!first_true)
			return false;
		else
			box = temp_box;
		for (int i = 1; i < list_size; i++) {
			if (list[i]->bounding_box(t0, t1, temp_box)) {
				box = surrounding_box(box, temp_box);
			}
			else
				return false;
		}
		return true;
	}

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
