//
//  PhyObjCharacterController.h
//  Prototype_3
//
//  Created by Ryan Bartley on 3/29/14.
//
//

#include "BulletDynamics/Dynamics/btActionInterface.h"

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/RigidBody.h"

#include "cinder/AxisAlignedBox.h"

class btDynamicsWorld;
class btPairCachingGhostObject;
class btKinematicCharacterController;
class btTriangleMesh;

///////////////////////////////////////////////////////////////////////
// This is not useable at this point and needs more tuning and testing,
// use at your own risk
///////////////////////////////////////////////////////////////////////

namespace bullet {
	
class PhyObjCharacterController : public btActionInterface {
public:
	
	struct Format {
		
		Format& collisionShape( btCollisionShape *shape ) { mCollisionShape = shape; return *this; }
		Format& jumpSpeed( btScalar jumpSpeed ) { mJumpSpeed = jumpSpeed; return *this; }
		Format& fallSpeed( btScalar fallSpeed ) { mFallSpeed = fallSpeed; return *this; }
		Format& stepHeight( btScalar stepHeight ) { mStepHeight = stepHeight; return *this; }
		Format& collGroup( int16_t collGroup ) { mCollisionGroup = collGroup; return *this; }
		Format& collMask( int16_t collMask ) { mCollisionMask = collMask; return *this; }
		
	private:
		btCollisionShape*   mCollisionShape;
		btScalar			mJumpSpeed, mFallSpeed, mStepHeight,
							mFriction, mRestitution;
		int16_t				mCollisionGroup, mCollisionMask;
		
		friend class PhyObjCharacterController;
	};
	
	void setVelocity( const ci::vec3 &v, float timeInterval );
	void setLinearVelocity( float forward, float backward, float rightward, float leftward, float timeInterval );
	
	void debugDraw( btIDebugDraw* debugDrawer ) {}
	
	void setRotation( const ci::quat &rotate );
	
	void jump();
	
	bool isOnGround();
	
	void setGravity( float gravity );
	void setJumpSpeed( float jumpSpeed );
	void setFallSpeed( float fallSpeed );
	
	void updateAction( btCollisionWorld* collisionWorld, float deltaTime );
	
	ci::AxisAlignedBox getAabb();
	
	btPairCachingGhostObject* getGhostObject();
	btKinematicCharacterController* getKinematicCharacterController();
	
private:
	PhyObjCharacterController( const Format &format );
	
	void init( const Format &format );
	
	void setWorldTransform( const btTransform &worldTrans );
	
	btKinematicCharacterController  *mCharacter;
	btCollisionObject				*mCollisionObject;
};
	
}
