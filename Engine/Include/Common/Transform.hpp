#pragma once
#include <Eigen/Core>
namespace Entropy {
	using namespace Eigen;

	class Transform
	{
	protected:
		Matrix4f m_matrix;

	public:
		// SceneObjectTransform() { BuildIdentityMatrix(m_matrix); m_bSceneObjectOnly = false; };
		//
		// SceneObjectTransform(const Matrix4f& matrix, const bool object_only = false) { m_matrix = matrix; m_bSceneObjectOnly = object_only; };

		friend std::ostream& operator<<(std::ostream& out, const Transform& obj);
	};
}