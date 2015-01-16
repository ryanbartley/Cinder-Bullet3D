//
//  ConstraintBase.h
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/RigidBody.h"

namespace bullet {
	
	///////////////////////////////////////////////////////////////////////
	// This is not useable at this point and needs more tuning and testing,
	// use at your own risk
	///////////////////////////////////////////////////////////////////////
	
class ConstraintBase {
public:
	
	struct Format {
		Format();
		
		void setObjA( const RigidBodyRef &objA ) { mObjA = objA; }
		void setObjB( const RigidBodyRef &objB ) { mObjB = objB; }
		
	protected:
		RigidBodyRef	mObjA, mObjB;
		
	};
	
	//! Returns the Typed Constraint Type.
	btTypedConstraintType getConstraintType() { return mConstraint->getConstraintType(); }
	
	//! The default global value of a parameter (such as ERP or CFM), optionally provide the axis (0..5). If no axis is provided, it uses the default axis for this constraint.
	void setParam(int num, btScalar value, int axis = -1) { mConstraint->setParam( num, value, axis ); }
	//! Returns the local value of parameter.
	btScalar getParam(int num, int axis = -1) const { return mConstraint->getParam( num, axis ); }
	
	//! Returns a pointer to the typed constraint.
	btTypedConstraintRef& getTypedConstraint() { return mConstraint; }
	//! Returns a const pointer to the typed constraint.
	const btTypedConstraintRef& getTypedConstraint() const { return mConstraint; }
	
	//! Sets the Breaking Threshold of this Constraint.
	void setBreakingImpulseThreshold( btScalar threshold ) { mConstraint->setBreakingImpulseThreshold( threshold ); }
	//! Returns the Breaking Threshold of this Constraint.
	btScalar getBreakingImpulseThreshold() { return mConstraint->getBreakingImpulseThreshold(); }
	
	//! Returns a btRigidBodyRef of Rigid Body A stored by the constraint.
	btRigidBodyRef getRigidBodyA() { return btRigidBodyRef( &mConstraint->getRigidBodyA() ); }
	//! Returns a btRigidBodyRef of Rigid Body B stored by the constraint.
	btRigidBodyRef getRigidBodyB() { return btRigidBodyRef( &mConstraint->getRigidBodyB() ); }
	
	//! Sets the size to draw the constraint, scales in all directions.
	void setDebugDrawSize( btScalar size ) { mConstraint->setDbgDrawSize( size ); }
	//! Returns the current set debug draw size.
	btScalar getDebugDrawSize() { return mConstraint->getDbgDrawSize(); }
	
protected:
	ConstraintBase();
	
	btTypedConstraintRef mConstraint;
};
	
}
