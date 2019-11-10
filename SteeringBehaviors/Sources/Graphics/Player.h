#pragma once

#include "GameEntity.h"
#include <memory>

namespace sf
{
class Shape;
}
namespace SteeringBehaviors
{
namespace Graphics
{
using sf::Shape;
class GameWorld;
class Player : public GameEntity
{
public:
	Player( GameWorld* gameWorld, Vec position, float maxSpeed );
	virtual ~Player();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;

	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void clampScreenPosition() override;
	virtual void handleKeyboard();
	virtual void handleMouse();

	void move( float deltaTime );
	void rotate( const Vec& targetDirection );

	virtual void calculateVelocity();

private:
	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };
};

} // namespace Graphics
} // namespace SteeringBehaviors
