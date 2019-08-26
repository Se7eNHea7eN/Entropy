#include "Camera.hpp"
#include <Eigen/Geometry>
using namespace Entropy;

Eigen::Matrix4f Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float const tanHalfFovy = tan(fovy / 2);

	Eigen::Matrix4f Result = Eigen::Matrix4f::Zero();
	Result(0, 0) = 1 / (aspect * tanHalfFovy);
	Result(1, 1) = 1 / (tanHalfFovy);
	Result(2, 2) = -(zFar + zNear) / (zFar - zNear);
	Result(3, 2) = -1.0;
	Result(2, 3) = -(2 * zFar * zNear) / (zFar - zNear);
	return Result;
}

Eigen::Matrix4f Camera::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Eigen::Matrix4f Result = Eigen::Matrix4f::Identity();
	Result(0, 0) = 2 / (right - left);
	Result(1, 1) = 2 / (top - bottom);
	Result(2, 2) = -2 / (zFar - zNear);
	Result(0, 3) = -(right + left) / (right - left);
	Result(1, 3) = -(top + bottom) / (top - bottom);
	Result(2, 3) = -(zFar + zNear) / (zFar - zNear);
	return Result;
}

Eigen::Matrix4f Camera::lookAt(Eigen::Vector3f const& eye, Eigen::Vector3f const& center, Eigen::Vector3f const& up)
{
	Eigen::Vector3f f(center - eye);
	Eigen::Vector3f s(f.cross(up));
	Eigen::Vector3f u(s.cross(f));
	f.normalize();
	s.normalize();
	u.normalize();

	Eigen::Matrix4f Result = Eigen::Matrix4f::Identity();
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
