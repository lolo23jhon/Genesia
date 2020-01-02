#include "SpriteSheet.h"

////////////////////////////////////////////////////////////
SpriteSheet::SpriteSheet(TextureManager* t_textureMgr) :
	m_textureManager{ t_textureMgr },
	m_animationCurrent{ nullptr },
	m_spriteScale{ 1.f,1.f }{}


////////////////////////////////////////////////////////////
SpriteSheet::~SpriteSheet() { releaseSheet(); }


////////////////////////////////////////////////////////////
void SpriteSheet::releaseSheet() {
	m_textureManager->releaseResource(m_texture);
	m_animationCurrent = nullptr;
	m_animations.clear();
}

////////////////////////////////////////////////////////////
sf::Vector2u SpriteSheet::getSpriteSize()const { return m_spriteSize; }


////////////////////////////////////////////////////////////
sf::Vector2f SpriteSheet::getSpritePosition()const { return m_sprite.getPosition(); }


////////////////////////////////////////////////////////////
Anim_Base* SpriteSheet::getCurrentAnim() { return m_animationCurrent; }


////////////////////////////////////////////////////////////
void SpriteSheet::setSpriteSize(const sf::Vector2u& t_size) {
	m_spriteSize = t_size;
	m_sprite.setOrigin(static_cast<float>(m_spriteSize.x) / 2, static_cast<float>(m_spriteSize.y));
}

////////////////////////////////////////////////////////////
void SpriteSheet::setSpritePosition(const sf::Vector2f& t_pos) {
	m_sprite.setPosition(t_pos);
}


////////////////////////////////////////////////////////////
void SpriteSheet::setSheetPadding(const sf::Vector2f& t_padding) {
	m_sheetPadding = t_padding;
}

////////////////////////////////////////////////////////////
void SpriteSheet::setSpriteSpacing(const sf::Vector2f& t_spacing) {
	m_spriteSpacing = t_spacing;
}

////////////////////////////////////////////////////////////
sf::Vector2f SpriteSheet::getSheetPadding()const {
	return m_sheetPadding;
}

////////////////////////////////////////////////////////////
sf::Vector2f SpriteSheet::getSpriteSpacing()const {
	return m_spriteSpacing;
}

////////////////////////////////////////////////////////////
sf::FloatRect SpriteSheet::getSpriteBounds()const {
	return m_sprite.getGlobalBounds();
}

////////////////////////////////////////////////////////////
void SpriteSheet::cropSprite(const sf::IntRect& t_rect) {
	m_sprite.setTextureRect(t_rect);
}


////////////////////////////////////////////////////////////
bool SpriteSheet::loadSheet(const std::string& t_file) {
	std::ifstream sheet;
	sheet.open(utilities::getWorkingDirectory() + t_file);
	if (!sheet.is_open()) {
		std::cout << "! Failed loading spritesheet: " << t_file << std::endl;
		return false;
	}

	// Release current spritesheet resources
	releaseSheet();
	std::string line;
	while (std::getline(sheet, line)) {
		if (line[0] == '|') { continue; }
		std::stringstream keystream{ line };
		std::string type;
		keystream >> type;
		if (type == "Texture") {
			if (!m_texture.empty()) {
				std::cout << "! Duplicate texture entires in: " << t_file << std::endl;
				continue;
			}
			std::string texture;
			keystream >> texture;
			if (!m_textureManager->requireResource(texture)) {
				std::cout << "! Could not set up the texture: " << texture << std::endl;
				continue;
			}
			m_texture = texture;
			m_sprite.setTexture(*m_textureManager->getResource(m_texture));
		}
		else if (type == "Size") {
			keystream >> m_spriteSize.x >> m_spriteSize.y;
			setSpriteSize(m_spriteSize);
		}
		else if (type == "Scale") {
			keystream >> m_spriteScale.x >> m_spriteScale.y;
			m_sprite.setScale(m_spriteScale);
		}
		else if (type == "Padding") {
			keystream >> m_sheetPadding.x >> m_sheetPadding.y;
		}
		else if (type == "Spacing") {
			keystream >> m_spriteSpacing.x >> m_spriteSpacing.y;
		}
		else if (type == "Animation") {
			std::string name;
			keystream >> name;
			if (m_animations.find(name) != m_animations.end()) {
				std::cerr << "! Duplicate animation (" << name << ") in :" << t_file << std::endl;
				continue;
			}
			std::unique_ptr<Anim_Base> anim;

			keystream >> *anim;
			anim->setSpriteSheet(this);
			anim->setName(name);
			anim->reset();
			if (!m_animationCurrent) {
				m_animationCurrent = anim.get();
				m_animationCurrent->play();
			}
			m_animations.emplace(name, std::move(anim));
		}
	}
	sheet.close();
	return true;
}

////////////////////////////////////////////////////////////
bool SpriteSheet::setAnimation(const std::string& t_name, bool t_play = false, bool t_loop = false) {
	auto it{ m_animations.find(t_name) };
	if (it == m_animations.end()) { return false; }
	if (it->second.get() == m_animationCurrent) { return false; }
	if (m_animationCurrent) {
		m_animationCurrent->stop();
	}
	m_animationCurrent = it->second.get();
	m_animationCurrent->setLooping(t_loop);
	if (t_play) { m_animationCurrent->play(); }
	m_animationCurrent->cropSprite();
	return true;
}

////////////////////////////////////////////////////////////
void SpriteSheet::update(float t_dt) {
	m_animationCurrent->update(t_dt);
}

////////////////////////////////////////////////////////////
void SpriteSheet::draw(sf::RenderWindow* t_wind) {
	t_wind->draw(m_sprite);
}