#ifndef ORGANISM_H
#define ORGANIS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceHolder.h"
#include "Actor_Base.h"
#include "Ai_Organism.h"
#include "Traits.h"


class Organism : public Actor_Base {
	friend class Traits;

protected:
	std::string m_name;
	float m_size;
	float m_age;

	bool m_isDead;
	float m_destructionDelay; // Time after death the organism's physical body remains after it has remerged with inifite conciousness

	float m_energy; // Expended on every activity; Death when <= 0

	// ------------------------------ Traits ------------------------------
	Traits m_traits;
	
	// ------------------------ Traits-based values -----------------------
	float m_trait_maxEnergy;
	float m_trait_digestiveEfficiency;
	float m_trait_restingMetabolicRate;
	float m_trait_movementSpeed;
	float m_trait_turningSpeed;
	float m_trait_foodDetectionRange;
	float m_trait_lifespan;

	std::unique_ptr<Ai_Organism> m_ai;

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
	const float& getEnergy()const;
	void setEnergy(const float& t_energy);
	void addEnergy(const float& t_energy);
	const float& getRestingMetabolicRate()const;
	void setRestingMetabolicRate(const float& t_rmr);

	void update(const float& t_elapsed);

	ActorPtr clone();

	void die();

};
#endif // !ORGANISM_H
