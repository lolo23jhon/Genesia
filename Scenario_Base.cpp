#include "Scenario_Base.h"
#include "SharedContext.h"
#include "Engine.h"

////////////////////////////////////////////////////////////
Scenario_Base::Scenario_Base(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms, const float& t_width, const float& t_height) :
	m_context{ t_context },
	m_maxNumActors{ t_maxNumActors },
	m_initialNumOrganisms{ t_initialNumOrganisms },
	m_simulationRectangle{ 0.f,0.f,t_width,t_height },
	m_simulationBackground{ {t_width, t_height} }
{
	m_simulationBackground.setFillColor({ 220,220,220 });
	m_simulationBackground.setPosition(m_simulationRectangle.left, m_simulationRectangle.top);
}

////////////////////////////////////////////////////////////
const unsigned& Scenario_Base::getMaxNumActors()const { return m_maxNumActors; }

////////////////////////////////////////////////////////////
void Scenario_Base::setMaxNumActors(const unsigned& t_num) { m_maxNumActors = t_num; }

////////////////////////////////////////////////////////////
const unsigned& Scenario_Base::getInitialNumOrganisms()const { return m_initialNumOrganisms; }

////////////////////////////////////////////////////////////
void Scenario_Base::update(const float& t_elapsed) {

	// Keep all the actors within the boudaries of the simation; Loo´p around when outside it
	m_context.m_engine->actorsForEach(
		[this](ActorPtr& t_actor) {
			auto p{ t_actor->getPosition() };
			const auto& min_x{ m_simulationRectangle.left };
			float max_x{ m_simulationRectangle.left + m_simulationRectangle.width };
			const auto& min_y{ m_simulationRectangle.top };
			float max_y{ m_simulationRectangle.top + m_simulationRectangle.height };

			bool isLoop{ false };

			// Wrap around x axis
			if (p.x > max_x) {
				p.x = min_x;
				isLoop = true;
			}
			else if (p.x < min_x) {
				p.x = max_x;
				isLoop = true;
			}

			// Wrap around y axis
			if (p.y > max_y) {
				p.y = min_y;
				isLoop = true;
			}
			else if (p.y < min_y) {
				p.y = max_y;
				isLoop = true;
			}

			if (isLoop) { t_actor->setPosition(p); }
		}
	);
}

////////////////////////////////////////////////////////////
void Scenario_Base::draw() {
	m_context.m_engine->getWindow().draw(m_simulationBackground);
}