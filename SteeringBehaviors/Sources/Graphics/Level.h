#pragma once
#include "GameEntity.h"

#include <vector>
#include <memory>

namespace sf
{
class RenderWindow;
}
namespace SteeringBehaviors
{
namespace Graphics
{
class Level : public GameEntity
{
public:
	Level()	 = default;
	~Level() = default;

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;
	virtual void processEvents( Event& event ) override;
	virtual void initGameEntities();

private:
	std::vector< std::unique_ptr< GameEntity > > m_gameEntities;
};

} // namespace Graphics
} // namespace SteeringBehaviors
