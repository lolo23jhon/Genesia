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


	// Reproduce if energy at or above 70%, reproduction costs 30% energy
	// Sexual maturity at 20% of lifespan
	if (owner->getEnergyPct() >= 0.80f && owner->getAgePct() >= 0.2f) {
		owner->addEnergyPct(-0.55f);
		auto offspringPtr{ owner->reproduce(owner->getContext()) };
		auto offspring{ static_cast<Organism*>(offspringPtr.get()) };
		offspring->setEnergyPct(0.7f);
		offspring->setRotation(t_owner->getContext().m_rng->generate(0.f, 359.9999999f));
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