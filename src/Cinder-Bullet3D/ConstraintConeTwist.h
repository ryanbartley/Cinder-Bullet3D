//
//  ConstraintConeTwist.h
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#include "Cinder-Bullet3D/ConstraintBase.h"

namespace bullet {
	

	
class ConstraintConeTwist : public ConstraintBase {
public:
	
	struct Format : ConstraintBase::Format {
		Format();
		
		Format& objA( const RigidBodyRef &objA ) { mObjA = objA; return *this; }
		Format& objB( const RigidBodyRef &objB ) { mObjB = objB; return *this; }
		Format& localAOrigin( const ci::Vec3f &origin ) { mLocalA.setOrigin( toBullet( origin ) ); return *this; }
		Format& localARot( const ci::Quatf &rot ) { mLocalA.setBasis( toBullet( rot.toMatrix33() ) ); return *this; }
		Format& localBOrigin( const ci::Vec3f &origin ) { mLocalB.setOrigin( toBullet( origin ) ); return *this; }
		Format& localBRot( const ci::Quatf &rot ) { mLocalB.setBasis( toBullet( rot.toMatrix33() ) ); return *this; }
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