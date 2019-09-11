#pragma once
#include <Eigen/Core>
#include "SceneNode.hpp"

namespace Entropy {
	using namespace Eigen;

	class Camera : public BaseSceneNode {
	public:
		Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
		Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		Matrix4f lookAt(Vector3f const& eye, Vector3f const& center, Vector3f const& up);
	};
}
