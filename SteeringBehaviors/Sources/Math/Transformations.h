#pragma once

#include <vector>

#include "Vector2.h"
#include "Matrix.h"

namespace SteeringBehaviors::Math
{
inline std::vector< Vector2 > WorldTransform( std::vector< Vector2 >& points,
											  const Vector2& pos,
											  const Vector2& forward,
											  const Vector2& side,
											  const Vector2& scale )
{
	std::vector< Vector2 > TranVector2s = points;

	// create a transformation matrix
	C2DMatrix matTransform;

	// scale
	if( ( scale.x != 1.0 ) || ( scale.y != 1.0 ) )
	{
		matTransform.Scale( scale.x, scale.y );
	}

	// rotate
	matTransform.Rotate( forward, side );

	// and translate
	matTransform.Translate( pos.x, pos.y );

	// now transform the object's vertices
	matTransform.TransformVector2s( TranVector2s );

	return TranVector2s;
}

//--------------------------- WorldTransform -----------------------------
//
//  given a std::vector of 2D vectors, a position and  orientation
//  this function transforms the 2D vectors into the object's world space
//------------------------------------------------------------------------
inline std::vector< Vector2 > WorldTransform( std::vector< Vector2 >& points,
											  const Vector2& pos,
											  const Vector2& forward,
											  const Vector2& side )
{
	// copy the original vertices into the buffer about to be transformed
	std::vector< Vector2 > TranVector2s = points;

	// create a transformation matrix
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate( forward, side );

	// and translate
	matTransform.Translate( pos.x, pos.y );

	// now transform the object's vertices
	matTransform.TransformVector2s( TranVector2s );

	return TranVector2s;
}

//--------------------- PointToWorldSpace --------------------------------
//
//  Transforms a point from the agent's local space into world space
//------------------------------------------------------------------------
inline Vector2 PointToWorldSpace( const Vector2& point,
								  const Vector2& AgentHeading,
								  const Vector2& AgentSide,
								  const Vector2& AgentPosition )
{
	// make a copy of the point
	Vector2 TransPoint = point;

	// create a transformation matrix
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate( AgentHeading, AgentSide );

	// and translate
	matTransform.Translate( AgentPosition.x, AgentPosition.y );

	// now transform the vertices
	matTransform.TransformVector2s( TransPoint );

	return TransPoint;
}

//--------------------- VectorToWorldSpace --------------------------------
//
//  Transforms a vector from the agent's local space into world space
//------------------------------------------------------------------------
inline Vector2 VectorToWorldSpace( const Vector2& vec, const Vector2& AgentHeading, const Vector2& AgentSide )
{
	// make a copy of the point
	Vector2 TransVec = vec;

	// create a transformation matrix
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate( AgentHeading, AgentSide );

	// now transform the vertices
	matTransform.TransformVector2s( TransVec );

	return TransVec;
}

//--------------------- PointToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline Vector2 PointToLocalSpace( const Vector2& point,
								  const Vector2& AgentHeading,
								  const Vector2& AgentSide,
								  const Vector2& AgentPosition )
{

	Vector2 TransPoint = point;

	C2DMatrix matTransform;

	float Tx = -AgentPosition.dot( AgentHeading );
	float Ty = -AgentPosition.dot( AgentSide );

	matTransform._11( AgentHeading.x );
	matTransform._12( AgentSide.x );
	matTransform._21( AgentHeading.y );
	matTransform._22( AgentSide.y );
	matTransform._31( Tx );
	matTransform._32( Ty );

	matTransform.TransformVector2s( TransPoint );

	return TransPoint;
}

//--------------------- VectorToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline Vector2 VectorToLocalSpace( const Vector2& vec, const Vector2& AgentHeading, const Vector2& AgentSide )
{

	// make a copy of the point
	Vector2 TransPoint = vec;

	// create a transformation matrix
	C2DMatrix matTransform;

	// create the transformation matrix
	matTransform._11( AgentHeading.x );
	matTransform._12( AgentSide.x );
	matTransform._21( AgentHeading.y );
	matTransform._22( AgentSide.y );

	// now transform the vertices
	matTransform.TransformVector2s( TransPoint );

	return TransPoint;
}

//-------------------------- Vec2DRotateAroundOrigin --------------------------
//
//  rotates a vector ang rads around the origin
//-----------------------------------------------------------------------------
inline void Vec2DRotateAroundOrigin( Vector2& v, float ang )
{
	// create a transformation matrix
	C2DMatrix mat;

	// rotate
	mat.Rotate( ang );

	// now transform the object's vertices
	mat.TransformVector2s( v );
}

//------------------------ CreateWhiskers ------------------------------------
//
//  given an origin, a facing direction, a 'field of view' describing the
//  limit of the outer whiskers, a whisker length and the number of whiskers
//  this method returns a vector containing the end positions of a series
//  of whiskers radiating away from the origin and with equal distance between
//  them. (like the spokes of a wheel clipped to a specific segment size)
//----------------------------------------------------------------------------
inline std::vector< Vector2 > CreateWhiskers(
	unsigned int NumWhiskers, float WhiskerLength, float fov, Vector2 facing, Vector2 origin )
{
	// this is the magnitude of the angle separating each whisker
	float SectorSize = fov / ( float )( NumWhiskers - 1 );

	std::vector< Vector2 > whiskers;
	Vector2 temp;
	float angle = -fov * 0.5f;

	for( unsigned int w = 0; w < NumWhiskers; ++w )
	{
		// create the whisker extending outwards at this angle
		temp = facing;
		Vec2DRotateAroundOrigin( temp, angle );
		whiskers.push_back( origin + WhiskerLength * temp );

		angle += SectorSize;
	}

	return whiskers;
}

} // namespace SteeringBehaviors::Math
