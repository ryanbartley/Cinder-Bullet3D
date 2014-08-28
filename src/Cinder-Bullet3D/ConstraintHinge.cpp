//
//  ConstraintHinge.cpp
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#include "Cinder-Bullet3D/ConstraintHinge.h"

using namespace std;
using namespace ci;

namespace bullet {
	
ConstraintHinge::Format::Format()
: mUseReferenceFrameA( false )
{
	mLocalA.setIdentity();
	mLocalB.setIdentity();
}
	
ConstraintHinge::ConstraintHinge( const Format &format )
{
	if( format.mObjB ) {
		mConstraint = make_shared<btHingeConstraint>( *format.mObjA->getRigidBody().get(),
												 *format.mObjB->getRigidBody().get(),
												 format.mLocalA,
												 format.mLocalB,
												 format.mUseReferenceFrameA );
	}
	else {
		mConstraint = make_shared<btHingeConstraint>( *format.mObjA->getRigidBody().get(),
													 format.mLocalA,
													 format.mUseReferenceFrameA );
	}
}

ConstraintHingeRef ConstraintHinge::create( const Format &format )
{
	return ConstraintHingeRef( new ConstraintHinge( format ) );
}
	
}