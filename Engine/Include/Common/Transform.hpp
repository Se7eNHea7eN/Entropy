#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Entropy {
	using namespace Eigen;

	class Transform
	{
	protected:

		Quaternionf orientation;
		Vector3f position;
		Vector3f scale = Vector3f(1,1,1);
	public:

		friend std::ostream& operator<<(std::ostream& out, const Transform& obj);

		Matrix4f ModelMatrix() {
			return  Affine3f(Translation3f(position) * orientation * Scaling(scale)).matrix();
		}
		
		void SetPosition(const Vector3f& pos) { position = pos; }
		inline const Vector3f& Position(void) const { return position; }

		void SetOrientation(const Quaternionf& q) { orientation = q; }
		inline const Quaternionf& Orientation(void) const { return orientation; }

		void SetScale(const Vector3f& scl) { scale = scl; }

		inline const Vector3f Scale() const {
			return scale;
		}
		void SetDirection(const Vector3f& newDirection){
			Vector3f up = this->Up();

			Matrix3f camAxes;

			camAxes.col(2) = (-newDirection).normalized();
			camAxes.col(0) = up.cross(camAxes.col(2)).normalized();
			camAxes.col(1) = camAxes.col(2).cross(camAxes.col(0)).normalized();
			SetOrientation(Quaternionf(camAxes));
		}
		Vector3f Direction() const {
			return -(Orientation() * Vector3f::UnitZ());
		}
		
		Vector3f Up() const {
			return Orientation() * Vector3f::UnitY();
		}
		
		Vector3f Right() const{
			return Orientation() * Vector3f::UnitX();
		}

		Vector3f Translate(Vector3f t) {
			position += t;
			return position;
		}

		Quaternionf Rotate(const Quaternionf& q) {
			SetOrientation(Orientation() * q);
		}

		
	};
}
