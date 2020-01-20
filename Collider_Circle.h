#ifndef COLLIDER_CIRCLE_H
#define COLLIDER_CIRCLE_H

#include "Collider.h"

class Collider_Circle : public Collider {

	float m_radius;

public:
	Collider_Circle(Actor_Base* t_owner,const sf::Vector2f& t_pos, const float& t_radius);
	void setRadius(const float& t_radius);
	const float& getRadius()const;

	void update(Actor_Base* t_owner, const sf::Vector2f& t_pos, const float& t_radius);
	void draw(sf::RenderWindow& t_window);

	bool checkCollision(const Collider* t_otherCircle)const;
};

#endif // !COLLIDER_CIRCLE_H