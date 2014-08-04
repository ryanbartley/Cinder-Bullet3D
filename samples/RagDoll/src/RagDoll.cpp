//
//  RagDoll.c
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#include "RagDoll.h"
#include "Cinder-Bullet3D/ConstraintHinge.h"
#include "Cinder-Bullet3D/ConstraintConeTwist.h"

#include "cinder/gl/GlslProg.h"

#define CONSTRAINT_DEBUG_SIZE 0.2f

using namespace ci::app;
using namespace bullet;
using namespace std;
using namespace ci;

RagDoll::RagDoll ( const bullet::ContextRef &context, const ci::Vec3f &positionOffset, const ci::gl::GlslProgRef &glsl  )
: mOwner( context )
{
	
	// Setup the geometry
	mShapes.push_back( createCapsuleShape( 0.15, 0.20 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.15 ).length( 0.20 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.15, 0.28 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.15 ).length( 0.28 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.10, 0.05 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.10 ).length( 0.05 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.07, 0.45 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.07 ).length( 0.45 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05, 0.37 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05 ).length( 0.37 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.07, 0.45 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.07 ).length( 0.45 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05, 0.37 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05 ).length( 0.37 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05, 0.33 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05 ).length( 0.33 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.04, 0.25 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.04 ).length( 0.25 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05, 0.33 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05 ).length( 0.33 ).enable( geom::Attrib::NORMAL ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.04, 0.25 ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.04 ).length( 0.25 ).enable( geom::Attrib::NORMAL ), glsl ) );
	
	btTransform offset; offset.setIdentity();
	offset.setOrigin( toBullet( positionOffset ) );
	
	bullet::RigidBody::Format format;
	format.addToWorld( true );
	
	// PELVIS
	format.initialPosition( Vec3f( 0, 1, 0 ) ).mass( 1.0 ).collisionShape( mShapes[BODYPART_PELVIS] );
	auto body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// SPINE
	format.initialPosition( Vec3f( 0, 1.2, 0 ) ).collisionShape( mShapes[BODYPART_SPINE] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// HEAD
	format.initialPosition( Vec3f( 0, 1.6, 0 ) ).collisionShape( mShapes[BODYPART_HEAD] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT UPPER LEG
	format.initialPosition( Vec3f( -0.18, 0.65, 0.0 ) ).collisionShape( mShapes[BODYPART_LEFT_UPPER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT LOWER LEG
	format.initialPosition( Vec3f( -0.18, 0.2, 0. ) ).collisionShape( mShapes[BODYPART_LEFT_LOWER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// RIGHT UPPER LEG
	format.initialPosition( Vec3f( 0.18, 0.65, 0. ) ).collisionShape( mShapes[BODYPART_RIGHT_UPPER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// RIGHT LOWER LEG
	format.initialPosition( Vec3f( 0.18, 0.2, 0. ) ).collisionShape( mShapes[BODYPART_RIGHT_LOWER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT UPPER ARM
	format.initialPosition( Vec3f( -0.35, 1.45, 0.0 ) ).initialRotation( Quatf( 0, 0, M_PI_2 ) ).collisionShape( mShapes[BODYPART_LEFT_UPPER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT LOWER ARM
	format.initialPosition( Vec3f( -0.7, 1.45, 0.0 ) ).initialRotation( Quatf( 0, 0, M_PI_2 ) ).collisionShape( mShapes[BODYPART_LEFT_LOWER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	format.initialPosition( Vec3f( 0.35, 1.45, 0.0 ) ).initialRotation( Quatf( 0, 0, -M_PI_2 ) ).collisionShape( mShapes[BODYPART_RIGHT_UPPER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// RIGHT LOWER ARM
	format.initialPosition( Vec3f( 0.7, 1.45, 0.0 ) ).initialRotation( Quatf( 0, 0, -M_PI_2 ) ).collisionShape( mShapes[BODYPART_RIGHT_LOWER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	{
		// Setup some damping on the mBodies
		auto bodyIt = mBodies.begin();
		auto end = mBodies.end();
		for ( ; bodyIt != end; ++bodyIt )
		{
			cout << "adding damping" << endl;
			(*bodyIt)->setDamping(0.5, 0.85);
			(*bodyIt)->setDeactivationTime(0.8);
			(*bodyIt)->setSleepingThresholds(4, 4);
		}
	}
	
	ConstraintHinge::Format hingeFormat;
	ConstraintConeTwist::Format coneFormat;
	
	hingeFormat.localAOrigin( Vec3f( 0.0, 0.15, 0.0 ) ).localARot( 0,M_PI_2,0 ).objA( mBodies[BODYPART_PELVIS] )
				.localBOrigin( Vec3f( 0.0, -0.15, 0.0 ) ).localBRot( 0,M_PI_2,0 ).objB( mBodies[BODYPART_SPINE] );
	{
		auto hinge = ConstraintHinge::create( hingeFormat );
		hinge->setLimit( btScalar(-M_PI_4), btScalar(M_PI_2) );
		hinge->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( hinge );
	}
	
	coneFormat.localAOrigin( Vec3f( 0.0, 0.30, 0.0 ) ).localARot( 0,0,M_PI_2 ).objA( mBodies[BODYPART_SPINE] )
			  .localBOrigin( Vec3f( 0.0, -0.14, 0.0 ) ).localBRot( 0,0,M_PI_2 ).objB( mBodies[BODYPART_HEAD] );
	{
		auto cone = ConstraintConeTwist::create( coneFormat );
		cone->setLimit( M_PI_4, M_PI_4, M_PI_2 );
		cone->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	// Something's going on with quaternions here
	coneFormat.localAOrigin( Vec3f( -0.18, -0.10, 0.0 ) ).localARot( 0, 0, -M_PI_4*5 ).objA( mBodies[BODYPART_PELVIS] )
			  .localBOrigin( Vec3f( 0.0, 0.225, 0.0 ) ).localARot( 0, 0, -M_PI_4*5 ).objB( mBodies[BODYPART_LEFT_UPPER_LEG] );
	{
		auto cone = ConstraintConeTwist::create( coneFormat );
		cone->setLimit( M_PI_4, M_PI_4, 0 );
		cone->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}
	

	hingeFormat.localAOrigin( Vec3f( 0.0, -0.225, 0.0 ) ).localARot( 0,M_PI_2,0 ).objA( mBodies[BODYPART_LEFT_UPPER_LEG] )
				.localBOrigin( Vec3f( 0.0, 0.185, 0.0 ) ).localBRot( 0,M_PI_2,0 ).objB( mBodies[BODYPART_LEFT_LOWER_LEG] );
	{
		auto hinge = ConstraintHinge::create( hingeFormat );
		hinge->setLimit( btScalar(0), btScalar(M_PI_2) );
		hinge->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( hinge );
	}

	coneFormat.localAOrigin( Vec3f( 0.18, -0.10, 0.0 ) ).localARot( 0, 0, M_PI_4 ).objA( mBodies[BODYPART_PELVIS] )
			  .localBOrigin( Vec3f( 0.0, 0.225, 0.0 ) ).localBRot( 0, 0, M_PI_4 ).objB( mBodies[BODYPART_RIGHT_UPPER_LEG] );
	{
		auto cone = ConstraintConeTwist::create( coneFormat );
		cone->setLimit( M_PI_4, M_PI_4, 0 );
		cone->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	hingeFormat.localAOrigin( Vec3f( 0.0, -0.225, 0.0 ) ).localARot( 0,M_PI_2,0 ).objA( mBodies[BODYPART_RIGHT_UPPER_LEG] )
			   .localBOrigin( Vec3f( 0.0, 0.185, 0.0 ) ).localBRot( 0,M_PI_2,0 ).objB( mBodies[BODYPART_RIGHT_LOWER_LEG] );
	{
		auto hinge = ConstraintHinge::create( hingeFormat );
		hinge->setLimit( btScalar(0), btScalar(M_PI_2) );
		hinge->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( hinge );
	}

	coneFormat.localAOrigin( Vec3f( -0.2, 0.15, 0.0 ) ).localARot( 0,0,M_PI ).objA( mBodies[BODYPART_SPINE] )
			  .localBOrigin( Vec3f( 0.0, -0.18, 0.0 ) ).localBRot( 0,0,M_PI_2 ).objB( mBodies[BODYPART_LEFT_UPPER_ARM] );
	{
		auto cone = ConstraintConeTwist::create( coneFormat );
		cone->setLimit( M_PI_2, M_PI_2, 0 );
		cone->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	hingeFormat.localAOrigin( Vec3f( 0.0, 0.18, 0.0 ) ).localARot( 0,M_PI_2,0 ).objA( mBodies[BODYPART_LEFT_UPPER_ARM] )
			   .localBOrigin( Vec3f( 0.0, -0.14, 0.0 ) ).localBRot( 0,M_PI_2,0 ).objB( mBodies[BODYPART_LEFT_LOWER_ARM] );
	{
		auto hinge = ConstraintHinge::create( hingeFormat );
		hinge->setLimit( btScalar(0), btScalar(M_PI_2) );
		hinge->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( hinge );
	}

	coneFormat.localAOrigin( Vec3f( 0.2, 0.15, 0.0 ) ).localARot( 0,0,0 ).objA( mBodies[BODYPART_SPINE] )
			  .localBOrigin( Vec3f( 0., -0.18, 0.0 ) ).localBRot( 0,0,M_PI_2 ).objB( mBodies[BODYPART_RIGHT_UPPER_ARM] );
	{
		auto cone = ConstraintConeTwist::create( coneFormat );
		cone->setLimit( M_PI_2, M_PI_2, 0 );
		cone->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	hingeFormat.localAOrigin( Vec3f( 0.0, 0.18, 0.0 ) ).localARot( 0,M_PI_2,0 ).objA( mBodies[BODYPART_RIGHT_UPPER_ARM] )
			   .localBOrigin( Vec3f( 0.0, -0.14, 0.0 ) ).localBRot( 0,M_PI_2,0 ).objB( mBodies[BODYPART_RIGHT_LOWER_ARM] );
	{
		auto hinge = ConstraintHinge::create( hingeFormat );
		hinge->setLimit( btScalar(0), btScalar(M_PI_2) );
		hinge->setDebugDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( hinge );
	}
	
	{
		auto jointIt = mConstraints.begin();
		auto end = mConstraints.end();
		for( ; jointIt != end; ++jointIt ) {
			mOwner->addConstraint( (*jointIt)->getTypedConstraint(), true );
		}
	}
}

RagDollRef RagDoll::create( const bullet::ContextRef &context, const ci::Vec3f &positionOffset, const ci::gl::GlslProgRef &glsl  )
{
	return RagDollRef( new RagDoll( context, positionOffset, glsl ) );
}
	
RagDoll::~RagDoll ()
{
	int i;
	
	// Remove all constraints
	for ( i = 0; i < JOINT_COUNT; ++i)
	{
		mOwner->removeConstraint(mConstraints[i]);
		mConstraints[i].reset();
	}
	
	// Remove all bodies and shapes
	for ( i = 0; i < BODYPART_COUNT; ++i)
	{
		mOwner->removeRigidBody(mBodies[i]);
		
		mBodies[i].reset();
		mShapes[i].reset();
	}
}
