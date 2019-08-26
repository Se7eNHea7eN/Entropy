#pragma once
#include <Eigen/Core>
namespace Entropy {
	class Camera {
	public:
		Eigen::Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
		Eigen::Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		Eigen::Matrix4f lookAt(Eigen::Vector3f const& eye, Eigen::Vector3f const& center, Eigen::Vector3f const& up);
	};
}