#include "SFML\Graphics\Shape.hpp"

#include "GameEntity.h"
#include "GameWorld.h"

namespace SteeringBehaviors
{
namespace Graphics
{
GameEntity::GameEntity( GameWorld* gameWorld, float maxForceValue )
	: m_gameWorld{ gameWorld }, m_maxForceValue{ maxForceValue }
{
}

GameEntity::~GameEntity() = default;

} // namespace Graphics
} // namespace SteeringBehaviors
