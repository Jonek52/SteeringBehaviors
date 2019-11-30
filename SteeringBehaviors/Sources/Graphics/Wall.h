#pragma once
#include "GameEntity.h"

namespace sf
{
class Shape;
}

namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld;
class Wall : public GameEntity
{
public:
	enum class Orientation
	{
		HORIZONTAL,
		VERTICAL
	};

	enum class Side
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public:
	Wall( GameWorld* gameWorld, Orientation orientation, Side side );
	~Wall();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update( float delta ) override;

	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;
	virtual void wrapScreenPosition() override;

	virtual Math::Vector2 getFrom() const;
	virtual Math::Vector2 getTo() const;
	virtual Math::Vector2 getNormal() const;

private:
	virtual void initPhysicalPart() override;
	virtual void initGfxPart() override;
	void calculateWallSize();
	void calculateWallPosition();

private:
	Orientation m_orientation;
	Side m_side;

	Math::Vector2 m_normal;
	Math::Vector2 m_from;
	Math::Vector2 m_to;

	inline static constexpr float m_width = 10.0f;
};
} // namespace Graphics

} // namespace SteeringBehaviors
