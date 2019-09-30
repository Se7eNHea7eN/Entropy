#pragma once
#include <Eigen/Geometry>
#include "Component.hpp"
typedef unsigned int uint;
namespace Entropy {
	using namespace Eigen;
	class Transform;
	
	class Camera : public Component {
	public:
		Matrix4f perspective(float fovy, float aspect, float zNear, float zFar);
		Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		Matrix4f lookAt(Vector3f const& eye, Vector3f const& center, Vector3f const& up);

		void SetViewport(uint offsetx, uint offsety, uint width, uint height);
		void SetViewport(uint width, uint height);

		inline uint VpX(void) const { return mVpX; }
		inline uint VpY(void) const { return mVpY; }
		inline uint VpWidth(void) const { return mVpWidth; }
		inline uint VpHeight(void) const { return mVpHeight; }

		inline float FovY(void) const { return mFovY; }
		void SetFovY(float value);
		inline float NearDistance(void)const { return mNearDist; };
		inline float FarDistance(void)const { return mFarDist; };
		
		void SetTarget(const std::shared_ptr<Transform>  target);
		inline const const std::shared_ptr<Transform> target(void) const { return mTarget; }

		const Eigen::Affine3f& ViewMatrix(void) const;
		const Eigen::Matrix4f& ProjectionMatrix(void) const;

		void RotateAroundTarget(const Eigen::Quaternionf& q);
		void Zoom(float d);


		Eigen::Vector3f unProject(const Eigen::Vector2f& uv, float depth, const Eigen::Matrix4f& invModelview) const;
		Eigen::Vector3f unProject(const Eigen::Vector2f& uv, float depth) const;

	protected:
		void updateViewMatrix(void) const;
		void updateProjectionMatrix(void) const;

	protected:

		uint mVpX, mVpY;
		uint mVpWidth, mVpHeight;

		mutable Eigen::Affine3f mViewMatrix;
		mutable Eigen::Matrix4f mProjectionMatrix;

		mutable bool mViewIsUptodate;
		mutable bool mProjIsUptodate;

		// used by rotateAroundTarget
		std::shared_ptr<Transform> mTarget;

		float mFovY = 1.0471975511965977461542144610932f;
		float mNearDist = 0.1f;
		float mFarDist = 1000.f;
	};
}
