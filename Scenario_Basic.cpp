#include "Scenario_Basic.h"
#include "Engine.h"
#include "Food.h"
#include "Organism.h"
#include "SharedContext.h"

Scenario_Basic::Scenario_Basic(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms, const float& t_width, const float& t_height) :
	Scenario_Base(t_context, t_maxNumActors, t_initialNumOrganisms, t_width, t_height),
	m_firstOrganism{ std::move(Organism::makeDefaultOffspring(m_context, "Organism", sf::Vector2f(0.f, 0.f), 0.f, 0.f))}
{}

////////////////////////////////////////////////////////////
void Scenario_Basic::init() {


	auto& rng{ *m_context.m_rng };
	auto simSize{ m_context.m_window->getSize() };


	for (unsigned i{ 0 }; i < m_initialNumOrganisms; i++) {
		float x{ rng(0.f, (float)simSize.x) };
		float y{ rng(0.f, (float)simSize.y) };
		float rot{ rng(0.f, 359.9999999f) };


		auto organism{ std::move(m_firstOrganism->reproduce(m_context)) }; // Make children of the first organism
		organism->setPosition({ x,y });
		organism->setRotation(rot);
		m_context.m_engine->spawnActor(std::move(organism));
	}

}