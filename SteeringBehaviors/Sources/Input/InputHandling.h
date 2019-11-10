#pragma once

namespace sf
{
class Event;
} // namespace sf
namespace SteeringBehaviors
{
namespace Input
{
class InputHandling
{
public:
	virtual ~InputHandling()					   = default;
	virtual void processInput()					   = 0;
	virtual void processEvents( sf::Event& event ) = 0;
};
} // namespace Input
} // namespace SteeringBehaviors
