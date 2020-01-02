#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <unordered_map>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Anim_Base.h"
#include "TextureManager.h"
#include "Utilities.h"

using Animations = std::unordered_map < std::string, std::unique_ptr<Anim_Base>>;
class SpriteSheet {

	std::string m_texture;
	sf::Sprite m_sprite;
	sf::Vector2u m_spriteSize;
	sf::Vector2f m_spriteScale;

	sf::Vector2f m_sheetPadding;
	sf::Vector2f m_spriteSpacing;

	Animations m_animations;
	Anim_Base* m_animationCurrent;

	TextureManager* m_textureManager;

public:
	SpriteSheet(TextureManager* t_textureMgr);
	~SpriteSheet();

	void cropSprite(const sf::IntRect& t_rect);
	sf::Vector2u getSpriteSize()const;
	sf::Vector2f getSpritePosition()const;
	void setSpriteSize(const sf::Vector2u& t_size);
	void setSpritePosition(const sf::Vector2f& t_pos);

	void setSheetPadding(const sf::Vector2f& t_padding);
	void setSpriteSpacing(const sf::Vector2f& t_spacing);
	sf::Vector2f getSheetPadding()const;
	sf::Vector2f getSpriteSpacing()const;
	sf::FloatRect getSpriteBounds()const;

	bool loadSheet(const std::string& t_file);
	void releaseSheet();

	Anim_Base* getCurrentAnim();
	bool setAnimation(const std::string& t_name, bool t_play = false, bool t_loop = false);

	void update(float t_dt);
	void draw(sf::RenderWindow* t_wind);

};

#endif // !SPRITE_SHEET_H