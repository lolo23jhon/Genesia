#include "Scenario_Basic.h"
#include "Engine.h"
#include "Food.h"
#include "Organism.h"
#include "SharedContext.h"

Scenario_Basic::Scenario_Basic(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms, const float& t_width, const float& t_height) :
	Scenario_Base(t_context, t_maxNumActors, t_initialNumOrganisms,t_width, t_height),
	m_firstOrganism{ nullptr }
{
	const float movementSpeed{150.f}; // unit/second
	const float rotationSpeed{50.f}; // deg/second
	
	m_firstOrganism = std::make_unique<Organism>(m_context, "Organism", 5.f, sf::Color::Green, sf::Vector2f(0.f, 0.f), 0.f, movementSpeed, rotationSpeed, 0.f);
}

////////////////////////////////////////////////////////////
void Scenario_Basic::init() {


	auto& rng{ *m_context.m_rng };
	auto simSize{ m_context.m_window->getSize() };


	for (unsigned i{ 0 }; i < m_initialNumOrganisms; i++) {
		float x{ rng(0.f, (float)simSize.x) };
		float y{ rng(0.f, (float)simSize.y) };
		float rot{ rng(0.f, 359.999f) };


		auto organism{ m_firstOrganism->clone() };
		organism->setPosition({ x,y });
		organism->setRotation(rot);
		m_context.m_engine->spawnActor(std::move(organism));
	}

}