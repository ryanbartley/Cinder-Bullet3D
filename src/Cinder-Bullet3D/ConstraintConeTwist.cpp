//
//  ConstraintConeTwist.cpp
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#include "ConstraintConeTwist.h"

namespace bullet {
	
	ConstraintConeTwist::Format::Format()
	{
		mLocalA.setIdentity();
		mLocalB.setIdentity();
	}
	
ConstraintConeTwist::ConstraintConeTwist( const Format &format )
{
	mConstraint = std::make_shared<btConeTwistConstraint>( *format.mObjA->getRigidBody().get(), *format.mObjB->getRigidBody().get(), format.mLocalA, format.mLocalB );
}
	
ConstraintConeTwistRef ConstraintConeTwist::create( const Format &format )
{
	return ConstraintConeTwistRef( new ConstraintConeTwist( format ) );
}
	
	
}