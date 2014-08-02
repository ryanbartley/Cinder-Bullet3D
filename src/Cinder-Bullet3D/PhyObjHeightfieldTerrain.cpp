//
//  PhyObjRigidTerrain.cpp
//  Prototype_3
//
//  Created by Ryan Bartley on 3/29/14.
//
//

#include "PhyObjHeightfieldTerrain.h"
#include "Common.h"

#include "btHeightfieldTerrainShape.h"

using namespace ci;
using namespace std;

namespace bullet {
	
PhyObjHeightfieldTerrain::Format::Format()
: mHeightData( nullptr ), mMinHeight( -1.0f ),
	mMaxHeight( 1.0f ), mScale( Vec3f( 1, 1, 1 ) )
{
}
	
PhyObjHeightfieldTerrain::PhyObjHeightfieldTerrain( const Format &format )
{
	mType = PhyObjType::HEIGHTFIELD;
	init( format );
}
	
PhyObjHeightfieldTerrainRef PhyObjHeightfieldTerrain::create( const Format &format )
{
	return PhyObjHeightfieldTerrainRef( new PhyObjHeightfieldTerrain( format ) );
}
	
void PhyObjHeightfieldTerrain::init( const Format &format )
{
	// If you haven't created a collision shape and you've given a trimesh
	// I'll make the convex hull otherwise I'll assume that you need to make one
	
	if ( ! format.mCollShape && format.mHeightData ) {
		int32_t length	= format.mHeightData->getHeight();
		int32_t width	= format.mHeightData->getWidth();
		
		float heightScale = math<float>::abs( format.mMinHeight ) + math<float>::abs( format.mMaxHeight );
		btHeightfieldTerrainShapeRef shape( new btHeightfieldTerrainShape( width, length, format.mHeightData->getData(), heightScale, format.mMinHeight, format.mMaxHeight, 1, PHY_FLOAT, false ) );
		shape->setLocalScaling( toBullet( format.mScale ) );
		
		auto changedFormat = const_cast<Format&>( format );
		changedFormat.collisionShape( shape );
		
		PhyObjBase::init( changedFormat );
	}
	else {
		PhyObjBase::init( format );
	}
}
	
btHeightfieldTerrainShapeRef PhyObjHeightfieldTerrain::getHeightfieldShape()
{
	return std::dynamic_pointer_cast<btHeightfieldTerrainShape>(mCollisionShape);
}
const btHeightfieldTerrainShapeRef PhyObjHeightfieldTerrain::getHeightfieldShape() const
{
	return std::dynamic_pointer_cast<btHeightfieldTerrainShape>(mCollisionShape);
}
	
}