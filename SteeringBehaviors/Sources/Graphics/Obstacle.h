#pragma once

#include "GameEntity.h"

namespace SteeringBehaviors::Graphics
{
class Obstacle : public GameEntity
{
public:
	Obstacle( GameWorld* gameWorld, Math::Vector2 position, float radius );
	virtual ~Obstacle();

	virtual void init() override;
	virtual void teardown() override;
	virtual void update( float delta ) override;
	virtual void render( RenderWindow* ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void initGfxPart() override;
	virtual void initPhysicalPart() override;
	virtual void wrapScreenPosition() override;

private:
};

} // namespace SteeringBehaviors::Graphics
