//
//  PhyObjCharacterController.cpp
//  Prototype_3
//
//  Created by Ryan Bartley on 3/29/14.
//
//

#include "PhyObjCharacterController.h"
#include "Cinder-Bullet3D/BulletContext.h"

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace bullet {
	
PhyObjCharacterController::PhyObjCharacterController( const Format &format )
{
	init( format );
}
	
void PhyObjCharacterController::init( const Format &format )
{
	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	ghost->setCollisionShape( format.mCollisionShape );
	mCollisionObject = ghost;
	
	ghost->setFriction( format.mFriction );
	ghost->setRestitution( format.mRestitution );
	
	ghost->setUserPointer( this );
	
	ghost->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	
	btScalar stepHeight = format.mStepHeight == -1 ? getAabb().getSize().z / 1.5f : format.mStepHeight;
	
	mCharacter = new btKinematicCharacterController( ghost,
													static_cast<btConvexShape*>(ghost->getCollisionShape()),
													stepHeight );
	mCharacter->setJumpSpeed( format.mJumpSpeed );
	mCharacter->setFallSpeed( format.mFallSpeed );
	
	mCharacter->setUpAxis( 2 );
	
	auto world = Context::getCurrent()->world();
	
	world->addCollisionObject( ghost );
	world->addAction( this );
}
	
void PhyObjCharacterController::setVelocity( const ci::vec3 &v, float timeInterval )
{
	if( !mCharacter || !mCollisionObject )
		return;
	
	mCharacter->setVelocityForTimeInterval( toBullet( v ), timeInterval );
}
	
void PhyObjCharacterController::setLinearVelocity( float forward, float backward, float rightward, float leftward, float timeInterval )
{
	btTransform xform =  getGhostObject()->getWorldTransform();
	
	ci::vec3 walkDirection(0.f, 0.f, 0.f);
	
	if (leftward != 0)
		walkDirection -= ci::vec3(leftward, 0.f, 0.f);
	
	if (rightward != 0)
		walkDirection += ci::vec3(rightward, 0.f, 0.f);
	
	if (forward != 0)
		walkDirection += ci::vec3(0.f, forward, 0.f);
	
	if (backward != 0)
		walkDirection -= ci::vec3(0.f, backward, 0.f);
	
	ci::vec3 result = fromBullet(xform.getRotation()) * walkDirection;
	
	setVelocity(result, timeInterval);
}
	
void PhyObjCharacterController::setWorldTransform( const btTransform &worldTrans )
{
	//TODO: This
}
	
void PhyObjCharacterController::updateAction( btCollisionWorld *collisionWorld, float deltaTime )
{
	if( !mCharacter || !mCollisionObject )
		return;
	
	mCharacter->updateAction( collisionWorld, deltaTime );
	
	setWorldTransform( mCollisionObject->getWorldTransform() );
}
	
void PhyObjCharacterController::setRotation( const ci::quat &rotate )
{
	mCharacter->getGhostObject()->getWorldTransform().getBasis();
}
	
void PhyObjCharacterController::setGravity( float gravity )
{
	mCharacter->setGravity( gravity );
}
	
ci::AxisAlignedBox PhyObjCharacterController::getAabb()
{
	if ( mCollisionObject ) {
		btVector3 aabbMin;
		btVector3 aabbMax;
		
		mCollisionObject->getCollisionShape()->getAabb( mCollisionObject->getWorldTransform(), aabbMin, aabbMax );
		
		ci::vec3 min_aabb( aabbMin.x(), aabbMin.y(), aabbMin.z() );
		ci::vec3 max_aabb( aabbMax.x(), aabbMax.y(), aabbMax.z() );
		
		return ci::AxisAlignedBox( min_aabb, max_aabb );
	}
	
	return ci::AxisAlignedBox();
}
	
btPairCachingGhostObject* PhyObjCharacterController::getGhostObject()
{
	return static_cast<btPairCachingGhostObject*>(mCollisionObject);
}
	
btKinematicCharacterController* PhyObjCharacterController::getKinematicCharacterController()
{
	return mCharacter;
}
	

	
}