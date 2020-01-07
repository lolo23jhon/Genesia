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
	auto owner{ static_cast<Organism*>(t_owner) };
	float dt{ owner->getContext().m_engine->getElapsed().asSeconds() };
	float dx{ 0 };
	float dy{ 0 };
	const auto& disp{ owner->getMovementSpeed()*dt };

	mat::to_cartesian(disp, owner->getRotation(), dx, dy);
	owner->move(dx, dy);
	owner->rotate(owner->getRotationSpeed()* dt);
	owner->setTextSring(owner->getName() + " : " + *m_currentTask + " Rot: " + std::to_string(owner->getRotation()));
}

void Ai_Organism::update(Actor_Base* t_owner) {
	auto owner{ static_cast<Organism*>(t_owner) };
	pollTasks();
	executeTask(*m_currentTask, t_owner);
}
