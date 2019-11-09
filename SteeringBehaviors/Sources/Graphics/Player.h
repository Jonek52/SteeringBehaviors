#pragma once

#include "GameEntity.h"
#include "Box2D\Common\b2Math.h"

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
	using Vec = b2Vec2;

public:
	explicit Player( Vec position );
	virtual ~Player();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;

	virtual void processEvents( Event& event ) override;
	virtual void processInput( sf::Window* window ) override;

protected:
	virtual void handleKeyboard( sf::Window* window );
	virtual void handleMouse( sf::Window* window );

	void move( float deltaTime );
	void rotate( const Vec& targetDirection );

	virtual void calculateVelocity();

private:
	std::unique_ptr< CircleShape > m_playerShape;

	Vec m_position;
	Vec m_velocity;
	Vec m_lookDirection;

	const float m_maxSpeed;

	bool m_isDirty{ true };

	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };
};

} // namespace Graphics
} // namespace SteeringBehaviors
