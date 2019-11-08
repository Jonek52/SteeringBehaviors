#pragma once

namespace SteeringBehaviors
{
namespace Graphics
{
class Updatable
{
public:
	virtual ~Updatable()				   = default;
	virtual void update( float deltaTime ) = 0;
};

} // namespace Graphics
} // namespace SteeringBehaviors
