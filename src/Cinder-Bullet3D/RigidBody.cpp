//
//  DefaultPhysicsObject.cpp
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/27/13.
//
//

#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/BulletContext.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

using namespace ci;

namespace bullet {
	
RigidBody::Format::Format()
: mCollShape( nullptr ), mMotionState( nullptr ), mMass( 0.0f ),
	mAddToWorld( false ), mFriction( btScalar(0.5) ),
	mRestitution( btScalar(0.0) ), mCollisionGroup( -1 ), mCollisionMask( -1 ),
	mInitialPosition( vec3( 0.0f, 1.0f, 0.0f ) ), mInitialScale( vec3( 1.0f ) ),
	mRigidBodyUserPtr( nullptr ), mSetKinematic( false )
{
}
	
RigidBody::RigidBody( const Format &format )
: mRigidBody( nullptr ), mCollisionShape( nullptr ),
	mMotionState( nullptr ), mType( PhyObjType::NO_SHAPE ),
	mAddedToWorld( false ), mCollGroup( -1 ), mCollMask( -1 ), mUpdatedScale( false )
{
	init( format );
}
	
RigidBodyRef RigidBody::create( const bullet::RigidBody::Format &format )
{
	return RigidBodyRef( new RigidBody( format ) );
}
	
RigidBody::~RigidBody()
{
	if( mAddedToWorld ) {
		Context()->removeRigidBody( mRigidBody.get() );
	}
	
	mRigidBody.reset();
	mMotionState.reset();
	mCollisionShape.reset();
}
	
void RigidBody::setCollisionGroup( int16_t collGroup )
{
	//TODO: Figure out how to change things.
}
	
void RigidBody::setCollisionMask( int16_t collMask )
{
	//TODO: Figure out how to change things.
}
	
void RigidBody::recalculateBoundingSphere()
{
	btVector3 center;
	mCollisionShape->getBoundingSphere( center, mBoundingSphereRadius );
	mBoundingSphereCenter = fromBullet( center );
}
	
void RigidBody::init( const Format &format )
{
	mType = getPhyObjType( format.mCollShape->getName() );
	
	mCollGroup = format.mCollisionGroup;
	mCollMask = format.mCollisionMask;
	
	mCollisionShape = format.mCollShape;
	mCollisionShape->setLocalScaling( toBullet(format.mInitialScale) );

	recalculateBoundingSphere();
	
	btVector3 localInertia(0,0,0);
	
	if( format.mMass != 0.0f && ! format.mSetKinematic ) {
		mCollisionShape->calculateLocalInertia( format.mMass, localInertia );
	}
	
	mMotionState = format.mMotionState;
	
	btRigidBody::btRigidBodyConstructionInfo cInfo( format.mMass, mMotionState.get(), mCollisionShape.get(), localInertia );
	
	cInfo.m_friction = format.mFriction;
	cInfo.m_restitution = format.mRestitution;
	
	mRigidBody.reset( new btRigidBody(cInfo) );
	
	if( format.mSetKinematic ) {
		mRigidBody->setCollisionFlags( mRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
		setActivationState( DISABLE_DEACTIVATION );
	}
	
	if( format.mAddToWorld ) {
		Context()->addRigidBody( mRigidBody.get(), mCollGroup, mCollMask );
		mAddedToWorld = true;
	}
	
	if( ! mMotionState ) {
		btTransform trans;
		trans.setOrigin( toBullet( format.mInitialPosition ) );
		trans.setRotation( toBullet( format.mInitialRotation ) );
		mRigidBody->setWorldTransform( trans );
	}
	
	mRigidBody->setUserPointer( format.mRigidBodyUserPtr ? format.mRigidBodyUserPtr : this );
}
	
}