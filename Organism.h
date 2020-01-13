#ifndef ORGANISM_H
#define ORGANIS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "HSLColor.h"
#include "ResourceHolder.h"
#include "Ai_Organism.h"
#include "TraitCollection.h"
#include "Trait.h"


class Organism : public Actor_Base {
	friend class Trait_Base;

protected:
	HSL m_hslColor; // Contains the HSL copy of it's parent class RGB color for more better color manipulation

	std::string m_name;
	float m_age;

	bool m_isDead;
	float m_destructionDelay; // Time after death the organism's physical body remains after it has remerged with inifite conciousness

	float m_energy; // Expended on every activity; Death when <= 0

	// ------------------------------ Traits ------------------------------
	TraitCollection m_traits;
	
	// ------------------------ Traits-based values -----------------------
	float m_trait_maxEnergy;
	float m_trait_digestiveEfficiency;
	float m_trait_restingMetabolicRate;
	float m_trait_movementSpeed;
	float m_trait_turningSpeed;
	float m_trait_foodDetectionRange;
	float m_trait_lifespan;
	float m_trait_size;

	std::unique_ptr<Ai_Organism> m_ai;

public:
	Organism(SharedContext& t_context,
		const std::string& t_name,
		const sf::Vector2f& t_position,
		const float& t_rotation,
		const float& t_age = 0.f);
	const HSL& getColorHLS()const;
	void setColorHSL(const float& t_h, const float& t_s, const float&t_l); // Also overwrites the SFML rgb color

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
