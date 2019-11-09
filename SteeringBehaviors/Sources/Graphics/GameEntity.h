#pragma once

#include "Renderable.h"
#include "Updatable.h"
#include "../Input/EventHandling.h"
#include "../Input/InputHandling.h"

namespace SteeringBehaviors
{
namespace Graphics
{
class GameEntity : public Updatable, public Renderable, public Input::EventHandling, public Input::InputHandling
{
public:
	virtual ~GameEntity()	= default;
	virtual void init()		= 0;
	virtual void teardown() = 0;
};

} // namespace Graphics
} // namespace SteeringBehaviors
