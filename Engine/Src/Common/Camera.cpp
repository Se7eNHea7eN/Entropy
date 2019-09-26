#include <Eigen/Geometry>
#include "Common/Camera.hpp"
#include "Common/Transform.hpp"
#include "Common/SceneNode.hpp"
using namespace Entropy;
using namespace Eigen;

Matrix4f Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float const tanHalfFovy = tan(fovy / 2);

	Matrix4f Result = Matrix4f::Zero();
	Result(0, 0) = 1 / (aspect * tanHalfFovy);
	Result(1, 1) = 1 / (tanHalfFovy);
	Result(2, 2) = -(zFar + zNear) / (zFar - zNear);
	Result(3, 2) = -1.0;
	Result(2, 3) = -(2 * zFar * zNear) / (zFar - zNear);
	return Result;
}

Matrix4f Camera::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4f Result = Matrix4f::Identity();
	Result(0, 0) = 2 / (right - left);
	Result(1, 1) = 2 / (top - bottom);
	Result(2, 2) = -2 / (zFar - zNear);
	Result(0, 3) = -(right + left) / (right - left);
	Result(1, 3) = -(top + bottom) / (top - bottom);
	Result(2, 3) = -(zFar + zNear) / (zFar - zNear);
	return Result;
}

Matrix4f Camera::lookAt(Vector3f const& eye, Vector3f const& center, Vector3f const& up)
{
	Vector3f f(center - eye);
	Vector3f s(f.cross(up));
	Vector3f u(s.cross(f));
	f.normalize();
	s.normalize();
	u.normalize();

	Matrix4f Result = Matrix4f::Identity();
	Result(0, 0) = s.x();
	Result(0, 1) = s.y();
	Result(0, 2) = s.z();
	Result(1, 0) = u.x();
	Result(1, 1) = u.y();
	Result(1, 2) = u.z();
	Result(2, 0) = -f.x();
	Result(2, 1) = -f.y();
	Result(2, 2) = -f.z();
	Result(0, 3) = -s.dot(eye);
	Result(1, 3) = -u.dot(eye);
	Result(2, 3) = f.dot(eye);
	return Result;
}

void Camera::SetViewport(uint offsetx, uint offsety, uint width, uint height) {
	mVpX = offsetx;
	mVpY = offsety;
	mVpWidth = width;
	mVpHeight = height;

	mProjIsUptodate = false;
}

void Camera::SetViewport(uint width, uint height) {
	mVpWidth = width;
	mVpHeight = height;

	mProjIsUptodate = false;
}

void Camera::SetFovY(float value) {
	mFovY = value;
	mProjIsUptodate = false;
}

void Camera::SetTarget(const std::shared_ptr<Entropy::Transform> target) {
	mTarget = target;
	if (!mTarget->Position().isApprox( GetNode()->GetTransform() ->Position()))
	{
		Vector3f newDirection = GetNode()->GetTransform()->Position() - mTarget->Position();
		GetNode()->GetTransform()->SetDirection(newDirection.normalized());
	}
}

const Affine3f& Camera::ViewMatrix() const {
	updateViewMatrix();
	return mViewMatrix;
}

const Matrix4f& Camera::ProjectionMatrix() const {
	updateProjectionMatrix();
	return mProjectionMatrix;
}

void Camera::RotateAroundTarget(const Quaternionf& q) {
	Matrix4f mrot, mt, mtm;

	// update the transform matrix
	updateViewMatrix();
	Vector3f t = mViewMatrix * mTarget->Position();

	mViewMatrix = Translation3f(t)
		* q
		* Translation3f(-t)
		* mViewMatrix;

	Quaternionf qa(mViewMatrix.linear());
	qa = qa.conjugate();
	GetNode()->GetTransform()->SetOrientation(qa);
	GetNode()->GetTransform()->SetPosition(-(qa * mViewMatrix.translation()));

	mViewIsUptodate = true;
}


void Camera::Zoom(float d) {
	float dist = (GetNode()->GetTransform()->Position() - mTarget->Position()).norm();
	if (dist > d)
	{
		GetNode()->GetTransform()->SetPosition(GetNode()->GetTransform()->Position() + GetNode()->GetTransform()->Direction() * d);
		mViewIsUptodate = false;
	}
}

void Camera::updateViewMatrix() const {
	if (!mViewIsUptodate)
	{
		Quaternionf q = GetNode()->GetTransform()->Orientation().conjugate();
		mViewMatrix.linear() = q.toRotationMatrix();
		mViewMatrix.translation() = -(mViewMatrix.linear() * GetNode()->GetTransform()->Position());

		mViewIsUptodate = true;
	}
}

void Camera::updateProjectionMatrix() const {
	if (!mProjIsUptodate)
	{
		mProjectionMatrix.setIdentity();
		float aspect = float(mVpWidth) / float(mVpHeight);
		float theta = mFovY * 0.5;
		float range = mFarDist - mNearDist;
		float invtan = 1. / tan(theta);

		mProjectionMatrix(0, 0) = invtan / aspect;
		mProjectionMatrix(1, 1) = invtan;
		mProjectionMatrix(2, 2) = -(mNearDist + mFarDist) / range;
		mProjectionMatrix(3, 2) = -1;
		mProjectionMatrix(2, 3) = -2 * mNearDist * mFarDist / range;
		mProjectionMatrix(3, 3) = 0;

		mProjIsUptodate = true;
	}
}


