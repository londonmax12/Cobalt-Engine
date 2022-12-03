#pragma once

namespace Ikigai {
	struct Vector2
	{
	public:
		Vector2();
		Vector2(float scalar);
		Vector2(float x, float y);

		static Vector2 Zero();
		static Vector2 One();

		struct
		{
			union
			{
				float x, r, s, u;
			};
			union
			{
				float y, g, t, v;
			};

		};

		float LengthSq() {
			return x * x + y * y;
		}

		float Length() {
			return sqrt(LengthSq());
		}

		Vector2 Normalize() {
			float len = Length();
			Vector2 newVec{ x, y };
			newVec.x /= len;
			newVec.y /= len;
		}

		bool Compare(Vector2 vec, float tolerance = 1.0f) {
			if (abs(x - vec.x) > tolerance)
				return false;

			if (abs(y - vec.y) > tolerance)
				return false;

			return true;
		}

		float Distance(Vector2 vec)
		{
			Vector2 distance = Vector2(x - vec.x, y - vec.y);
			return distance.Length();
		}

		Vector2 operator+ (float scalar) {
			return (
				x + scalar,
				y + scalar
			);
		}
		Vector2 operator- (float scalar) {
			return (
				x - scalar,
				y - scalar
			);
		}
		Vector2 operator* (float scalar) {
			return (
				x * scalar,
				y * scalar
			);
		}
		Vector2 operator/ (float scalar) {
			return (
				x / scalar,
				y / scalar
			);
		}
		Vector2 operator+ (Vector2 a) {
			return (
				x + a.x,
				y + a.y
			);
		}
		Vector2 operator- (Vector2 a) {
			return (
				x - a.x,
				y - a.y
			);
		}
		Vector2 operator* (Vector2 a) {
			return (
				x * a.x,
				y * a.y
			);
		}
		Vector2 operator/ (Vector2 a) {
			return (
				x / a.x,
				y / a.y
			);
		}
	};

	struct Vector3
	{
	public:
		Vector3();
		Vector3(float scalar);
		Vector3(float x, float y, float z);

		static Vector3 Zero();
		static Vector3 One();

		struct
		{
			union
			{
				float x, r, s, u;
			};
			union
			{
				float y, g, t, v;
			};
			union
			{
				float z, b, p, w;
			};
		};

		bool Compare(Vector3 vec, float tolerance = 1.0f) {
			if (abs(x - vec.x) > tolerance)
				return false;

			if (abs(y - vec.y) > tolerance)
				return false;

			if (abs(z - vec.z) > tolerance)
				return false;

			return true;
		}

		float LengthSq() {
			return x * x + y * y + z * z;
		}

		float Length() {
			return sqrt(LengthSq());
		}

		Vector3 Normalize() {
			float len = Length();
			Vector3 newVec{ x, y, z };
			newVec.x /= len;
			newVec.y /= len;
			newVec.z /= len;

			return newVec;
		}

		float Distance(Vector3 vec)
		{
			Vector3 distance = Vector3(x - vec.x, y - vec.y, z - vec.z);
			return distance.Length();
		}

		float Dot(Vector3 vec) {
			float p = 0;
			p += x * vec.x;
			p += y * vec.y;
			p += z * vec.z;
			return p;
		}

		Vector3 Cross(Vector3 vec) {
			return Vector3(
				y * vec.z - z * vec.y,
				z * vec.x - x * vec.z,
				x * vec.y - y * vec.x
			);
		}

		Vector3 operator+ (float scalar) {
			return Vector3(
				x + scalar,
				y + scalar,
				z + scalar
			);
		}
		Vector3 operator- (float scalar) {
			return Vector3(
				x - scalar,
				y - scalar,
				z - scalar
			);
		}
		Vector3 operator* (float scalar) {
			return Vector3(
				x * scalar,
				y * scalar,
				z * scalar
			);
		}
		Vector3 operator/ (float scalar) {
			return Vector3(
				x / scalar,
				y / scalar,
				z / scalar
			);
		}
		Vector3 operator+ (Vector3 a) {
			return Vector3(
				x + a.x,
				y + a.y,
				z + a.z
			);
		}
		Vector3 operator- (Vector3 a) {
			return Vector3(
				x - a.x,
				y - a.y,
				z - a.z
			);	
		}
		Vector3 operator* (Vector3 a) {
			return Vector3(
				x * a.x,
				y * a.y,
				z * a.z
			);
		}
		Vector3 operator/ (Vector3 a) {
			return Vector3(
				x / a.x,
				y / a.y,
				z / a.z
			);
		}
		bool operator== (Vector3 a) {
			return Compare(a, 0.01);
		}
	};
	typedef Vector3 EulerAngle;

	struct Vector4
	{
	public:
		Vector4();
		Vector4(float scalar);
		Vector4(float x, float y, float z, float w);

		static Vector4 Zero();
		static Vector4 One();

		struct
		{
			union
			{
				float x, r, s;
			};
			union
			{
				float y, g, t;
			};
			union
			{
				float z, b, p;
			};
			union
			{
				float w, a, q;
			};
		};

		bool Compare(Vector4 vec, float tolerance = 1.0f) {
			if (abs(vec.x - x) > tolerance)
				return false;

			if (abs(vec.y - y) > tolerance)
				return false;

			if (abs(vec.z - z) > tolerance)
				return false;

			if (abs(vec.w - w) > tolerance)
				return false;

			return true;
		}

		float LengthSq() {
			return x * x + y * y + z * z + w * w;
		}

		float Length() {
			return sqrt(LengthSq());
		}

		Vector4 Normalize() {
			float len = Length();
			Vector4 newVec{ x, y, z, w };
			newVec.x /= len;
			newVec.y /= len;
			newVec.z /= len;
			newVec.w /= len;
			return newVec;
		}

		float Dot(Vector4 vec) {
			float p = 0;
			p += x * vec.x;
			p += y * vec.y;
			p += z * vec.z;
			p += w * vec.w;
			return p;
		}
		float Dot(float a0, float a1, float a2, float a3) {
			float p = 0;
			p = x * a0 + y * a1 + z * a2 + w * a3;
			return p;
		}
		Vector4 operator+ (float scalar) {
			return (
				x + scalar,
				y + scalar,
				z + scalar,
				w + scalar
			);
		}
		Vector4 operator- (float scalar) {
			return (
				x - scalar,
				y - scalar,
				z - scalar,
				w - scalar
			);
		}
		Vector4 operator* (float scalar) {
			return (
				x * scalar,
				y * scalar,
				z * scalar,
				w * scalar
			);
		}
		Vector4 operator/ (float scalar) {
			return (
				x / scalar,
				y / scalar,
				z / scalar,
				w / scalar
			);
		}
		Vector4 operator+ (Vector4 a) {
			return (
				x + a.x,
				y + a.y,
				z + a.z,
				w + a.w
			);
		}
		Vector4 operator- (Vector4 a) {
			return (
				x - a.x,
				y - a.y,
				z - a.z,
				w - a.w
			);
		}
		Vector4 operator* (Vector4 a) {
			return (
				x * a.x,
				y * a.y,
				z * a.z,
				w * a.w
			);
		}
		Vector4 operator/ (Vector4 a) {
			return (
				x / a.x,
				y / a.y,
				z / a.z,
				w / a.w
			);
		}
		bool operator== (Vector4 a) {
			return Compare(a, 0.01);
		}

	};
	typedef Vector4 Color;

	struct QuaternionAngle
	{
		QuaternionAngle() {
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 1.f;
		}
		QuaternionAngle(float X, float Y, float Z, float W) {
			x = X;
			y = Y;
			z = Z;
			w = W;
		}

		struct
		{
			union
			{
				float x, r, s;
			};
			union
			{
				float y, g, t;
			};
			union
			{
				float z, b, p;
			};
			union
			{
				float w, a, q;
			};
		};

		bool Compare(Vector4 vec, float tolerance = 1.0f) {
			if (abs(vec.x - x) > tolerance)
				return false;

			if (abs(vec.y - y) > tolerance)
				return false;

			if (abs(vec.z - z) > tolerance)
				return false;

			if (abs(vec.w - w) > tolerance)
				return false;

			return true;
		}

		float LengthSq() {
			return x * x + y * y + z * z + w * w;
		}

		float Length() {
			return sqrt(LengthSq());
		}

		QuaternionAngle Normalize() {
			float len = Length();
			QuaternionAngle newVec{ x, y, z, w };
			newVec.x /= len;
			newVec.y /= len;
			newVec.z /= len;
			newVec.w /= len;
			return newVec;
		}

		float Dot(QuaternionAngle vec) {
			float p = 0;
			p += x * vec.x;
			p += y * vec.y;
			p += z * vec.z;
			p += w * vec.w;
			return p;
		}
		float Dot(float a0, float a1, float a2, float a3) {
			float p = 0;
			p = x * a0 + y * a1 + z * a2 + w * a3;
			return p;
		}

		QuaternionAngle Conjugate();
		QuaternionAngle Inverse();

		static QuaternionAngle Multiply(QuaternionAngle a, QuaternionAngle b);
		static QuaternionAngle Slerp(QuaternionAngle a, QuaternionAngle b, float percent);
		static QuaternionAngle AxisAngle(Vector3 axis, float angle);

		QuaternionAngle operator* (QuaternionAngle b) {
			QuaternionAngle out;

			out.x = x * b.w + y + b.z - z * b.y + w * b.x;
			out.y = -x * b.z + y + b.w - z * b.x + w * b.y;
			out.z = x * b.y + y + b.x - z * b.w + w * b.z;
			out.w = -x * b.x + y + b.y - z * b.z + w * b.w;

			return out;
		}
	};
}