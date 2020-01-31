#include "MathHelpers.h"
#include <cmath>
#include <limits>
#include <vector>

namespace mat {

	constexpr float TO_DEGREES{ 57.2957795f };
	constexpr float TO_RADIANS{ 0.0174533f };
	constexpr float PI{ 3.1415926f };


	////////////////////////////////////////////////////////////
	float lerp(const float& t_a, const float& t_b, const float& t_f, bool t_clamp) {
		if (t_clamp) {
			if (t_f <= 0) { return t_a; }
			if (t_f >= 1) { return t_b; }
		}
		return t_a + t_f * (t_b - t_a);
	}

	////////////////////////////////////////////////////////////
	float midPoint(const float& t_a, const float& t_b) {
		return lerp(t_a, t_b, 0.5f);
	}

	////////////////////////////////////////////////////////////
	float toDegrees(const float& t_rad) { return t_rad * TO_DEGREES; }

	////////////////////////////////////////////////////////////
	float toRadians(const float& t_deg) { return t_deg * TO_RADIANS; }

	////////////////////////////////////////////////////////////
	float normalizeAngle(const float& t_deg) { // [0 360)
		if (t_deg >= 0 && t_deg < 360) { return t_deg; }
		float angle{ std::fmod(t_deg, 360.f) };
		return (angle >= 0 ? angle : angle + 360.f);
	}

	////////////////////////////////////////////////////////////
	template <typename T> T map_value_to_range(T t_min, T t_max, T t_newMin, T t_newMax, T t_n) { return t_newMin + (t_n - t_min) * (t_newMax - t_newMin) / (t_max - t_min); }

	////////////////////////////////////////////////////////////
	template <typename T> void map_value_to_range(const std::vector<T>& t_vector,std::vector<T>& t_mappedVector, T t_newMin, T t_newMax) {
		T min{ std::numeric_limits<T>::infinity() };
		T max{ -std::numeric_limits<T>::infinity() };
		for (const auto& n : t_vector ) {
			if (max < n) { max = n; }
			if (min > n) { min = n; }
		}

		t_mappedVector.clear();
		for (const auto& n : t_vector) {
			t_mappedVector.emplace_back(t_newMin + (n - min) * (t_newMax - t_newMin) / (max - min));
		}
	}

	////////////////////////////////////////////////////////////
	float vec2d_magnitude(const float& t_i, const float& t_j) { return std::sqrt(t_i * t_i + t_j * t_j); }

	////////////////////////////////////////////////////////////
	float vec2d_magnitude(const sf::Vector2f& t_vec) { return vec2d_magnitude(t_vec.x, t_vec.y); }

	////////////////////////////////////////////////////////////
	template <typename T>
	float vec2d_magnitude(const sf::Vector2<T> t_vec) { return std::sqrtf(t_vec.x * t_vec.x + t_vec.y * t_vec.y); }

	////////////////////////////////////////////////////////////
	void vec2d_unitary(const float& t_i, const float& t_j, float& t_out_i, float& t_out_j) {
		float mag{ vec2d_magnitude(t_i, t_j) };
		t_out_i = t_i / mag;
		t_out_j = t_j / mag;
	}

	////////////////////////////////////////////////////////////
	float vec2d_dotProduct(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2) {
		return	t_i1 * t_i2 + t_j1 * t_j2;
	}

	////////////////////////////////////////////////////////////
	float vec2d_dotProduct(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2) {
		return vec2d_dotProduct(t_vec1.x, t_vec1.y, t_vec2.x, t_vec2.y);
	}

	////////////////////////////////////////////////////////////
	float vec2d_crossProduct(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2) {
		return t_i1 * t_j2 - t_j1 * t_i2;
	}

	////////////////////////////////////////////////////////////
	float vec2d_crossProduct(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2) {
		return vec2d_crossProduct(t_vec1.x, t_vec1.y, t_vec2.x, t_vec2.y);
	}

	////////////////////////////////////////////////////////////
	float vec2d_angleBetweenVectors(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2) {
		return std::acos(vec2d_dotProduct(t_i1, t_j1, t_i2, t_j2) / (vec2d_magnitude(t_i1, t_j1) * vec2d_magnitude(t_i2, t_j2))) * TO_DEGREES;
	}

	////////////////////////////////////////////////////////////
	float vec2d_angleBetweenVectors(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2) {
		return vec2d_angleBetweenVectors(t_vec1.x, t_vec1.y, t_vec2.x, t_vec2.y);
	}

	////////////////////////////////////////////////////////////
	sf::Vector2f vec2d_unitary(const sf::Vector2f& t_vec) {
		float mag{ vec2d_magnitude(t_vec.x, t_vec.y) };
		return { t_vec.x / mag ,t_vec.y / mag };
	}

	////////////////////////////////////////////////////////////
	void vec2d_lerp(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2, const float& t_f, float& t_out_i, float& t_out_j) {
		t_out_i = lerp(t_i1, t_i2, t_f);
		t_out_j = lerp(t_j1, t_j2, t_f);
	}

	////////////////////////////////////////////////////////////
	void vec2d_midPoint(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2, float& t_out_i, float& t_out_j) {
		t_out_i = lerp(t_i1, t_i2, 0.5f);
		t_out_j = lerp(t_j1, t_j2, 0.5f);
	}

	////////////////////////////////////////////////////////////
	sf::Vector2f vec2d_lerp(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2, const float& t_f) {
		return { lerp(t_vec1.x, t_vec2.x, t_f) ,lerp(t_vec1.y, t_vec2.y, t_f) };
	}

	////////////////////////////////////////////////////////////
	sf::Vector2f vec2d_midPoint(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2) {
		return { lerp(t_vec1.x, t_vec2.x, 0.5f) ,lerp(t_vec1.y, t_vec2.y, 0.5f) };
	}

	////////////////////////////////////////////////////////////
	void to_polar(const float& t_i, const float& t_j, float& t_out_radius, float& t_out_degrees) {
		t_out_radius = std::sqrtf(t_i * t_i + t_j * t_j);
		t_out_degrees = std::atanf(t_i / t_j) * TO_DEGREES;
	}

	////////////////////////////////////////////////////////////
	void to_cartesian(const float& t_radius, const float& t_degrees, float& t_out_i, float& t_out_j) {
		t_out_i = t_radius * std::cos(t_degrees * TO_RADIANS);
		t_out_j = t_radius * std::sin(t_degrees * TO_RADIANS);
	}

	////////////////////////////////////////////////////////////
	float distance(const float& t_x1, const float& t_y1, const float& t_x2, const float& t_y2) {
		return std::sqrtf((t_x1 - t_x2) * (t_x1 - t_x2) + (t_y1 - t_y2) * (t_y1 - t_y2));
	}

	////////////////////////////////////////////////////////////
	float distance(const sf::Vector2f& t_pos1, const sf::Vector2f& t_pos2) {
		return distance(t_pos1.x, t_pos1.y, t_pos2.x, t_pos2.y);
	}

}