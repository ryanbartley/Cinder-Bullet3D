//
//  ConstraintGeneric6Dof.cpp
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#include "Cinder-Bullet3D/ConstraintGeneric6Dof.h"
#include "Cinder-Bullet3D/BulletContext.h"

namespace bullet {
	
ConstraintGeneric6Dof::Format::Format()
: mUseLinearReferenceFrame( false ), mAddToWorld( true )
{
	
}
	
ConstraintGeneric6Dof::ConstraintGeneric6Dof( const Format &format )
{
	if( format.mObjB ) {
		mConstraint =  std::make_shared<btGeneric6DofConstraint>( *format.mObjA->getRigidBody().get(), *format.mObjB->getRigidBody().get(), format.mFrameInA, format.mFrameInB, format.mUseLinearReferenceFrame );
	}
	else {
		mConstraint = std::make_shared<btGeneric6DofConstraint>( *format.mObjA->getRigidBody().get(), format.mFrameInA, format.mUseLinearReferenceFrame );
	}
	
	mConstraint->setUserConstraintPtr( this );
	
	if( format.mAddToWorld ) {
		Context()->addConstraint( mConstraint );
	}
	
}
	
ConstraintGeneric6Dof::~ConstraintGeneric6Dof()
{
	mConstraint.reset();
}
	
ConstraintGeneric6DofRef ConstraintGeneric6Dof::create( const Format &format )
{
	return ConstraintGeneric6DofRef( new ConstraintGeneric6Dof( format ) );
}
	
}