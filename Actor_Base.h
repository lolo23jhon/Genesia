#ifndef ACTOR_BASE_H
#define ACTOR_BASE_H

#include <memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "CollisionManager.h"

struct SharedContext;
class Actor_Base;
using ActorPtr = std::unique_ptr<Actor_Base>;

class Actor_Base {

protected:
	sf::Vector2f m_position;
	float m_rotation; // [0 360)
	sf::Sprite m_sprite;
	sf::Color m_color;
	sf::Text m_text;
	Collider m_collider;
	SharedContext& m_context;
	bool m_isSpriteVisible;
	bool m_isTextVisible;



	bool m_destroy; // If the actor shold be removed from the system on the nect tick

public:
	Actor_Base(
		SharedContext& t_context,
		const sf::Vector2f& t_position,
		const float& t_rotation,
		const sf::Color& t_color,
		const std::string& t_texture,
		const sf::IntRect& t_spriteRect,
		bool t_isSpriteVisible = true,
		bool t_isTextVisible = true);
	const sf::Vector2f& getPosition()const;
	void setPosition(const sf::Vector2f& t_position);
	const float& getRotation()const;
	void setRotation(const float& t_rotation);
	const sf::Color& getColor()const;
	void setColor(const sf::Color& t_color);
	const sf::Sprite& getSprite()const;
	void setSprite(const sf::Sprite& t_sprite);
	void setSprite(const std::string& t_texture, const sf::IntRect t_spriteRect = sf::IntRect());
	bool getIsSpriteVisible()const;
	void setIsSpriteVisible(bool t_visible);
	bool isTextVisible()const;
	void setIsTextVisible(bool t_visible);
	bool shouldBeDestroyed()const;
	void setShouldBeDestroyed(bool t_destroy);
	void setTextString(const std::string& t_str);
	std::string getTextString()const;
	SharedContext& getContext();
	const SharedContext& getContext() const;
	Collider& getCollider();
	const Collider& getCollider()const;

	virtual void move(const float& t_dx, const float& t_dy);
	virtual void rotate(const float& t_deg);

	virtual void update(const float& t_elapsed);
	virtual void updateCollider() = 0;
	virtual void draw();

	virtual ActorPtr clone();
	virtual void onDestruction(SharedContext& t_context); // Anything that happens when the actor is aihiated i. g. spawning something
};
#endif // !ACTOR_BASE_H
