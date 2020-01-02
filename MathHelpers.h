#ifndef MATH_HELPERS_HPP
#define MATH_HEMPLERS_HPP

#include <SFML/System/Vector2.hpp>

namespace mat {

	float lerp(const float& t_a, const float& t_b, const float& t_f, const bool& t_clamp = true);

	float midPoint(const float& t_a, const float& t_b);

	float toDegrees(const float& t_rad);

	float toRadians(const float& t_deg);

	float normalizeAngle(const float& t_deg); // [0 360)

	template <typename T>
	T map_value_to_range(T t_min, T t_max, T t_newMin, T t_newMax, T t_n);

	float vec2d_magnitude(const float& t_i, const float& t_j);

	float vec2d_magnitude(const sf::Vector2f& t_vec);

	template <typename T>
	float vec2d_magnitude(const sf::Vector2<T> t_vec);

	void vec2d_unitary(const float& t_i, const float& t_j, float& t_out_i, float& t_out_j);

	float vec2d_dotProduct(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2);

	float vec2d_dotProduct(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2);

	float vec2d_crossProduct(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2);

	float vec2d_crossProduct(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2);

	float vec2d_angleBetweenVectors(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2);

	float vec2d_angleBetweenVectors(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2);

	sf::Vector2f vec2d_unitary(const sf::Vector2f& t_vec);

	void vec2d_lerp(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2, const float& t_f, float& t_out_i, float& t_out_j);

	void vec2d_midPoint(const float& t_i1, const float& t_j1, const float& t_i2, const float& t_j2, float& t_out_i, float& t_out_j);

	sf::Vector2f vec2d_lerp(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2, const float& t_f);

	sf::Vector2f vec2d_midPoint(const sf::Vector2f& t_vec1, const sf::Vector2f& t_vec2);

	void to_polar(const float& t_i, const float& t_j, float& t_out_radius, float& t_out_degrees);

	void to_cartesian(const float& t_radius, const float& t_degrees, float& t_out_i, float& t_out_j);

	float distance(const float& t_x1, const float& t_y1, const float& t_x2, const float& t_y2);

	float distance(const sf::Vector2f& t_pos1, const sf::Vector2f& t_pos2);
}

#endif // !MATH_HELPERS_H