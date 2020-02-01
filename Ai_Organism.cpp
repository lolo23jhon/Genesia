#include "Ai_Organism.h"
#include "Organism.h"
#include "SharedContext.h"
#include "Engine.h"
#include "MathHelpers.h"
#include "PerlinNoise.h"

////////////////////////////////////////////////////////////
Ai_Organism::Ai_Organism(SharedContext& t_context) : m_noiseIncrement{ t_context.m_rng->generate(0.f, 100000.f) } {}

////////////////////////////////////////////////////////////
void Ai_Organism::update(Actor_Base* t_owner, const float& t_elapsed) {
	auto owner{ static_cast<Organism*>(t_owner) };


	// Reproduction is expensive, so the organism will make sure it has got plenty of energy 
	// Organisms reach sexual maturity at 20% of their lifespan
	if (owner->getEnergyPct() >= 0.8f && owner->getAgePct() >= 0.2f) {
		float energyCostPct{ 0.65f };
		float efficiency{ 0.7f };

		float maxEnergy{ owner->getEnergy() / owner->getEnergyPct() };

		// Make the actor and make an organism ptr to it
		auto offspringPtr{ owner->reproduce(owner->getContext()) };
		auto offspring{ static_cast<Organism*>(offspringPtr.get()) };
	
		// All the energy from the birth goes to the environment. The offspring gets an "energy check" that it itself cashes on spawn
		owner->addEnergy(-maxEnergy * energyCostPct);
		owner->getScenario().addEnergy(maxEnergy * energyCostPct);
		
		// Give the offspring an energy check minus an efficiency penalty due to birth labor
		offspring->setEnergy(0.f);
		offspring->addEnergy(maxEnergy * energyCostPct * efficiency);

		// Set a random rotation
		offspring->setRotation(t_owner->getContext().m_rng->generate(0.f, 359.9999999f));
		
		// Put the offspring in the spawn list
		t_owner->getContext().m_engine->spawnActor(std::move(offspringPtr));
	}

	// Idle movement
	{
		const auto& disp{ owner->getMovementSpeed() * t_elapsed };
		float dx{ 0 };
		float dy{ 0 };

		mat::to_cartesian(disp, owner->getRotation(), dx, dy);
		owner->move(dx, dy);
		owner->rotate(PerlinNoise::noise(m_noiseIncrement + owner->getAge()) * owner->getRotationSpeed() * t_elapsed); // Use perlin noise for natural-looking movement
	}
}