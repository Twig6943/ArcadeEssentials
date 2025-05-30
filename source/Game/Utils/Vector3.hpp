#pragma once
#include <complex>

struct Vector3 {
	float x;
	float y;
	float z;

	inline void X(float _x) {
		x = _x;
	}
	
	inline void Y(float _y) {
		y = _y;
	}

	inline void Z(float _z) {
		z = _z;
	}

	inline void Normalize() {
		float norm = std::sqrt(x * x + y * y + z * z);
		if (norm != 0.0f) {
			x /= norm;
			y /= norm;
			z /= norm;
		}
	}

	static inline float Dot(Vector3 a, Vector3 b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
};

inline Vector3 operator*(const Vector3 a, const float b) {
	return Vector3{
		a.x * b,
		a.y * b,
		a.z * b,
	};
}

inline Vector3 operator*(const float b, const Vector3 a) {
	return Vector3{
		a.x * b,
		a.y * b,
		a.z * b,
	};
}

inline Vector3 operator+(const Vector3 a, const Vector3 b) {
	return Vector3{
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	};
}

inline Vector3 operator-(const Vector3 a, const Vector3 b) {
	return Vector3{
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
	};
}
