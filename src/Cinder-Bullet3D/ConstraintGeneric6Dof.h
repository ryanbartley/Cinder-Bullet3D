//
//  ConstraintGeneric6Dof.h
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/ConstraintBase.h"

namespace bullet {
	
	///////////////////////////////////////////////////////////////////////
	// This is not useable at this point and needs more tuning and testing,
	// use at your own risk
	///////////////////////////////////////////////////////////////////////
	
class ConstraintGeneric6Dof : public ConstraintBase {
public:
	
	struct Format : ConstraintBase::Format {
		Format();
		
		void setFrameInA( const btTransform &frameInA ) { mFrameInA = frameInA; }
		void setFrameInB( const btTransform &frameInB ) { mFrameInB = frameInB; }
		
		Format& objA( const RigidBodyRef &objA ) { mObjA = objA; return *this; }
		Format& objB( const RigidBodyRef &objB ) { mObjB = objB; return *this; }
		Format& frameInA( const btTransform &frameInA ) { mFrameInA = frameInA; return *this; }
		Format& frameInB( const btTransform &frameInB ) { mFrameInB = frameInB; return *this; }
		Format& useLinearReferenceFrame( bool useLinearReferenceFrame ) { mUseLinearReferenceFrame = useLinearReferenceFrame; return *this;}
		Format& addToWorld( bool addToWorld ) { mAddToWorld = addToWorld; return *this; }
	
	protected:
		btTransform		mFrameInA, mFrameInB;
		bool			mUseLinearReferenceFrame, mAddToWorld;
		
		friend class ConstraintGeneric6Dof;
	};
	
	~ConstraintGeneric6Dof();
	
	static ConstraintGeneric6DofRef create( const Format &format );
	
	btGeneric6DofConstraintRef getConstraintGeneric6Dof() { return std::static_pointer_cast<btGeneric6DofConstraint>(mConstraint); }
	const btGeneric6DofConstraintRef getConstraintGeneric6Dof() const { return std::static_pointer_cast<btGeneric6DofConstraint>(mConstraint); }
	
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setLinearLowerLimit( const ci::vec3 &linearLower ) { getConstraintGeneric6Dof()->setLinearLowerLimit( toBullet( linearLower ) ); }
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setLinearUpperLimit( const ci::vec3 &linearUpper ) { getConstraintGeneric6Dof()->setLinearUpperLimit( toBullet( linearUpper ) ); }
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setAngularLowerLimit( const ci::vec3 &angularLower ) { getConstraintGeneric6Dof()->setAngularLowerLimit( toBullet( angularLower ) ); }
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setAngularUpperLimit( const ci::vec3 &angularUpper ) { getConstraintGeneric6Dof()->setAngularUpperLimit( toBullet( angularUpper ) ); }
	
private:
	ConstraintGeneric6Dof( const Format &format );
};
	
}
