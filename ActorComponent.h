#ifndef ACTOR_COMPONENT_H
#define ACTOR_COMPONENT_H

class Actor_Base;

enum class ActorComponentType {
	Hitbox,
	Ai,
	Sprite,
	Text
};


class ActorComponent {

public:
	void setIsDrawn(const bool& t_idDrawn);
	const bool& isDrawn()const;

	virtual void update(Actor_Base* t_owner) {}; // Update any functionality
	virtual void reset() {};	// General reset; optional

	virtual void draw() {}; // Draw on screen command
	virtual void clear() {}; // Clear from screen
protected:
	bool m_isDrawn; // Whether this component is drawn or not

};

#endif // !ACTOR_COMPONENT_BASE_H