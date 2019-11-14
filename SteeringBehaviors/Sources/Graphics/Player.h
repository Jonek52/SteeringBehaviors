#pragma once

#include "GameEntity.h"
#include <memory>

namespace sf
{
class Shape;
}
class b2Body;

namespace SteeringBehaviors
{
namespace Graphics
{
using sf::Shape;
class GameWorld;
class Player : public GameEntity
{
public:
	Player( GameWorld* gameWorld, float maxSpeed );
	virtual ~Player();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update() override;

	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void wrapScreenPosition() override;
	virtual void handleKeyboard();
	virtual void handleMouse();

	void move( float deltaTime );
	void rotate();

	virtual void applyForce();

private:
	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };
	Vec m_targetDirection;
};

} // namespace Graphics
} // namespace SteeringBehaviors
