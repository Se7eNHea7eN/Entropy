#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Entropy {
	using namespace Eigen;

	class Transform
	{
	protected:
		Isometry3d m_isometry = Isometry3d::Identity();
	public:
		// SceneObjectTransform() { BuildIdentityMatrix(m_matrix); m_bSceneObjectOnly = false; };
		//
		// SceneObjectTransform(const Matrix4f& matrix, const bool object_only = false) { m_matrix = matrix; m_bSceneObjectOnly = object_only; };

		friend std::ostream& operator<<(std::ostream& out, const Transform& obj);
		Isometry3d Isometry() { return m_isometry; }
		
		Vector3f Position() {
			return m_isometry.translation;
		}

		Vector3f Scale() {
			return m_isometry.scale;
		}
		
	};
}
