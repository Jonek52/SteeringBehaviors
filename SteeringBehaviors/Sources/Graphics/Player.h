#pragma once

#include "GameEntity.h"
#include <memory>

#include "chrono"
#include <array>

namespace sf
{
class Shape;
class Vertex;
} // namespace sf

namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld;
class Player : public GameEntity
{
public:
	Player( GameWorld* gameWorld, float maxSpeed );
	virtual ~Player();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update( std::chrono::milliseconds delta ) override;

	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void initGfxPart() override;
	virtual void initPhysicalPart() override;

	virtual void wrapScreenPosition() override;
	virtual void handleKeyboard();
	virtual void handleMouse();
	virtual void handleShooting( std::chrono::milliseconds delta );
	void move( float deltaTime );
	void rotate();

	virtual void applyForce();
	virtual void shootBall();

private:
	void wait();

private:
	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };
	Vec m_targetDirection;
	Vec m_mousePosition;

	bool canShoot{ true };
	bool wantsToShoot{ false };
	bool onCooldown{ false };

	bool m_drawLine{ false };
	std::array< sf::Vertex, 2 > m_line;

	std::chrono::steady_clock::time_point m_counter;

	struct Ball
	{
		std::unique_ptr< sf::CircleShape > graphicalBody;
		b2Body* physicalBody;
		float forceApplied{ 60.0f };
	} m_ball;
};

} // namespace Graphics
} // namespace SteeringBehaviors
