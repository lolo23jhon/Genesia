#include "Actor_Base.h"
#include "MathHelpers.h"
#include "SharedContext.h"
#include "Utilities.h"
#include "ResourceHolder.h"

static const std::string S_DEFAULT_FONT{ "Font_helvetica" };
static const sf::Color S_DEFAULT_TEXT_FILL_COLOR{ 25,25,25 };
static const sf::Color S_DEFAILT_TEXT_OUTILINE_COLOR{ 250,250,250 };
static const unsigned S_DEFAULT_TEXT_SIZE{ 2U };


////////////////////////////////////////////////////////////
Actor_Base::Actor_Base(
	SharedContext& t_context,
	const sf::Vector2f& t_position,
	const float& t_rotation,
	const sf::Color& t_color,
	const std::string& t_texture,
	const sf::IntRect& t_spriteRect,
	bool t_isSpriteVisible,
	bool t_isTextVisible) :
	m_context{ t_context },
	m_position{ t_position },
	m_rotation{ t_rotation },
	m_color{ t_color },
	m_isSpriteVisible{ t_isSpriteVisible },
	m_isTextVisible{ t_isTextVisible }
{

	// Initialize texture
	Resource* texture_resource{ m_context.m_resourceHolder->getResource(ResourceType::Texture, t_texture) };
	if (!texture_resource) {
		throw(std::runtime_error(std::string("Organism::Organims: Texture " + t_texture + " was nullptr!")));
	}
	m_sprite.setTexture(std::get<sf::Texture>(*texture_resource),true);
	if (t_spriteRect.width && t_spriteRect.height) {
		m_sprite.setTextureRect(t_spriteRect);

	}
	auto& spriteSize{ m_sprite.getTextureRect() };
	m_sprite.setOrigin(static_cast<float>(spriteSize.width) / 2, static_cast<float>(spriteSize.height) / 2);

	// Initialize text
	Resource* font_resource{ m_context.m_resourceHolder->getResource(ResourceType::Font, S_DEFAULT_FONT) };
	if (!font_resource) {
		throw(std::runtime_error(std::string("Organism::Organims: Font " + S_DEFAULT_FONT + " was nullptr!")));
	}
	m_text.setFont(std::get<sf::Font>(*font_resource));
	m_text.setFillColor(S_DEFAULT_TEXT_FILL_COLOR);
	m_text.setOutlineColor(S_DEFAILT_TEXT_OUTILINE_COLOR);

	update(); // Set sprite and text position
}

////////////////////////////////////////////////////////////
const sf::Vector2f& Actor_Base::getPosition()const { return m_position; }

////////////////////////////////////////////////////////////
void Actor_Base::setPosition(const sf::Vector2f& t_position) { m_position = t_position; }

////////////////////////////////////////////////////////////
const float& Actor_Base::getRotation()const { return m_rotation; }

////////////////////////////////////////////////////////////
void Actor_Base::setRotation(const float& t_rotation) {
	m_rotation = mat::normalizeAngle(t_rotation);
}

////////////////////////////////////////////////////////////
const sf::Color& Actor_Base::getColor()const { return m_color; }

////////////////////////////////////////////////////////////
void Actor_Base::setColor(const sf::Color& t_color) { m_color = t_color; }

////////////////////////////////////////////////////////////
const sf::Sprite& Actor_Base::getSprite()const { return m_sprite; }

////////////////////////////////////////////////////////////
void Actor_Base::setSprite(const sf::Sprite& t_sprite) { m_sprite = t_sprite; }

////////////////////////////////////////////////////////////
void Actor_Base::setSprite(const std::string& t_texture, const sf::IntRect t_spriteRect) {
	Resource* texture_resource{ m_context.m_resourceHolder->getResource(ResourceType::Texture, t_texture) };
	if (!texture_resource) {
		throw(std::runtime_error(std::string("Organism::Organims: Texture " + t_texture + " was nullptr!")));
	}
	auto& texture{ std::get<sf::Texture>(*texture_resource) };
	m_sprite.setTexture(texture, false);
	m_sprite.setTextureRect(t_spriteRect);
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(m_rotation);
}

////////////////////////////////////////////////////////////
bool Actor_Base::getIsSpriteVisible()const { return m_isSpriteVisible; }

////////////////////////////////////////////////////////////
void Actor_Base::setIsSpriteVisible(bool t_visible) { m_isSpriteVisible = t_visible; }

////////////////////////////////////////////////////////////
bool Actor_Base::isTextVisible()const { return m_isTextVisible; }

////////////////////////////////////////////////////////////
void Actor_Base::setIsTextVisible(bool t_visible) { m_isTextVisible = t_visible; }

////////////////////////////////////////////////////////////
SharedContext& Actor_Base::getContext() {return m_context;}

////////////////////////////////////////////////////////////
const SharedContext& Actor_Base::getContext() const { return m_context; }


////////////////////////////////////////////////////////////
void Actor_Base::draw() {
	if (m_isSpriteVisible) {
		m_context.m_window->draw(m_sprite);
	}
	if (m_isTextVisible) {
		m_context.m_window->draw(m_text);
	}
}

void Actor_Base::update() {
	// Update sprite
	m_sprite.setRotation(m_rotation);
	m_sprite.setPosition(m_position);
	m_sprite.setColor(m_color);

	// Put text origin in text's center (just in case it changes of string; remember this is a base)
	utilities::centerSFMLText(m_text);

	// Put text above sprite (-y = up; +y = down):
	m_text.setPosition(m_position.x, m_position.y - (m_sprite.getGlobalBounds().height*1.4 + utilities::getSFMLTextMaxHeight(m_text))/2); // 
}

////////////////////////////////////////////////////////////
void Actor_Base::move(const float& t_dx, const float& t_dy) {
	m_position.x += t_dx;
	m_position.y += t_dy;
}

////////////////////////////////////////////////////////////
void Actor_Base::rotate(const float& t_deg, bool t_leftRight) {
	m_rotation = mat::normalizeAngle(m_rotation + (t_leftRight ? -t_deg : t_deg));// Clockwise angle -> true = right = +angle; false = left = -angle
}


////////////////////////////////////////////////////////////
ActorPtr Actor_Base::clone() {
	return std::make_unique<Actor_Base>(*this);
}
