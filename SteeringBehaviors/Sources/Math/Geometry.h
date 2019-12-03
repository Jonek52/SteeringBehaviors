#pragma once

#include "Vector2.h"

namespace SteeringBehaviors::Math
{
inline bool DoRayCircleIntersect( Vector2 RayOrigin, Vector2 RayHeading, Vector2 CircleOrigin, float radius )
{

	Vector2 ToCircle = CircleOrigin - RayOrigin;
	float length	 = ToCircle.length();
	float v			 = ToCircle.dot( RayHeading );
	float d			 = radius * radius - ( length * length - v * v );

	return ( d < 0.0 );
}

inline bool LineIntersection2D( Vector2 A, Vector2 B, Vector2 C, Vector2 D, float& dist, Vector2& point )
{

	float rTop = ( A.y - C.y ) * ( D.x - C.x ) - ( A.x - C.x ) * ( D.y - C.y );
	float rBot = ( B.x - A.x ) * ( D.y - C.y ) - ( B.y - A.y ) * ( D.x - C.x );

	float sTop = ( A.y - C.y ) * ( B.x - A.x ) - ( A.x - C.x ) * ( B.y - A.y );
	float sBot = ( B.x - A.x ) * ( D.y - C.y ) - ( B.y - A.y ) * ( D.x - C.x );

	if( ( rBot == 0 ) || ( sBot == 0 ) )
	{
		return false;
	}

	float r = rTop / rBot;
	float s = sTop / sBot;

	if( ( r > 0 ) && ( r < 1 ) && ( s > 0 ) && ( s < 1 ) )
	{
		dist = distance( A, B ) * r;

		point = A + r * ( B - A );

		return true;
	}

	else
	{
		dist = 0;

		return false;
	}
}

} // namespace SteeringBehaviors::Math
