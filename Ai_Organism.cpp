#include "Ai_Organism.h"
#include "Organism.h"
#include "SharedContext.h"
#include "Engine.h"
#include "MathHelpers.h"

////////////////////////////////////////////////////////////
Ai_Organism::Ai_Organism() : Ai_Base() {
	registerTask<Ai_Organism>("Idle", &Ai_Organism::Task_Idle);
}

////////////////////////////////////////////////////////////
void Ai_Organism::Task_Idle(Actor_Base* t_owner) {
	auto owner{ dynamic_cast<Organism*>(t_owner) };
	float dt{ owner->getContext().m_engine->getElapsed().asSeconds() };
	float dx{ 0 };
	float dy{ 0 };
	mat::to_cartesian(owner->getMovementSpeed(), owner->getRotation(), dx, dy);
	owner->move(dx* dt, dy* dt);
}