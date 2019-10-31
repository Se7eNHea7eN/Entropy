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