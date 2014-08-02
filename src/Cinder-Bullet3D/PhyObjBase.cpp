//
//  DefaultPhysicsObject.cpp
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/27/13.
//
//

#include "PhyObjBase.h"
#include "Common.h"
#include "BulletContext.h"
#include "btCollisionObject.h"

using namespace ci;

namespace bullet {
	
PhyObjBase::Format::Format()
: mCollShape( nullptr ), mMass( 0.0f ), mAddToWorld( true ),
	mFriction( btScalar(0.5) ), mRestitution( btScalar(0.0) ),
	mCollisionGroup( -1 ), mCollisionMask( -1 ),
	mInitialPosition( Vec3f::yAxis() ), mInitialScale( Vec3f::one() ),
	mInitialRotation( Quatf::identity() ), mCollisionShapeUserPtr( nullptr ),
	mMotionStateUserPtr( nullptr ), mRigidBodyUserPtr( nullptr ),
	mSetKinematic( false )
{
}
	
PhyObjBase::PhyObjBase()
: mRigidBody( nullptr ), mCollisionShape( nullptr ),
	mMotionState( nullptr ), mType( PhyObjType::NO_SHAPE ),
	mAddedToWorld( false ), mCollGroup( -1 ), mCollMask( -1 ), mUpdatedScale( false )
{
}
	
PhyObjBase::~PhyObjBase()
{
	if( mAddedToWorld ) {
		Context()->removeRigidBody( mRigidBody.get() );
	}
	
	mRigidBody.reset();
	mMotionState.reset();
	mCollisionShape.reset();
}
	
void PhyObjBase::setCollisionGroup( int16_t collGroup )
{
	//TODO: Figure out how to change things.
}
	
void PhyObjBase::setCollisionMask( int16_t collMask )
{
	//TODO: Figure out how to change things.
}
	
void PhyObjBase::recalculateBoundingSphere()
{
	btVector3 center;
	mCollisionShape->getBoundingSphere( center, mBoundingSphereRadius );
	mBoundingSphereCenter = fromBullet( center );
}
	
void PhyObjBase::init( const Format &format )
{
	mCollGroup = format.mCollisionGroup;
	mCollMask = format.mCollisionMask;
	
	mCollisionShape = format.mCollShape;
	mCollisionShape->setLocalScaling( toBullet(format.mInitialScale) );

	recalculateBoundingSphere();
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin( toBullet( format.mInitialPosition ) );
	transform.setRotation( toBullet( format.mInitialRotation ) );
	
	btVector3 localInertia(0,0,0);
	
	if( format.mMass != 0.0f && ! format.mSetKinematic ) {
		mCollisionShape->calculateLocalInertia( format.mMass, localInertia );
	}
	
	if( format.mSetKinematic ){
		mMotionState.reset( new SimpleGlKinematicMotionState( transform ) );
		mMotionState->setUserPointer( format.mMotionStateUserPtr );
	}
	else {
		mMotionState.reset( new SimpleGlDynamicMotionState( transform ) );
		mMotionState->setUserPointer( format.mMotionStateUserPtr );
	}
	
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
	
	
	mRigidBody->setUserPointer( format.mRigidBodyUserPtr ? format.mRigidBodyUserPtr : this );
	mCollisionShape->setUserPointer( format.mCollisionShapeUserPtr ? format.mCollisionShapeUserPtr : this );
	
}
	
}