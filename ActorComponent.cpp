#include "ActorComponent.h"

////////////////////////////////////////////////////////////
void ActorComponent::setIsDrawn(const bool& t_idDrawn) { m_isDrawn = t_idDrawn; }

////////////////////////////////////////////////////////////
const bool& ActorComponent::isDrawn()const { return m_isDrawn; }