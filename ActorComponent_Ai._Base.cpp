#include <exception>
#include <sstream>
#include <string>
#include "ActorComponent_Ai_Base.h"
#include "ResourceHolder.h"
#include "Actor_Base.h"
#include "MathHelpers.h"


////////////////////////////////////////////////////////////
ActorComponent_Ai_Base::ActorComponent_Ai_Base(SharedContext& t_context, const float& t_movementSpeed, const float& t_rotationSpeed) :
	m_movementSpeed{ t_movementSpeed }, m_rotationSpeed{ t_rotationSpeed }{}


////////////////////////////////////////////////////////////
ActorComponent_Ai_Base::ActorComponent_Ai_Base(SharedContext& t_context, std::stringstream& t_stream) {
	std::string rot_speed, mov_speed;
	if (!(t_stream >> rot_speed >> mov_speed)) {
		throw(std::runtime_error("ActorComponent_Ai::ActorComponent_Ai(SharedContext&, std::stringstream&)"));
	}
	m_movementSpeed = std::stof(mov_speed);
	m_rotationSpeed = std::stof(rot_speed);
}

////////////////////////////////////////////////////////////
const float& ActorComponent_Ai_Base::getRotationSpeed()const { return m_rotationSpeed; }

////////////////////////////////////////////////////////////
void ActorComponent_Ai_Base::setRotationSpeed(const float& t_rotationSpeed) { m_rotationSpeed = t_rotationSpeed; }

////////////////////////////////////////////////////////////
const float& ActorComponent_Ai_Base::getMovementSpeed()const { return m_movementSpeed; }

////////////////////////////////////////////////////////////
void ActorComponent_Ai_Base::setMovementSpeed(const float& t_movementSpeed) { m_movementSpeed = t_movementSpeed; }

////////////////////////////////////////////////////////////
void ActorComponent_Ai_Base::move(Actor_Base* t_owner, const float& t_relativeDirectionDegrees) {
	float rot{ mat::normalizeAngle(t_owner->getRotation() + t_relativeDirectionDegrees) };
	float dx{ 0 };
	float dy{ 0 };
	mat::to_cartesian(m_movementSpeed, rot, dx, dy);
	t_owner->incrementPosition(dx, dy);
}

////////////////////////////////////////////////////////////
void ActorComponent_Ai_Base::rotate(Actor_Base* t_owner, bool t_leftRight) {
	// Clockwise angle: left = false = +angle; right = true = -angle
	t_owner->rotate(t_leftRight ? m_rotationSpeed : -m_rotationSpeed);
}