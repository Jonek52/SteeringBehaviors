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

inline bool isEqual( float a, float b )
{
	if( fabs( a - b ) < 1E-12 )
	{
		return true;
	}

	return false;
}

inline bool isEqual( double a, double b )
{
	if( fabs( a - b ) < 1E-12 )
	{
		return true;
	}

	return false;
}

inline float Vec2DDistance (const Vector2& v1, const Vector2& v2)
{

	float ySeparation = v2.y - v1.y;
	float xSeparation = v2.x - v1.x;

	return sqrt (ySeparation * ySeparation + xSeparation * xSeparation);
}

inline float Vec2DDistanceSq (const Vector2& v1, const Vector2& v2)
{

	float ySeparation = v2.y - v1.y;
	float xSeparation = v2.x - v1.x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
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
