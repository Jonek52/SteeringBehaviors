#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

namespace SteeringBehaviors::Math
{

struct Vector2
{
	Vector2( float x, float y );
	Vector2();

	Vector2( const Vector2& ) = default;
	Vector2( Vector2&& )	  = default;

	Vector2& operator=( const Vector2& ) = default;
	Vector2& operator=( Vector2&& ) = default;

	Vector2 operator+( const Vector2& v ) const;

	Vector2 operator-( const Vector2& v ) const;

	Vector2 operator*( float value ) const;

	friend Vector2 operator*( float value, const Vector2& v )
	{
		return v * value;
	}

	Vector2 operator/( float value ) const;

	Vector2& operator+=( const Vector2& v );

	Vector2& operator-=( const Vector2& v );

	Vector2& operator*=( float val );

	Vector2 operator/=( float val );

	Vector2 perp() const;

	Vector2& normalize();

	float length() const;

	float lengthSquared() const;

	float dot( const Vector2& v ) const;

	Vector2& truncate( float val );

	void zero();

	friend float dot( const Vector2& v0, const Vector2& v1 )
	{
		return v0.dot( v1 );
	}

	float x;
	float y;
};

Vector2 normalize( const Vector2& v );

float vecDistance( const Vector2& v0, const Vector2& v1 );
float vecDistanceSquared( const Vector2& v0, const Vector2& v1 );

} // namespace SteeringBehaviors::Math
