//
//  ConvexHullPhyObj.cpp
//  Prototype_3
//
//  Created by Ryan Bartley on 3/26/14.
//
//

#include "Physics/PhyObjConvexHull.h"
#include "Physics/Common.h"

namespace bullet {
	
PhyObjConvexHull::Format::Format()
: mMesh( nullptr )
{
	
}
	
PhyObjConvexHull::PhyObjConvexHull( const Format &format )
{
	mType = PhyObjType::CONVEX_HULL;
	init( format );
}

PhyObjConvexHullRef PhyObjConvexHull::create( const Format &format )
{
	return PhyObjConvexHullRef( new PhyObjConvexHull( format ) );
}
	
PhyObjConvexHull::~PhyObjConvexHull()
{
}
	
void PhyObjConvexHull::init( const Format &format )
{
	// If you haven't created a collision shape and you've given a trimesh
	// I'll make the convex hull otherwise I'll assume that you need to make one
	
	if ( ! format.mCollShape && format.mMesh ) {
		auto mMesh = format.mMesh->getVertices<3>();
		std::vector<btVector3> mBulletMesh( format.mMesh->getNumVertices() );
		
		auto bulletIt = mBulletMesh.begin();
		auto end = mBulletMesh.end();
		while( bulletIt != end ) {
			*bulletIt = toBullet( *mMesh++ );
			++bulletIt;
		}
		
		btConvexHullShapeRef convexShape( new btConvexHullShape( &mBulletMesh.data()->getX(), mBulletMesh.size() ) );
		convexShape->initializePolyhedralFeatures();
		
		auto changedFormat = const_cast<Format&>(format);
		changedFormat.collisionShape( convexShape );
		
		PhyObjBase::init( changedFormat );
	}
	else {
		PhyObjBase::init( format );
	}
}
	
}