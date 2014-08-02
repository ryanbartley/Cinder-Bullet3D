//
//  ConstraintGeneric6Dof.cpp
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#include "ConstraintGeneric6Dof.h"
#include "BulletContext.h"

namespace bullet {
	
ConstraintGeneric6Dof::Format::Format()
: mUseLinearReferenceFrame( false ), mAddToWorld( true )
{
	
}
	
ConstraintGeneric6Dof::ConstraintGeneric6Dof( const Format &format )
{
	if( format.mObjB ) {
		mConstraint = new btGeneric6DofConstraint( *format.mObjA->getRigidBody(), *format.mObjB->getRigidBody(), format.mFrameInA, format.mFrameInB, format.mUseLinearReferenceFrame );
	}
	else {
		mConstraint = new btGeneric6DofConstraint( *format.mObjA->getRigidBody(), format.mFrameInA, format.mUseLinearReferenceFrame );
	}
	
	mConstraint->setUserConstraintPtr( this );
	
	if( format.mAddToWorld ) {
		Context()->world()->addConstraint( mConstraint );
	}
	
}
	
ConstraintGeneric6Dof::~ConstraintGeneric6Dof()
{
	delete mConstraint;
}
	
ConstraintGeneric6DofRef ConstraintGeneric6Dof::create( const Format &format )
{
	return ConstraintGeneric6DofRef( new ConstraintGeneric6Dof( format ) );
}
	
}