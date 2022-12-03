#include "ikipch.h"

#include "Matrix.h"

namespace Ikigai {
	Matrix4x4 Matrix4x4::OrthographicMatrix(float left, float right, float bottom, float top, float nearClip, float farClip)
	{
		Matrix4x4 newMatrix;

		float lr = 1.f / (left / right);
		float bt = 1.f / (bottom / top);
		float nf = 1.f / (nearClip / farClip);

		newMatrix.data[0] = 2.f * lr;
		newMatrix.data[5] = 2.f * bt;
		newMatrix.data[10] = 2.f * nf;

		newMatrix.data[12] = (left + right) * lr;
		newMatrix.data[13] = (top + bottom) * bt;
		newMatrix.data[14] = (nearClip + farClip) * nf;
		return newMatrix;
	}
	Matrix4x4 Matrix4x4::PerspectiveMatrix(float fovRadians, float aspectRatio, float nearClip, float farClip)
	{
		float halfTanFov = tan(fovRadians * 0.5f);
		Matrix4x4 newMatrix = Matrix4x4(true);
		
		newMatrix.data[0] = 1.f / (aspectRatio * halfTanFov);
		newMatrix.data[5] = 1.f / halfTanFov;
		newMatrix.data[10] = -((farClip + nearClip) / (farClip - nearClip));
		newMatrix.data[11] = -1.f;
		newMatrix.data[14] = -((2.f * farClip * nearClip) / (farClip - nearClip));
		return newMatrix;
	}
	Matrix4x4 Matrix4x4::LookAt(Vector3 position, Vector3 target, Vector3 up)
	{
		Matrix4x4 newMatrix = Matrix4x4(true);\

		Vector3 x, y, z;
		z.x = target.x - position.x;
		z.y = target.y - position.y;
		z.z = target.z - position.z;
		z = z.Normalize();

		x = z.Cross(up).Normalize();
		y = x.Cross(z);
		
		newMatrix.data[0] = x.x;
		newMatrix.data[1] = y.x;
		newMatrix.data[2] = -z.x;
		newMatrix.data[3] = 0.f;
		newMatrix.data[4] = x.y;
		newMatrix.data[5] = y.y;
		newMatrix.data[6] = -z.y;
		newMatrix.data[7] = 0.f;
		newMatrix.data[8] = x.z;
		newMatrix.data[9] = y.z;
		newMatrix.data[10] = -z.z;
		newMatrix.data[11] = 0.f;
		newMatrix.data[12] = -x.Dot(position);
		newMatrix.data[13] = -y.Dot(position);
		newMatrix.data[14] = z.Dot(position);
		newMatrix.data[15] = 1.f;

		return newMatrix;
	}
	Matrix4x4 Matrix4x4::Inverse(Matrix4x4 matrix)
	{
		const float* m = matrix.data;

		float t0 = m[10] * m[15];
		float t1 = m[14] * m[11];
		float t2 = m[6] * m[15];
		float t3 = m[14] * m[7];
		float t4 = m[6] * m[11];
		float t5 = m[10] * m[7];
		float t6 = m[2] * m[15];
		float t7 = m[14] * m[3];
		float t8 = m[2] * m[11];
		float t9 = m[10] * m[3];
		float t10 = m[2] * m[7];
		float t11 = m[6] * m[3];
		float t12 = m[8] * m[13];
		float t13 = m[12] * m[9];
		float t14 = m[4] * m[13];
		float t15 = m[12] * m[5];
		float t16 = m[4] * m[9];
		float t17 = m[8] * m[5];
		float t18 = m[0] * m[13];
		float t19 = m[12] * m[1];
		float t20 = m[0] * m[9];
		float t21 = m[8] * m[1];
		float t22 = m[0] * m[5];
		float t23 = m[4] * m[1];
		
		Matrix4x4 retMat = Matrix4x4(false);
		float* o = retMat.data;

		o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
		o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
		o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
		o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

		float d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

		o[0] = d * o[0];
		o[1] = d * o[1];
		o[2] = d * o[2];
		o[3] = d * o[3];
		o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
		o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
		o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
		o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
		o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
		o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
		o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
		o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
		o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
		o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
		o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
		o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));
		
		return retMat;
	}
	Matrix4x4 Matrix4x4::Transpose(Matrix4x4 matrix)
	{
		Matrix4x4 newMatrix;

		newMatrix.data[0] = newMatrix.data[0];
		newMatrix.data[1] = newMatrix.data[4];
		newMatrix.data[2] = newMatrix.data[8];
		newMatrix.data[3] = newMatrix.data[12];
		newMatrix.data[4] = newMatrix.data[1];
		newMatrix.data[5] = newMatrix.data[5];
		newMatrix.data[6] = newMatrix.data[9];
		newMatrix.data[7] = newMatrix.data[13];
		newMatrix.data[8] = newMatrix.data[2];
		newMatrix.data[9] = newMatrix.data[6];
		newMatrix.data[10] = newMatrix.data[10];
		newMatrix.data[11] = newMatrix.data[14];
		newMatrix.data[12] = newMatrix.data[3];
		newMatrix.data[13] = newMatrix.data[7];
		newMatrix.data[14] = newMatrix.data[11];
		newMatrix.data[15] = newMatrix.data[15];

		return newMatrix;
	}
	Matrix4x4 Matrix4x4::Translation(Vector3 position)
	{
		Matrix4x4 newMatrix;
		newMatrix.data[12] = position.x;
		newMatrix.data[13] = position.y;
		newMatrix.data[14] = position.z;
		return newMatrix;
	}
	Matrix4x4 Matrix4x4::Scale(Vector3 scale)
	{
		Matrix4x4 newMatrix;
		newMatrix.data[0] = scale.x;
		newMatrix.data[5] = scale.y;
		newMatrix.data[10] = scale.z;
		return newMatrix;
	}
	Matrix4x4 Matrix4x4::Rotate(EulerAngle rotationRadians)
	{
		Matrix4x4 x = Matrix4x4::RotateEulerX(rotationRadians.x);
		Matrix4x4 y = Matrix4x4::RotateEulerX(rotationRadians.y);
		Matrix4x4 z = Matrix4x4::RotateEulerX(rotationRadians.z);
		
		Matrix4x4 out = Matrix4x4::Multiply(x, y);
		out = out * z;

		return out;
	}
	Matrix4x4 Matrix4x4::Rotate(QuaternionAngle rotation)
	{
		Matrix4x4 out;

		QuaternionAngle n = rotation.Normalize();

		out.data[0] = 1.0f - 2.0f * n.y * n.y - 2.0f * n.z * n.z;
		out.data[1] = 2.0f * n.x * n.y - 2.0f * n.z * n.w;
		out.data[2] = 2.0f * n.x * n.z + 2.0f * n.y * n.w;

		out.data[4] = 2.0f * n.x * n.y + 2.0f * n.z * n.w;
		out.data[5] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.z * n.z;
		out.data[6] = 2.0f * n.y * n.z - 2.0f * n.x * n.w;

		out.data[8] = 2.0f * n.x * n.z - 2.0f * n.y * n.w;
		out.data[9] = 2.0f * n.y * n.z + 2.0f * n.x * n.w;
		out.data[10] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.y * n.y;

		return Matrix4x4();
	}
	Matrix4x4 Matrix4x4::Rotate(QuaternionAngle rotation, Vector3 center)
	{
		Matrix4x4 outMatrix;

		float* o = outMatrix.data;
		o[0] = (rotation.x * rotation.x) - (rotation.y * rotation.y) - (rotation.z * rotation.z) + (rotation.w * rotation.w);
		o[1] = 2.0f * ((rotation.x * rotation.y) + (rotation.z * rotation.w));
		o[2] = 2.0f * ((rotation.x * rotation.z) - (rotation.y * rotation.w));
		o[3] = center.x - center.x * o[0] - center.y * o[1] - center.z * o[2];

		o[4] = 2.0f * ((rotation.x * rotation.y) - (rotation.z * rotation.w));
		o[5] = -(rotation.x * rotation.x) + (rotation.y * rotation.y) - (rotation.z * rotation.z) + (rotation.w * rotation.w);
		o[6] = 2.0f * ((rotation.y * rotation.z) + (rotation.x * rotation.w));
		o[7] = center.y - center.x * o[4] - center.y * o[5] - center.z * o[6];

		o[8] = 2.0f * ((rotation.x * rotation.z) + (rotation.y * rotation.w));
		o[9] = 2.0f * ((rotation.y * rotation.z) - (rotation.x * rotation.w));
		o[10] = -(rotation.x * rotation.x) - (rotation.y * rotation.y) + (rotation.z * rotation.z) + (rotation.w * rotation.w);
		o[11] = center.z - center.x * o[8] - center.y * o[9] - center.z * o[10];

		o[12] = 0.0f;
		o[13] = 0.0f;
		o[14] = 0.0f;
		o[15] = 1.0f;

		return outMatrix;
	}
	Matrix4x4 Matrix4x4::Multiply(Matrix4x4 a, Matrix4x4 b)
	{
		float* pMat1 = a.data;
		float* pMat2 = b.data;

		Matrix4x4 newMatrix;
		float* tempData = newMatrix.data;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				*tempData =
					pMat1[0] * pMat2[j] +
					pMat1[1] * pMat2[j + 4] +
					pMat1[2] * pMat2[j + 8] +
					pMat1[3] * pMat2[j + 12];
				tempData++;
			}
			pMat1 += 4;
		}
		return newMatrix;
	}
	Vector3 Matrix4x4::Foward(Matrix4x4 matrix)
	{
		Vector3 dir;
		dir.x = -matrix.data[2];
		dir.y = -matrix.data[6];
		dir.z = -matrix.data[10];
		return dir.Normalize();
	}
	Vector3 Matrix4x4::Backward(Matrix4x4 matrix)
	{
		Vector3 dir;
		dir.x = matrix.data[2];
		dir.y = matrix.data[6];
		dir.z = matrix.data[10];
		return dir.Normalize();
	}
	Vector3 Matrix4x4::Up(Matrix4x4 matrix)
	{
		Vector3 dir;
		dir.x = matrix.data[1];
		dir.y = matrix.data[5];
		dir.z = matrix.data[9];
		return dir.Normalize();
	}
	Vector3 Matrix4x4::Down(Matrix4x4 matrix)
	{
		Vector3 dir;
		dir.x = -matrix.data[1];
		dir.y = -matrix.data[5];
		dir.z = -matrix.data[9];
		return dir.Normalize();
	}
	Vector3 Matrix4x4::Left(Matrix4x4 matrix)
	{
		Vector3 dir;
		dir.x = -matrix.data[0];
		dir.y = -matrix.data[4];
		dir.z = -matrix.data[8];
		return dir.Normalize();
	}
	Vector3 Matrix4x4::Right(Matrix4x4 matrix)
	{
		Vector3 dir;
		dir.x = matrix.data[0];
		dir.y = matrix.data[4];
		dir.z = matrix.data[8];
		return dir.Normalize();
	}
	Matrix4x4 Matrix4x4::RotateEulerX(float rotationRadians)
	{
		Matrix4x4 newMatrix;
		float cosine = cos(rotationRadians);
		float sine = sin(rotationRadians);
		
		newMatrix.data[5] = cosine;
		newMatrix.data[6] = sine;
		newMatrix.data[9] = -sine;
		newMatrix.data[10] = cosine;

		return newMatrix;
	}
	Matrix4x4 Matrix4x4::RotateEulerY(float rotationRadians)
	{
		Matrix4x4 newMatrix;
		float cosine = cos(rotationRadians);
		float sine = sin(rotationRadians);

		newMatrix.data[0] = cosine;
		newMatrix.data[2] = -sine;
		newMatrix.data[8] = sine;
		newMatrix.data[10] = cosine;

		return newMatrix;
	}
	Matrix4x4 Matrix4x4::RotateEulerZ(float rotationRadians)
	{
		Matrix4x4 newMatrix;
		float cosine = cos(rotationRadians);
		float sine = sin(rotationRadians);

		newMatrix.data[0] = cosine;
		newMatrix.data[1] = sine;
		newMatrix.data[4] = -sine;
		newMatrix.data[5] = cosine;

		return newMatrix;
	}
}
