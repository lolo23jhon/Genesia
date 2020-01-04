#include "Scenario_Basic.h"
#include "Engine.h"
#include "ActorFactory.h"

static const std::string S_TEXTURE_NAME{ "Texture_TriangleActorCyan" };
static const std::string S_TEXTURE_FILENAME{ "resources\\triangle_actor_cyan.png" };

////////////////////////////////////////////////////////////
void init(SharedContext& t_context) {

	auto& factory{ t_context.m_engine->getActorFactory() };

	auto actor{ factory.newActor()
		.addComponent(ActorComponentType::Sprite, t_context, S_TEXTURE_NAME, S_TEXTURE_FILENAME)
		.create() };
	unsigned numCopies{ 4 };
	Actors copies;
	copies.reserve(numCopies);
	factory.makeActorCopies(actor, copies, numCopies);
	copies.emplace_back(std::move(actor));

	auto& rng{ *t_context.m_rng };
	auto size{ t_context.m_window->getSize() };
	for (auto& it : copies) {
		float x{ rng(0.f, (float)size.x) };
		float y{ rng(0.f, (float)size.y) };
		float rot{ rng(0.f,359.999f) };

		// Spawn them at random positions and facing angles
		it->setPosition(x, y);
		it->setRotation(rot);
		t_context.m_engine->spawnActor(std::move(it));
	}



	// Spawn initial amount of food



	// Spanw initial amount of organisms
		// Call the factory 




}

////////////////////////////////////////////////////////////
void update() {


}