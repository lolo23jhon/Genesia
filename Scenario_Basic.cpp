#include "Scenario_Basic.h"
#include "Engine.h"
#include "SharedContext.h"


static const float S_FOOD_ENERGY{ 400.f };
static const float S_FOOD_DURATION{ INFINITY };

Scenario_Basic::Scenario_Basic(SharedContext& t_context,
	const unsigned& t_initialNumOrganisms,
	const unsigned& t_maxNumOrganisms,
	const unsigned& t_initialNumFood,
	const unsigned& t_maxNumFood,
	const float& t_width,
	const float& t_height) :
	Scenario_Base(t_context, t_maxNumOrganisms + t_maxNumFood, t_initialNumOrganisms, t_width, t_height),
	m_maxNumOrganisms{ t_maxNumOrganisms },
	m_initialNumOrganisms{ t_initialNumOrganisms },
	m_maxNumFood{ t_maxNumOrganisms },
	m_initialNumFood{ t_initialNumFood },
	m_firstOrganism{ std::move(Organism::makeDefaultOffspring(m_context, "Organism", sf::Vector2f(0.f, 0.f), 0.f, 0.f)) },
	m_food{ std::make_unique<Food>(t_context, sf::Vector2f(0.f,0.f), 0.f, S_FOOD_ENERGY, S_FOOD_DURATION) }
{}

////////////////////////////////////////////////////////////
void Scenario_Basic::init() {

	auto& rng{ *m_context.m_rng };

	// Create and spanw the organisms
	for (unsigned i{ 0U }; i < m_initialNumOrganisms; i++) {
		float x{ rng(m_simulationRectangle.width , m_simulationRectangle.left) };
		float y{ rng(m_simulationRectangle.height , m_simulationRectangle.top) };
		float rot{ rng(0.f, 359.9999999f) };


		auto organism{ std::move(m_firstOrganism->reproduce(m_context)) }; // Make children of the first organism
		organism->setPosition({ x,y });
		organism->setRotation(rot);
		m_context.m_engine->spawnActor(std::move(organism));
	}

	// Create and spawn food
	for (unsigned i{ 0U }; i < m_initialNumFood; i++) {
		float x{ rng(m_simulationRectangle.width , m_simulationRectangle.left) };
		float y{ rng(m_simulationRectangle.height , m_simulationRectangle.top) };
		float rot{ rng(0.f, 359.9999999f) };
		float energyFactor{ m_context.m_rng->normalDisttribution(1.f, 0.4f) };

		auto food{ std::move(m_food->clone(m_context)) };
		food->setPosition({ x,y });
		food->setRotation(rot);
		static_cast<Food*>(food.get())->setEnergy(energyFactor * S_FOOD_ENERGY);
		food->setTextString("Food: " + std::to_string(static_cast<int>(static_cast<Food*>(food.get())->getEnergy())));
		m_context.m_engine->spawnActor(std::move(food));
	}
}

////////////////////////////////////////////////////////////
void Scenario_Basic::update(const float& t_elapsed) {
	// Spawn in more food if necessary
	if (Food::getNumFood()  < m_initialNumFood) {
		auto& rng{ *m_context.m_rng };

		float x{ rng(m_simulationRectangle.width , m_simulationRectangle.left) };
		float y{ rng(m_simulationRectangle.height , m_simulationRectangle.top) };
		float rot{ rng(0.f, 359.9999999f) };
		float energyFactor{ m_context.m_rng->normalDisttribution(1.f, 0.4f) };

		auto food{ std::move(m_food->clone(m_context)) };
		food->setPosition({ x,y });
		food->setRotation(rot);
		static_cast<Food*>(food.get())->setEnergy(energyFactor * S_FOOD_ENERGY);
		food->setTextString("Food: " + std::to_string(static_cast<int>(static_cast<Food*>(food.get())->getEnergy())));
		m_context.m_engine->spawnActor(std::move(food));
	}
	Scenario_Base::update(t_elapsed);
}