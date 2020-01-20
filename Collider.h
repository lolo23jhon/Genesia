#ifndef COLLIDER_H
#define COLLIDER_H

#include <unordered_map>
#include <functional>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Actor_Base.h"


enum class ColliderType {
	AABB,
	CIRCLE
};

class Collider {
protected:
	Actor_Base* m_owner;
	sf::FloatRect m_aabb;
	ColliderType m_colliderType; // Used to avoid using RTTI on collision resolution

public:
	Collider( Actor_Base* t_owner,const sf::Vector2f& t_pos, const sf::Vector2f& t_size, bool t_isPosCenter = true);
	virtual void update(Actor_Base* t_owner,const sf::Vector2f& t_pos, const sf::Vector2f& t_size, bool t_isPosCenter = true);
	
	virtual void draw(sf::RenderWindow& t_window);
	
	void setSize(const float& t_width, const float& t_height);
	void setSize(const sf::Vector2f& t_size);
	void setTopLeftPos(const float& t_x, const float& t_y);
	void setTopLeftPos(const sf::Vector2f& t_pos);
	void setCenterPos(const float& t_x, const float& t_y);
	void setCenterPos(const sf::Vector2f& t_pos);
	sf::Vector2f getTopLeftPos()const;
	sf::Vector2f getCenterPos()const;
	const ColliderType& getColliderType()const;
	const sf::FloatRect getAABB()const;
	const Actor_Base* getOwner()const;
	Actor_Base* getOwner();

	virtual bool checkCollision(const Collider* t_otherAABB)const; // Collision check for two aabbs

};

#endif // !COLLIDER_H