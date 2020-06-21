#pragma once
#include "Utils.hpp"
#include "Eigen/Core"
#include <iostream>
#include <memory>

using namespace Eigen;
namespace Entropy {
	class RTMaterial;
	class pdf;
	class onb
	{
	public:
		onb() {}
		inline Vector3f operator[](int i) const { return axis[i]; }
		Vector3f u() const { return axis[0]; }
		Vector3f v() const { return axis[1]; }
		Vector3f w() const { return axis[2]; }
		Vector3f local(float a, float b, float c) const { return a * u() + b * v() + c * w(); }
		Vector3f local(const Vector3f& a) const { return a.x() * u() + a.y() * v() + a.z() * w(); }
		void build_from_w(const Vector3f&);
		Vector3f axis[3];
	};

	void onb::build_from_w(const Vector3f& n) {
		axis[2] = n.normalized();
		Vector3f a;
		if (fabs(w().x()) > 0.9)
			a = Vector3f(0, 1, 0);
		else
			a = Vector3f(1, 0, 0);
		axis[1] = w().cross(a).normalized();
		axis[0] = w().cross(v());
	}

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
		float u;
		float v;
	};
	struct scatter_record
	{
		Ray specular_ray;
		bool is_specular;
		Vector3f attenuation;
		std::shared_ptr<pdf> pdf_ptr;
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
		virtual float  pdf_value(const Vector3f& o, const Vector3f& v) const { return 0.0; }
		virtual Vector3f random(const Vector3f& o) const { return Vector3f(1, 0, 0); }
	};

	class translate : public Hittable {
	public:
		translate(Hittable* p, const Vector3f& displacement)
			: ptr(p), offset(displacement) {}
		virtual bool hit(
			const Ray& r, float t_min, float t_max, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const;

		Hittable* ptr;
		Vector3f offset;
	};

	bool translate::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
		Ray moved_r(r.origin() - offset, r.direction(), r.time());
		if (ptr->hit(moved_r, t_min, t_max, rec)) {
			rec.p += offset;
			return true;
		}
		else
			return false;
	}

	bool translate::bounding_box(float t0, float t1, AABB& box) const {
		if (ptr->bounding_box(t0, t1, box)) {
			box = AABB(box.Min() + offset, box.Max() + offset);
			return true;
		}
		else
			return false;
	}

	class rotate_y : public Hittable {
	public:
		rotate_y(Hittable* p, float angle);
		virtual bool hit(
			const Ray& r, float t_min, float t_max, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = bbox; return hasbox;
		}
		Hittable* ptr;
		float sin_theta;
		float cos_theta;
		bool hasbox;
		AABB bbox;
	};
	rotate_y::rotate_y(Hittable* p, float angle) : ptr(p) {
		float radians = (PI / 180.) * angle;
		sin_theta = sin(radians);
		cos_theta = cos(radians);
		hasbox = ptr->bounding_box(0, 1, bbox);
		Vector3f _min(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3f _max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					float x = i * bbox.Max().x() + (1 - i) * bbox.Min().x();
					float y = j * bbox.Max().y() + (1 - j) * bbox.Min().y();
					float z = k * bbox.Max().z() + (1 - k) * bbox.Min().z();
					float newx = cos_theta * x + sin_theta * z;
					float newz = -sin_theta * x + cos_theta * z;
					Vector3f tester(newx, y, newz);
					for (int c = 0; c < 3; c++)
					{
						if (tester[c] > _max[c])
							_max[c] = tester[c];
						if (tester[c] < _min[c])
							_min[c] = tester[c];
					}
				}
			}
		}
		bbox = AABB(_min, _max);
	}
	bool rotate_y::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
		Vector3f origin = r.origin();
		Vector3f direction = r.direction();
		origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
		origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
		direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
		direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];
		Ray rotated_r(origin, direction, r.time());
		if (ptr->hit(rotated_r, t_min, t_max, rec)) {
			Vector3f p = rec.p;
			Vector3f normal = rec.normal;
			p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
			p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
			normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
			normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
			rec.p = p;
			rec.normal = normal;
			return true;
		}
		else
			return false;
	}
	class Sphere : public Hittable {
	public:
		Sphere() {
		}

		Sphere(Vector3f cen, float r, RTMaterial* m) : center(cen), radius(r), mat_ptr(m) {
		};
		virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const;

		virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
		virtual Vector3f random(const Vector3f& o) const;
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

	float Sphere::pdf_value(const Vector3f& o, const Vector3f& v) const {
		HitRecord rec;
		if (this->hit(Ray(o, v), 0.001, FLT_MAX, rec)) {
			float cos_theta_max = sqrt(1 - radius * radius / (center - o).squaredNorm());
			float solid_angle = 2 * PI * (1 - cos_theta_max);
			return  1 / solid_angle;
		}
		else
			return 0;
	}

	Vector3f Sphere::random(const Vector3f& o) const {
		Vector3f direction = center - o;
		float distance_squared = direction.squaredNorm();
		onb uvw;
		uvw.build_from_w(direction);
		return uvw.local(random_to_sphere(radius, distance_squared));
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

	class XYRect : public Hittable {
	public:
		XYRect() {}
		XYRect(float _x0, float _x1, float _y0, float _y1, float _k, RTMaterial* mat)
			: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
		virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(Vector3f(x0, y0, k - 0.0001), Vector3f(x1, y1, k + 0.0001));
			return true;
		}
		RTMaterial* mp;
		float x0, x1, y0, y1, k;
	};

	bool XYRect::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
		float t = (k - r.origin().z()) / r.direction().z();
		if (t < t0 || t > t1)
			return false;
		float x = r.origin().x() + t * r.direction().x();
		float y = r.origin().y() + t * r.direction().y();
		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (y - y0) / (y1 - y0);
		rec.t = t;
		rec.mat_ptr = mp;
		rec.p = r.point_at_parameter(t);
		rec.normal = Vector3f(0, 0, 1);
		return true;
	}


	class XZRect : public Hittable {
	public:
		XZRect() {}
		XZRect(float _x0, float _x1, float _z0, float _z1, float _k, RTMaterial* mat)
			: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
		virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(Vector3f(x0, k - 0.0001, z0), Vector3f(x1, k + 0.0001, z1));
			return true;
		}
		virtual float  pdf_value(const Vector3f& o, const Vector3f& v) const {
			HitRecord rec;
			if (this->hit(Ray(o, v), 0.001, FLT_MAX, rec)) {
				float area = (x1 - x0) * (z1 - z0);
				float distance_squared = rec.t * rec.t * v.squaredNorm();
				float cosine = fabs(v.dot(rec.normal) / v.norm());
				return  distance_squared / (cosine * area);
			}
			else
				return 0;
		}
		virtual Vector3f random(const Vector3f& o) const {
			Vector3f random_point = Vector3f(x0 + random_double() * (x1 - x0), k,
				z0 + random_double() * (z1 - z0));
			return random_point - o;
		}
		RTMaterial* mp;
		float x0, x1, z0, z1, k;
	};

	bool XZRect::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
		float t = (k - r.origin().y()) / r.direction().y();
		if (t < t0 || t > t1)
			return false;
		float x = r.origin().x() + t * r.direction().x();
		float z = r.origin().z() + t * r.direction().z();
		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.mat_ptr = mp;
		rec.p = r.point_at_parameter(t);
		rec.normal = Vector3f(0, 1, 0);
		return true;
	}

	class YZRect : public Hittable {
	public:
		YZRect() {}
		YZRect(float _y0, float _y1, float _z0, float _z1, float _k, RTMaterial* mat)
			: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
		virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(Vector3f(k - 0.0001, y0, z0), Vector3f(k + 0.0001, y1, z1));
			return true;
		}
		RTMaterial* mp;
		float y0, y1, z0, z1, k;
	};

	bool YZRect::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
		float t = (k - r.origin().x()) / r.direction().x();
		if (t < t0 || t > t1)
			return false;
		float y = r.origin().y() + t * r.direction().y();
		float z = r.origin().z() + t * r.direction().z();
		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;
		rec.u = (y - y0) / (y1 - y0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.mat_ptr = mp;
		rec.p = r.point_at_parameter(t);
		rec.normal = Vector3f(1, 0, 0);
		return true;
	}

	class flip_normals : public Hittable {
	public:
		flip_normals(Hittable* p) : ptr(p) {}

		virtual bool hit(
			const Ray& r, float t_min, float t_max, HitRecord& rec) const {

			if (ptr->hit(r, t_min, t_max, rec)) {
				rec.normal = -rec.normal;
				return true;
			}
			else
				return false;
		}

		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			return ptr->bounding_box(t0, t1, box);
		}

		Hittable* ptr;
	};




	class HittableList : public Hittable {
	public:
		HittableList() {}
		HittableList(Hittable** l, int n) { list = l; list_size = n; }
		virtual bool bounding_box(float t0, float t1, AABB& box) const;

		virtual bool hit(
			const Ray& r, float tmin, float tmax, HitRecord& rec) const;


		virtual float pdf_value(const Vector3f& o, const Vector3f& v) const;
		virtual Vector3f random(const Vector3f& o) const;

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

	float HittableList::pdf_value(const Vector3f& o, const Vector3f& v) const {
		float weight = 1.0 / list_size;
		float sum = 0;
		for (int i = 0; i < list_size; i++)
			sum += weight * list[i]->pdf_value(o, v);
		return sum;
	}

	Vector3f HittableList::random(const Vector3f& o) const {
		int index = int(random_double() * list_size);
		return list[index]->random(o);
	}


	class Box : public Hittable {
	public:
		Box() {}
		Box(const Vector3f& p0, const Vector3f& p1, RTMaterial* ptr);
		virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(pmin, pmax);
			return true;
		}
		Vector3f pmin, pmax;
		Hittable* list_ptr;
	};

	Box::Box(const Vector3f& p0, const Vector3f& p1, RTMaterial* ptr) {
		pmin = p0;
		pmax = p1;
		Hittable** list = new Hittable * [6];
		list[0] = new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
		list[1] = new flip_normals(
			new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
		list[2] = new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
		list[3] = new flip_normals(
			new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
		list[4] = new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
		list[5] = new flip_normals(
			new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
		list_ptr = new HittableList(list, 6);
	}

	bool Box::hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
		return list_ptr->hit(r, t0, t1, rec);
	}


}
