#pragma once
#include <random>
#include "Eigen/Core"
using namespace Eigen;
const float PI = 3.141592657f;

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator =
		std::bind(distribution, generator);
	return rand_generator();
}
inline Vector3f random_cosine_direction() {
	float r1 = random_double();
	float r2 = random_double();
	float z = sqrt(1 - r2);
	float phi = 2 * PI * r1;
	float x = cos(phi) * sqrt(r2);
	float y = sin(phi) * sqrt(r2);
	return Vector3f(x, y, z);
}
Vector3f random_in_unit_sphere() {
	Vector3f p;
	do {
		p = 2.0 * Vector3f(random_double(), random_double(), random_double()) - Vector3f(1, 1, 1);
	} while (p.norm() >= 1.0);
	return p.normalized();
}

Vector3f reflect(const Vector3f& v, const Vector3f& n) {
	return v - 2 * v.dot(n) * n;
}

bool refract(const Vector3f& v, const Vector3f& n, float ni_over_nt, Vector3f& refracted) {
	Vector3f uv = v.normalized();
	float dt = uv.dot(n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - dt * n) - sqrt(discriminant) * n;
		return true;
	}
	return false;
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }