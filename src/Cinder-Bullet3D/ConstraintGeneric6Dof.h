//
//  ConstraintGeneric6Dof.h
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#pragma once

#include "Physics/Common.h"
#include "Physics/PhyObjBase.h"
#include "Physics/ConstraintBase.h"

namespace bullet {
	
typedef std::shared_ptr<class ConstraintGeneric6Dof> ConstraintGeneric6DofRef;
	
class ConstraintGeneric6Dof : public ConstraintBase {
public:
	
	struct Format : ConstraintBase::Format {
		Format();
		
		void setFrameInA( const btTransform &frameInA ) { mFrameInA = frameInA; }
		void setFrameInB( const btTransform &frameInB ) { mFrameInB = frameInB; }
		
		Format& objA( const PhyObjBaseRef &objA ) { mObjA = objA; return *this; }
		Format& objB( const PhyObjBaseRef &objB ) { mObjB = objB; return *this; }
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
	
	btGeneric6DofConstraint* getConstraintGeneric6Dof() { return static_cast<btGeneric6DofConstraint*>(mConstraint); }
	const btGeneric6DofConstraint* getConstraintGeneric6Dof() const { return static_cast<btGeneric6DofConstraint*>(mConstraint); }
	
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setLinearLowerLimit( const ci::Vec3f &linearLower ) { getConstraintGeneric6Dof()->setLinearLowerLimit( toBullet( linearLower ) ); }
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setLinearUpperLimit( const ci::Vec3f &linearUpper ) { getConstraintGeneric6Dof()->setLinearUpperLimit( toBullet( linearUpper ) ); }
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setAngularLowerLimit( const ci::Vec3f &angularLower ) { getConstraintGeneric6Dof()->setAngularLowerLimit( toBullet( angularLower ) ); }
	//! From the Bullet Wiki, For each axis, if lower limit = upper limit, The axis is locked. If lower limit < upper limit, the axis is limited between the specified values. If lower limit > upper limit, The axis is free and has no limits.
	void setAngularUpperLimit( const ci::Vec3f &angularUpper ) { getConstraintGeneric6Dof()->setAngularUpperLimit( toBullet( angularUpper ) ); }
	
private:
	ConstraintGeneric6Dof( const Format &format );
};
	
}
