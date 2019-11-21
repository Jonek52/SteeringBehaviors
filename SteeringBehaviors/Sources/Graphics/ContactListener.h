#pragma once

#include "Box2D/Dynamics/b2WorldCallbacks.h"

#include <iostream>

class b2Contact;
namespace SteeringBehaviors
{
namespace Graphics
{
class ContactListener : public b2ContactListener
{
	void BeginContact( b2Contact* contact ) override;
};

} // namespace Graphics
} // namespace SteeringBehaviors
