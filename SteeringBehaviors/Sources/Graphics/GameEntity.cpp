#include "SFML\Graphics\Shape.hpp"

#include "GameEntity.h"
#include "GameWorld.h"

namespace SteeringBehaviors
{
namespace Graphics
{
GameEntity::GameEntity( GameWorld* gameWorld, Vec position, float maxSpeed )
	: m_gameWorld{ gameWorld },
	  m_position{ position },
	  m_velocity{ 0.0f, 0.0f },
	  m_lookDirection{ 0.0f, -1.0f },
	  m_rightDirection{ 1.0f, 0.0f },
	  m_maxSpeed{ maxSpeed }
{
}

GameEntity::~GameEntity() = default;

} // namespace Graphics
} // namespace SteeringBehaviors
