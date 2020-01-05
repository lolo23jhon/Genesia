#ifndef ORGANISM_H
#define ORGANIS_H

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceHolder.h"
#include "Actor_Base.h"

class Organism : public Actor_Base {
protected:
	std::string m_name;
	float m_size;
	float m_age;

	float m_movementSpeed;
	float m_rotationSpeed;


public:
	Organism(SharedContext& t_context,
		const std::string& t_name,
		const float& t_size,
		const sf::Color& t_color,
		const sf::Vector2f& t_position,
		const float& t_rotation,
		const float& t_movSpeed,
		const float& t_rotSpeed,
		const float& t_age = 0.f);
	const float& getMovementSpeed()const;
	void setMovementSpeed(const float& t_movementSpeed);
	const float& getRotationSpeed()const;
	void setRotationSpeed(const float& t_rotationSpeed);
	const std::string& getName()const;
	void setName(const std::string& t_name);
	const float& getAge()const;
	void setAge(const float& t_age);
	const float& getSize()const;
	void setSize(const float& t_size);

	void update();

	ActorPtr clone();

	virtual void eat();


};
#endif // !ORGANISM_H
