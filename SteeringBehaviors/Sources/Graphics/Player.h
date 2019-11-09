#pragma once

#include "GameEntity.h"
#include "SFML\System\Vector2.hpp"

#include <memory>

namespace sf
{
class CircleShape;
}
namespace SteeringBehaviors
{
namespace Graphics
{
using sf::CircleShape;
class Player : public GameEntity
{
public:
	explicit Player( sf::Vector2< float > position );
	virtual ~Player();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;

	virtual void calculateVelocity();

	virtual void processEvents( Event& event ) override;
	virtual void processInput();
	virtual void handleKeyboard();
	virtual void handleMouse();

private:
	std::unique_ptr< CircleShape > m_playerShape;

	sf::Vector2< float > m_velocity;
	sf::Vector2< float > m_position;
	sf::Vector2< float > m_lookDirection;

	const float m_maxSpeed;

	bool m_isDirty{ false };

	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };
};

} // namespace Graphics
} // namespace SteeringBehaviors
