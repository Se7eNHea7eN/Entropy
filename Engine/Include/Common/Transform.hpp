#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Entropy {
	using namespace Eigen;

	class Transform
	{
	protected:
		Isometry3f m_isometry = Isometry3f::Identity();
	public:
		// SceneObjectTransform() { BuildIdentityMatrix(m_matrix); m_bSceneObjectOnly = false; };
		//
		// SceneObjectTransform(const Matrix4f& matrix, const bool object_only = false) { m_matrix = matrix; m_bSceneObjectOnly = object_only; };

		friend std::ostream& operator<<(std::ostream& out, const Transform& obj);
		Isometry3f Isometry() { return m_isometry; }
		
		Vector3f Position() {
			return m_isometry.translation();
		}
		
		Vector3f Forward() {
			return m_isometry.rotation() * Vector3f(0, 0, 1);
		}

		Vector3f Up() {
			return m_isometry.rotation() * Vector3f(0, 1, 0);
		}
		// Vector3f Scale() {
		// 	return m_isometry.scale();
		// }
		
	};
}
