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
	mConstraint = make_shared<btHingeConstraint>( *format.mObjA->getRigidBody().get(),
												 *format.mObjB->getRigidBody().get(),
												 format.mLocalA,
												 format.mLocalB,
												 format.mUseReferenceFrameA );
	mConstraint->setBreakingImpulseThreshold( 1000.0 );
}

ConstraintHingeRef ConstraintHinge::create( const Format &format )
{
	return ConstraintHingeRef( new ConstraintHinge( format ) );
}
	
}