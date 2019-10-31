#pragma once
#include <random>
#include "Eigen/Core"
using namespace Eigen;

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> rand_generator =
		std::bind(distribution, generator);
	return rand_generator();
}

Vector3f random_in_unit_sphere() {
	Vector3f p;
	do {
		p = 2.0 * Vector3f(random_double(), random_double(), random_double()) - Vector3f(1, 1, 1);
	} while (p.norm() >= 1.0);
	return p;
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