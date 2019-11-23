#pragma once

#include "SFML/System/Vector2.hpp"

#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

namespace SteeringBehaviors
{
namespace Math
{

inline float randFloat()
{
	return ( ( rand() ) / ( RAND_MAX + 1.0f ) );
}

inline float randInRange( float x, float y )
{
	return x + randFloat() * ( y - x );
}

inline bool randBool()
{
	if( randFloat() > 0.5 )
		return true;

	else
		return false;
}

inline float randomClamped()
{
	return randFloat() - randFloat();
}

inline float toDegrees( float radians )
{
	float inversePI = 1.0f / static_cast< float >( M_PI );
	return radians * ( 180.0f * inversePI );
}

inline float toRadians( float degrees )
{
	float inverse180 = 1.0f / 180.0f;
	return degrees * ( static_cast< float >( M_PI ) * inverse180 );
}

template< typename T = float >
sf::Vector2< T > toSFMLVector( const Vector2& v )
{
	return sf::Vector2< T >( v.x, v.y );
}

template< typename T >
Vector2 toMathVector( const sf::Vector2< T > v )
{
	return Vector2{ static_cast< float >( v.x ), static_cast< float >( v.y ) };
}

} // namespace Math
} // namespace SteeringBehaviors
