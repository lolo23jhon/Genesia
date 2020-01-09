#ifndef SCENARIO_BASE_H
#define SCENARIO_BASE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

struct SharedContext;

class Scenario_Base {
protected:
	unsigned m_maxNumActors; // Unlimited if 0
	unsigned m_initialNumOrganisms;
	sf::FloatRect m_simulationRectangle;
	sf::RectangleShape m_simulationBackground;
	SharedContext& m_context;



public:
	Scenario_Base(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms, const float& t_width, const float& t_height);
	const unsigned& getMaxNumActors()const;
	void setMaxNumActors(const unsigned& t_num);
	const unsigned& getInitialNumOrganisms()const;


	virtual void init() = 0; // Anything that happens when the simulation begins, like spawning the first characters
	virtual void update(const float& t_elapsed); // Anything that happens every frame, such as spawning x actor on condition y at time z
	virtual void draw();  // Draw scenery like a background for example.

};

#endif // !SCENARIO_BASE_H