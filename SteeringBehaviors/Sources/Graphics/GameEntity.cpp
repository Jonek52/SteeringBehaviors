#include "SFML\Graphics\Shape.hpp"

#include "Box2D\Dynamics\b2Body.h"

#include "GameEntity.h"
#include "GameWorld.h"

namespace SteeringBehaviors
{
namespace Graphics
{
GameEntity::GameEntity( GameWorld* gameWorld, float maxForceValue, uint16 collisionCategory, uint16 collisionMask )
	: m_gameWorld{ gameWorld },
	  m_physicalBody{ nullptr },
	  m_collisionCategory{ collisionCategory },
	  m_collisionMask{ collisionMask },
	  m_lookDirection{ 0.0f, -1.0f },
	  m_sideDirection{ 1.0f, 0.0f },
	  m_maxForceValue{ maxForceValue }
{
}

GameEntity::~GameEntity() = default;

} // namespace Graphics
} // namespace SteeringBehaviors
