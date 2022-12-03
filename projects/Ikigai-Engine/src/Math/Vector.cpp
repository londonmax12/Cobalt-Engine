#include "ikipch.h"
#include "Vector.h"

namespace Ikigai {
	Vector2::Vector2()
	{
		x = 0.f;
		y = 0.f;
	}
	Vector2::Vector2(float scalar)
	{
		x = scalar;
		y = scalar;
	}


	Vector2::Vector2(float X, float Y)
	{
		x = X;
		y = Y;
	}

	Vector2 Vector2::Zero()
	{
		return Vector2(0.f);
	}

	Vector2 Vector2::One()
	{
		return Vector2(1.f);
	}

	Vector3::Vector3()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	Vector3::Vector3(float scalar)
	{
		x = scalar;
		y = scalar;
		z = scalar;
	}

	Vector3::Vector3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector3 Vector3::Zero()
	{
		return Vector3(0.f);
	}

	Vector3 Vector3::One()
	{
		return Vector3(1.f);
	}

	Vector4::Vector4()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	}

	Vector4::Vector4(float scalar)
	{
		x = scalar;
		y = scalar;
		z = scalar;
		w = scalar;
	}

	Vector4::Vector4(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	Vector4 Vector4::Zero()
	{
		return Vector4(0.f);
	}

	Vector4 Vector4::One()
	{
		return Vector4(1.f);
	}

	QuaternionAngle QuaternionAngle::Conjugate()
	{
		return QuaternionAngle(
			x * -1,
			y * -1,
			z * -1,
			w
		);
	}
	QuaternionAngle QuaternionAngle::Inverse()
	{
		QuaternionAngle newAngle{ x, y, z, w };
		newAngle.Conjugate().Normalize();
		return newAngle;
	}
	QuaternionAngle QuaternionAngle::Multiply(QuaternionAngle a, QuaternionAngle b)
	{
		QuaternionAngle out;

		out.x = a.x * b.w + a.y + b.z - a.z * b.y + a.w * b.x;
		out.y = -a.x * b.z + a.y + b.w - a.z * b.x + a.w * b.y;
		out.z = a.x * b.y + a.y + b.x - a.z * b.w + a.w * b.z;
		out.w = -a.x * b.x + a.y + b.y - a.z * b.z + a.w * b.w;

		return out;
	}
	QuaternionAngle QuaternionAngle::Slerp(QuaternionAngle a, QuaternionAngle b, float percent)
	{
		QuaternionAngle out;

		QuaternionAngle q1 = a.Normalize();
		QuaternionAngle q2 = b.Normalize();

		float dot = q1.Dot(q2);

		if (dot < 0.0f) {
			q1.x = -q1.x;
			q1.y = -q1.y;
			q1.z = -q1.z;
			q1.w = -q1.w;

			dot = -dot;
		}

		if (dot > 0.9995f) {
			out = QuaternionAngle((q1.x + (q2.x - q1.x) * percent), (q1.y + (q2.y - q1.y) * percent), (q1.z + (q2.z - q1.z) * percent), (q1.w + (q2.w - q1.w) * percent));
			return out.Normalize();
		}
		
		float theta0 = acos(dot);
		float theta = theta0 * percent;
		float sinTheta = sin(theta);
		float sinTheta0 = sin(theta0);

		float s0 = cos(theta) - dot * sinTheta / sinTheta0;
		float s1 = sinTheta / sinTheta0;

		return QuaternionAngle((q1.x * s0) + (q2.x * s1), (q1.y * s0) + (q2.y * s1), (q1.z * s0) + (q2.z * s1), (q1.w * s0) + (q2.w * s1));
	}
	QuaternionAngle QuaternionAngle::AxisAngle(Vector3 axis, float angle)
	{
		float hAngle = angle * 0.5f;
		float sine = sin(angle);
		float cosine = cos(angle);

		return { sine * axis.x, sine * axis.y, sine * axis.z, cosine };
	}
}