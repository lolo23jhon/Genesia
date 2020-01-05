#include "Scenario_Base.h"

////////////////////////////////////////////////////////////
Scenario_Base::Scenario_Base(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms) :
	m_context{ t_context }, m_maxNumActors{ t_maxNumActors }, m_initialNumOrganisms{ t_initialNumOrganisms }{}

////////////////////////////////////////////////////////////
const unsigned& Scenario_Base::getMaxNumActors()const { return m_maxNumActors; }

////////////////////////////////////////////////////////////
void Scenario_Base::setMaxNumActors(const unsigned& t_num) { m_maxNumActors = t_num; }

////////////////////////////////////////////////////////////
const unsigned& Scenario_Base::getInitialNumOrganisms()const { return m_initialNumOrganisms; }
