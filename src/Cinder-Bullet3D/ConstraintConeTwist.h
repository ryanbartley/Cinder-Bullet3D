//
//  ConstraintConeTwist.h
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#include "Cinder-Bullet3D/ConstraintBase.h"

namespace bullet {
	
	///////////////////////////////////////////////////////////////////////
	// This is not useable at this point and needs more tuning and testing,
	// use at your own risk
	///////////////////////////////////////////////////////////////////////
	
class ConstraintConeTwist : public ConstraintBase {
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
		
	protected:
		btTransform mLocalA, mLocalB;
		
		friend class ConstraintConeTwist;
	};
	
	static ConstraintConeTwistRef create( const Format &format );
	
	inline btConeTwistConstraintRef getConeTwist()
	{
		return std::static_pointer_cast<btConeTwistConstraint>(mConstraint);
	}
	
	inline void setLimit( int limitIndex, btScalar limitValue )
	{
		getConeTwist()->setLimit( limitIndex, limitValue );
	}
	
	inline void setLimit( btScalar swingSpan1, btScalar swingSpan2, btScalar swingSpan3 )
	{
		getConeTwist()->setLimit( swingSpan1, swingSpan2, swingSpan3 );
	}
	
	~ConstraintConeTwist() {}
	
private:
	ConstraintConeTwist( const Format &format );
};
	
}