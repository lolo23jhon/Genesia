#include "Ai_Organism.h"
#include "Organism.h"
#include "SharedContext.h"
#include "Engine.h"
#include "PerlinNoise.h"
#include "MathHelpers.h"

////////////////////////////////////////////////////////////
Ai_Organism::Ai_Organism(Actor_Base* t_owner) : Ai_Base(), m_noiseIncrement{ t_owner->getContext().m_engine->getRandom(0.f, 1000000.f) }{
	registerTask<Ai_Organism>("Idle", &Ai_Organism::Task_Idle);
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
void Ai_Organism::update(Actor_Base* t_owner, const float& t_elapsed) {
	auto owner{ static_cast<Organism*>(t_owner) };
	pollTasks();
	executeTask(*m_currentTask, t_owner, t_elapsed);
}
