//
//  CompoundShape.cpp
//  Prototype_3
//
//  Created by Ryan Bartley on 3/25/14.
//
//

#include "Cinder-Bullet3D/PhyObjCompoundShape.h"
#include "Cinder-Bullet3D/Common.h"

namespace bullet {
	
PhyObjCompoundShape::Format::Format()
{
}

PhyObjCompoundShape::PhyObjCompoundShape( const Format &format )
{
	mType = PhyObjType::COMPOUND_SHAPE;
	init( format );
}
	
void PhyObjCompoundShape::init( const Format &format )
{
	// If you haven't created a collision shape and you've given shapes and offsets
	// I'll make the compound shape otherwise I'll assume that you need to make one
	
	if ( ! format.mCollShape && format.mShapesAndOffsets.size() > 0 ) {
		btCompoundShapeRef pCompound( new btCompoundShape() );
		
		auto shapeIt = format.mShapesAndOffsets.begin();
		auto end = format.mShapesAndOffsets.end();
		while(  shapeIt != end ) {
			auto offsetIt = shapeIt->second.begin();
			auto end = shapeIt->second.end();
			while( offsetIt != end ) {
				pCompound->addChildShape( *offsetIt, shapeIt->first );
				++offsetIt;
			}
			++shapeIt;
		}
		
		const_cast<Format&>( format ).collisionShape( pCompound );
		
		RigidBody::init( format );
	}
	else {
		RigidBody::init( format );
	}
	
	mCollisionShape->setUserPointer( this );
}
	
}