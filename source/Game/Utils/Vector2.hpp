#pragma once

struct Vector2 {
	float x, y;

	inline void X(float _x) {
		x = _x;
	}

	inline void Y(float _y) {
		y = _y;
	}

	inline float X() const {
		return x;
	}

	inline float Y() const {
		return y;
	}

	inline float Length() const {
		return std::sqrt(x * x + y * y);
	}

	inline void Normalize() {
		float norm = std::sqrt(x * x + y * y);
		if (norm != 0.0f) {
			x /= norm;
			y /= norm;
		}
	}

	inline Vector2 Normalized() const {
		float norm = std::sqrt(x * x + y * y);
		if (norm != 0.0f) {
			return Vector2{
				x / norm,
				y / norm,
			};
		}
		return *this;
	}

	static inline float Dot(Vector2 a, Vector2 b) {
		return a.x * b.x + a.y * b.y;
	}
};