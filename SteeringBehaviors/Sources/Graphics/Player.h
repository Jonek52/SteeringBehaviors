#pragma once

#include "GameEntity.h"
#include <memory>

#include "chrono"
#include <array>

#include "SFML/Graphics/Vertex.hpp"

namespace sf
{
class Shape;
class CircleShape;
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
	virtual void update( float delta ) override;

	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void initGfxPart() override;
	virtual void initPhysicalPart() override;

	virtual void wrapScreenPosition() override;
	virtual void handleKeyboard();
	virtual void handleMouse();
	virtual void handleShooting( float delta );
	void move( float delta );
	void rotate();

	virtual void applyForce();
	virtual void shootBall();
	virtual void spawnBall();

private:
	void wait();

private:
	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };

	bool canShoot{ true };
	bool wantsToShoot{ false };
	bool onCooldown{ false };

	bool m_drawLine{ false };
	std::array< sf::Vertex, 2 > m_line;

	std::chrono::steady_clock::time_point m_counter;
};

} // namespace Graphics
} // namespace SteeringBehaviors
