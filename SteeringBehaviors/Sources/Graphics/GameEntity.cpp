#include "SFML\Graphics\Shape.hpp"

#include "GameEntity.h"
#include "GameWorld.h"

namespace SteeringBehaviors
{
namespace Graphics
{
GameEntity::GameEntity( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position )
	: m_gameWorld{ gameWorld }, m_maxSpeed{ maxSpeed }, m_position{ position }
{
}

GameEntity::~GameEntity() = default;

Math::Vector2 GameEntity::getPosition() const
{
	return m_position;
}

Math::Vector2 GameEntity::getVelocity() const
{
	return m_velocity;
}

Math::Vector2 GameEntity::getLookDirection() const
{
	return m_lookDirection;
}

SteeringBehaviors::Math::Vector2 GameEntity::getSideDirection() const
{
	return m_sideDirection;
}

sf::Shape* GameEntity::getGraphicalBody() const
{
	return m_graphicalBody.get();
}

float GameEntity::getMaxSpeed() const
{
	return m_maxSpeed;
}

float GameEntity::getSpeed() const
{
	return m_velocity.length();
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

} // namespace Graphics
} // namespace SteeringBehaviors
