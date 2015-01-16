//
//  ConstraintHinge.h
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

	
class ConstraintHinge : public ConstraintBase {
public:
	
	struct Format : ConstraintBase::Format {
		Format();
		
		Format& objA( const RigidBodyRef &objA ) { mObjA = objA; return *this; }
		Format& objB( const RigidBodyRef &objB ) { mObjB = objB; return *this; }
		Format& localAOrigin( const ci::vec3 &origin ) { mLocalA.setOrigin( toBullet( origin ) ); return *this; }
		Format& localARot( float z, float y, float x ) { mLocalA.getBasis().setEulerZYX( z, y, x ); return *this; }
		Format& localBOrigin( const ci::vec3 &origin ) { mLocalB.setOrigin( toBullet( origin ) ); return *this; }
		Format& localBRot( float z, float y, float x ) { mLocalB.getBasis().setEulerZYX( z, y, x ); return *this; }
		Format& localAtrans( const btTransform &trans ) { mLocalA = trans; return *this; }
		Format& localBtrans( const btTransform &trans ) { mLocalB = trans; return *this; }
		Format& useReferenceFrameA( bool use ) { mUseReferenceFrameA = use; return *this; }
		
	protected:
		btTransform mLocalA, mLocalB;
		bool mUseReferenceFrameA;
		
		friend class ConstraintHinge;
	};
	
	static ConstraintHingeRef create( const Format &format );
	
	btHingeConstraintRef getHingeConstraint()
	{
		return std::static_pointer_cast<btHingeConstraint>(mConstraint);
	}
	
	inline void setLimit( btScalar low, btScalar high )
	{
		getHingeConstraint()->setLimit( low, high );
	}
	
	~ConstraintHinge() {}
	
private:
	ConstraintHinge( const Format &format );
};
	
}
