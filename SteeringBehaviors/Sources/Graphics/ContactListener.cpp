#include "ContactListener.h"
#include "Enemy.h"
#include "Player.h"
#include "GameWorld.h"

#include "Box2D/Dynamics/Contacts/b2Contact.h"

namespace SteeringBehaviors
{
namespace Graphics
{
void ContactListener::BeginContact( b2Contact* contact )
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	GameWorld::CollisionCategory bodyAId = ( GameWorld::CollisionCategory )( ( int )bodyAUserData );
	GameWorld::CollisionCategory bodyBId = ( GameWorld::CollisionCategory )( ( int )bodyBUserData );

	if( bodyAId == GameWorld::PLAYER && bodyBId == GameWorld::ENEMY )
	{
		std::cout << "Player hit enemy\n";
	}

	if( bodyAId == GameWorld::BALL && bodyBId == GameWorld::ENEMY )
	{
		std::cout << "Ball hit enemy\n";
	}
}

} // namespace Graphics
} // namespace SteeringBehaviors
