#ifndef ACTOR_BASE_H
#define ACTOR_BASE_H

#include <memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

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
	SharedContext& m_context;
	bool m_isSpriteVisible;
	bool m_isTextVisible;

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
	const std::string& getTextString()const;
	void setTextSring(const std::string& t_str);
	bool isTextVisible()const;
	void setIsTextVisible(bool t_visible);
	SharedContext& getContext();
	const SharedContext& getContext() const;

	void move(const float& t_dx, const float& t_dy);
	void rotate(const float& t_deg);

	virtual void update();
	virtual void draw();

	virtual ActorPtr clone();
};
#endif // !ACTOR_BASE_H
