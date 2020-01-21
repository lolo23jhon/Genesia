#include "Ai_Organism.h"
#include "Organism.h"
#include "SharedContext.h"
#include "Engine.h"
#include "PerlinNoise.h"
#include "MathHelpers.h"

////////////////////////////////////////////////////////////
Ai_Organism::Ai_Organism(Actor_Base* t_owner) : Ai_Base(), m_noiseIncrement{ t_owner->getContext().m_engine->getRandom(0.f, 1000000.f) }{
	static bool first{ true };
	if (first) {
		registerTask<Ai_Organism>("Idle", &Ai_Organism::Task_Idle);
		registerTask<Ai_Organism>("Organism_Reproduce", &Ai_Organism::Task_Reproduce);
		first = false;
	}
}

////////////////////////////////////////////////////////////
void Ai_Organism::Task_Idle(Actor_Base* t_owner, const float& t_elapsed) {
	auto owner{ static_cast<Organism*>(t_owner) };
	float dx{ 0 };
	float dy{ 0 };
	const auto& disp{ owner->getMovementSpeed() * t_elapsed };

	mat::to_cartesian(disp, owner->getRotation(), dx, dy);
	owner->move(dx, dy);
	owner->rotate(PerlinNoise::noise(m_noiseIncrement + owner->getAge())* owner->getRotationSpeed()* t_elapsed); // Use perlin noise for natural-looking movement
}

////////////////////////////////////////////////////////////
void Ai_Organism::Task_Reproduce(Actor_Base* t_owner, const float& t_elapsed) {
	auto owner{ static_cast<Organism*>(t_owner) };
	auto offspring{ owner->reproduce(t_owner->getContext()) };
	owner->addEnergyPct(-3.f);
	t_owner->getContext().m_engine->spawnActor(std::move(offspring));
	m_isDone = true;
}


////////////////////////////////////////////////////////////
void Ai_Organism::update(Actor_Base* t_owner, const float& t_elapsed) {
	auto owner{static_cast<Organism*>(t_owner)};
	if (owner->getEnergyPct() > 0.7) { 
		m_taskQueue.emplace("Organism_Reproduce");
		m_isDone = false;
	}

	pollTasks();
	executeTask(*m_currentTask, static_cast<Organism*>(t_owner), t_elapsed);
}
