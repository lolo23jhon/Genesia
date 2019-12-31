#ifndef ACTOR_COMPONENT_H
#define ACTOR_COMPONENT_H

#include <SFML/System/Vector2.hpp>

class Actor_Base;

enum class ActorComponentType {
	Box,
	Ai,
	Sprite,
	Text
};


class ActorComponent_Base {
public:
	virtual void update(Actor_Base* t_owner) = 0; // Update any functionality
	virtual void reset(Actor_Base* t_owner)	 = 0; // General reset; optional
	virtual void draw(Actor_Base* t_owner)   = 0; // Draw on screen command

	static bool isComponentDrawable(const ActorComponentType& t_componentId);

};


class ActorComponent_Drawable : public ActorComponent_Base {

protected:
	bool m_visible; // Whether this component is drawn or not
	sf::Vector2f m_positionOffsetFromOwner; // Offsets the relative origin of the component from the owners center
	float m_angularOffsetFromOwner; // If 0, has the same rotation as owner, it serves as delta [0 360) deg

public:
	ActorComponent_Drawable();
	void setVisibility(const bool& t_isVisible);
	bool getVisibility()const;
	void setPositionOffsetFromOwner(const float& t_x, const float& t_y);
	void setPositionOffsetFromOwner(const sf::Vector2f& t_offset);
	void setPolarPositionOffsetFromOwner(const float& t_distance, const float& t_angleDeg);
	void incrementPolarDistanceFromOwner(const float& t_dist); // Increment polar length from parent node
	void incrementPositionOffsetFromOwner(const float& t_x, const float& t_y);
	void incrementPositionOffsetFromOwner(const sf::Vector2f& t_deltaOffset);
	void setAngularOffsetFromOwner(const float& t_angDeg);
	const float& getAngularOffsetFromOwner()const;
	sf::Vector2f getAboslutePosition(Actor_Base* t_owner)const;
	float getAbsoluteRotation(Actor_Base* t_owner)const;

	void update(Actor_Base* t_owner) {};
	void reset(Actor_Base* t_owner) {};
	void draw(Actor_Base* t_owner) {};
};

#endif // !ACTOR_COMPONENT_BASE_H