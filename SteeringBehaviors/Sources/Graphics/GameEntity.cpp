#include "stdafx.h"

#include "SFML\Graphics\Shape.hpp"

#include "GameEntity.h"
#include "GameWorld.h"

namespace SteeringBehaviors
{
namespace Graphics
{
GameEntity::GameEntity( GameWorld* gameWorld, const Math::Vector2& position, float radius )
	: m_gameWorld{ gameWorld }, m_position{ position }, m_radius{ radius }
{
}

GameEntity::~GameEntity() = default;

void GameEntity::markAsDeleted()
{
	m_toDelete = true;
}

Math::Vector2 GameEntity::getPosition() const
{
	return m_position;
}

sf::Shape* GameEntity::getGraphicalBody() const
{
	return m_graphicalBody.get();
}

GameWorld* GameEntity::getWorld()
{
	return m_gameWorld;
}

float GameEntity::getRadius()
{
	return m_radius;
}

bool GameEntity::isTagged() const
{
	return m_tagged;
}

void GameEntity::tag()
{
	m_tagged = true;
}

void GameEntity::untag()
{
	m_tagged = false;
}

bool GameEntity::toDelete() const
{
	return m_toDelete;
}

} // namespace Graphics
} // namespace SteeringBehaviors
