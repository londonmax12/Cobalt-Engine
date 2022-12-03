#pragma once

#include "Memory/Memory.h"
#include "Vector.h"

namespace Ikigai {
	struct Matrix4x4 {
		float data[16];

		Matrix4x4() {
			data[0] = 1.0f;
			data[5] = 1.0f;
			data[10] = 1.0f;
			data[15] = 1.0f;
		}
		Matrix4x4(bool zero) {
			if (!zero) {
				data[0] = 1.0f;
				data[5] = 1.0f;
				data[10] = 1.0f;
				data[15] = 1.0f;
				return;
			}
			Memory::MemZeroMemory(data, sizeof(float) * 16);
		}

		static Matrix4x4 OrthographicMatrix(float left, float right, float bottom, float top, float nearClip, float farClip);
		static Matrix4x4 PerspectiveMatrix(float fovRadians, float aspectRation, float nearClip, float farClip);

		static Matrix4x4 LookAt(Vector3 position, Vector3 target, Vector3 up);

		static Matrix4x4 Inverse(Matrix4x4 matrix);

		static Matrix4x4 Transpose(Matrix4x4 matrix);

		static Matrix4x4 Translation(Vector3 position);
		static Matrix4x4 Scale(Vector3 scale);
		static Matrix4x4 Rotate(EulerAngle rotation);
		static Matrix4x4 Rotate(QuaternionAngle rotation);
		static Matrix4x4 Rotate(QuaternionAngle rotation, Vector3 center);

		static Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b);

		static Vector3 Foward(Matrix4x4 matrix);
		static Vector3 Backward(Matrix4x4 matrix);
		static Vector3 Up(Matrix4x4 matrix);
		static Vector3 Down(Matrix4x4 matrix);
		static Vector3 Left(Matrix4x4 matrix);
		static Vector3 Right(Matrix4x4 matrix);

		Matrix4x4 operator* (Matrix4x4 a) {
			
			float* pMat1 = data;
			float* pMat2 = a.data;
			
			Matrix4x4 temp;
			float* tempData = temp.data;
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

			return temp;
		}
	private:
		static Matrix4x4 RotateEulerX(float rotationRadians);
		static Matrix4x4 RotateEulerY(float rotationRadians);
		static Matrix4x4 RotateEulerZ(float rotationRadians);
	};
}