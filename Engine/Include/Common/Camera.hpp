#pragma once
#include <Eigen/Core>
#include "Common/SceneObject.hpp"
#include "SceneNode.hpp"

namespace Entropy {
	using namespace Eigen;

	class SceneObjectCamera : public BaseSceneNode {
	public:
		Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
		Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		Matrix4f lookAt(Eigen::Vector3f const& eye, Eigen::Vector3f const& center, Eigen::Vector3f const& up);
	};
}
