#ifndef SHARED_CONTEXT_H
#define SHARED_CONTEXT_H

// Used by the classes and nodes to get access to the engine, window and some other important stuff
//	that is globally shared
#include <SFML/Graphics/RenderWindow.hpp>

class Engine;
class RandomGenerator;
class ResourceHolder;
struct SharedContext {

	sf::RenderWindow& m_window;
	Engine& m_engine;
	RandomGenerator& m_rng;
	ResourceHolder& m_resourceHolder;

	////////////////////////////////////////////////////////////
	SharedContext(
		sf::RenderWindow& t_window,
		Engine& t_engine,
		RandomGenerator& t_rng,
		ResourceHolder& t_resourceHolder):
		m_window{ t_window },
		m_engine{ t_engine },
		m_rng{ t_rng },
		m_resourceHolder{t_resourceHolder}
	{
	}
};

#endif // !SHARED_CONTEXT_H
