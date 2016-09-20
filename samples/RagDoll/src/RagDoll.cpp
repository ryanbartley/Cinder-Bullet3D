//
//  RagDoll.c
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#define _USE_MATH_DEFINES
#include <cmath>

#include "RagDoll.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/app/App.h"

#define CONSTRAINT_DEBUG_SIZE 0.2f

using namespace ci::app;
using namespace bullet;
using namespace std;
using namespace ci;

RagDoll::RagDoll ( const bullet::ContextRef &context, const ci::vec3 &positionOffset, const ci::gl::GlslProgRef &glsl  )
: mOwner( context )
{
	// Setup the geometry
	mShapes.push_back( createCapsuleShape( 0.15f, 0.20f ) );
	
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.15f ).length( 0.20f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.15f, 0.28f ) );
	
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.15f ).length( 0.28f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.10f, 0.05f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.10f ).length( 0.05f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.07f, 0.45f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.07f ).length( 0.45f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05f, 0.37f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05f ).length( 0.37f ), glsl ) );
	
	mShapes.push_back( createCapsuleShape( 0.07f, 0.45f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.07f ).length( 0.45f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05f, 0.37f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05f ).length( 0.37f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05f, 0.33f ) );
	
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05f ).length( 0.33f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.04f, 0.25f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.04f ).length( 0.25f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.05f, 0.33f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.05f ).length( 0.33f ), glsl ) );
	mShapes.push_back( createCapsuleShape( 0.04f, 0.25f ) );
	mBatches.push_back( gl::Batch::create( geom::Capsule().radius( 0.04f ).length( 0.25f ), glsl ) );
	
	btTransform offset; offset.setIdentity();
	offset.setOrigin( toBullet( positionOffset ) );
	
	bullet::RigidBody::Format format;
	format.addToWorld( true );
	
	// PELVIS
	format.initialPosition( vec3( 0, 1, 0 ) ).mass( 1.0 ).collisionShape( mShapes[BODYPART_PELVIS] );
	auto body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// SPINE
	format.initialPosition( vec3( 0, 1.2, 0 ) ).collisionShape( mShapes[BODYPART_SPINE] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// HEAD
	format.initialPosition( vec3( 0, 1.6, 0 ) ).collisionShape( mShapes[BODYPART_HEAD] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT UPPER LEG
	format.initialPosition( vec3( -0.18, 0.65, 0.0 ) ).collisionShape( mShapes[BODYPART_LEFT_UPPER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT LOWER LEG
	format.initialPosition( vec3( -0.18, 0.2, 0. ) ).collisionShape( mShapes[BODYPART_LEFT_LOWER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// RIGHT UPPER LEG
	format.initialPosition( vec3( 0.18, 0.65, 0. ) ).collisionShape( mShapes[BODYPART_RIGHT_UPPER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// RIGHT LOWER LEG
	format.initialPosition( vec3( 0.18, 0.2, 0. ) ).collisionShape( mShapes[BODYPART_RIGHT_LOWER_LEG] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT UPPER ARM
	format.initialPosition( vec3( -0.35, 1.45, 0.0 ) ).initialRotation( quat( vec3( 0, 0, M_PI_2 ) ) ).collisionShape( mShapes[BODYPART_LEFT_UPPER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// LEFT LOWER ARM
	format.initialPosition( vec3( -0.7, 1.45, 0.0 ) ).initialRotation( quat( vec3( 0, 0, M_PI_2 ) ) ).collisionShape( mShapes[BODYPART_LEFT_LOWER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	format.initialPosition( vec3( 0.35, 1.45, 0.0 ) ).initialRotation( quat( vec3( 0, 0, -M_PI_2 ) ) ).collisionShape( mShapes[BODYPART_RIGHT_UPPER_ARM] );
	body = RigidBody::create( format );
	mBodies.push_back( body );
	mMotionStates.push_back( SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( offset * body->getCenterOfMassTransform() ) ) );
	mBodies.back()->setMotionState( mMotionStates.back() );
	
	// RIGHT LOWER ARM
	format.initialPosition( vec3( 0.7, 1.45, 0.0 ) ).initialRotation( quat( vec3( 0, 0, -M_PI_2 ) ) ).collisionShape( mShapes[BODYPART_RIGHT_LOWER_ARM] );
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
			(*bodyIt)->setDamping(0.5f, 0.85f);
			(*bodyIt)->setDeactivationTime(0.8f);
			(*bodyIt)->setSleepingThresholds(4.0f, 4.0f);
		}
	}
	
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.0f, 0.15f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,float(M_PI_2),0.0f);
		auto bodyA = mBodies[BODYPART_PELVIS];
		b.setOrigin( bt::toBullet( vec3( 0.0f, -0.15f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f, float( M_PI_2),0.0f  );
		auto bodyB = mBodies[BODYPART_SPINE];
		
		auto constraint = make_shared<btHingeConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b, false );
		constraint->setLimit( btScalar(-M_PI_4), btScalar(M_PI_2) );
		constraint->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( constraint );
	}
	
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.0f, 0.30f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,0.0f,float(M_PI_2));
		auto bodyA = mBodies[BODYPART_SPINE];
		b.setOrigin( bt::toBullet( vec3( 0.0f, -0.14f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,0.0f,float(M_PI_2)  );
		auto bodyB = mBodies[BODYPART_HEAD];
		
		auto cone = make_shared<btConeTwistConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b );
		cone->setLimit( float( M_PI_4 ), float( M_PI_4 ), float( M_PI_2 ) );
		cone->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	// Something's going on with quaternions here
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( -0.18f, -0.10f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f, 0.0f, -float(M_PI_4)*5.0f);
		auto bodyA = mBodies[BODYPART_PELVIS];
		b.setOrigin( bt::toBullet( vec3( 0.0f, 0.225f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f, 0.0f, -float(M_PI_4)*5.0f  );
		auto bodyB = mBodies[BODYPART_LEFT_UPPER_LEG];
		
		auto cone = make_shared<btConeTwistConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b );
		cone->setLimit( float( M_PI_4 ), float( M_PI_4 ), float( M_PI_2 ) );
		cone->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}
	
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.0f, -0.225f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,float(M_PI_2),0.0f);
		auto bodyA = mBodies[BODYPART_LEFT_UPPER_LEG];
		b.setOrigin( bt::toBullet( vec3( 0.0f, 0.185f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,float(M_PI_2),0.0f  );
		auto bodyB = mBodies[BODYPART_LEFT_LOWER_LEG];
		
		auto constraint = make_shared<btHingeConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b, false );
		constraint->setLimit( btScalar(0), btScalar(M_PI_2)  );
		constraint->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( constraint );
	}
	
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.18f, -0.10f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f, 0.0f, float(M_PI_4));
		auto bodyA = mBodies[BODYPART_PELVIS];
		b.setOrigin( bt::toBullet( vec3( 0.0f, 0.225f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f, 0.0f, float(M_PI_4) );
		auto bodyB = mBodies[BODYPART_RIGHT_UPPER_LEG];
		
		auto cone = make_shared<btConeTwistConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b );
		cone->setLimit( float( M_PI_4 ), float( M_PI_4), 0.0f );
		cone->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}
	
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.0f, -0.225f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,float(M_PI_2),0.0f);
		auto bodyA = mBodies[BODYPART_RIGHT_UPPER_LEG];
		b.setOrigin( bt::toBullet( vec3( 0.0f, 0.185f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,float(M_PI_2),0.0f );
		auto bodyB = mBodies[BODYPART_RIGHT_LOWER_LEG];
		
		auto constraint = make_shared<btHingeConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b, false );
		constraint->setLimit( btScalar(0), btScalar(M_PI_2)  );
		constraint->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( constraint );
	}

	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( -0.2f, 0.15f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,0.0f,float(M_PI));
		auto bodyA = mBodies[BODYPART_SPINE];
		b.setOrigin( bt::toBullet( vec3( 0.0f, -0.18f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,0.0f,float(M_PI_2) );
		auto bodyB = mBodies[BODYPART_LEFT_UPPER_ARM];
		
		auto cone = make_shared<btConeTwistConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b );
		cone->setLimit( float(M_PI_2), float(M_PI_2), 0.0f );
		cone->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.0f, 0.18f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,float(M_PI_2),0.0f);
		auto bodyA = mBodies[BODYPART_LEFT_UPPER_ARM];
		b.setOrigin( bt::toBullet( vec3( 0.0f, -0.14f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,float(M_PI_2),0.0f );
		auto bodyB = mBodies[BODYPART_LEFT_LOWER_ARM];
		
		auto constraint = make_shared<btHingeConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b, false );
		constraint->setLimit( btScalar(0), btScalar(M_PI_2) );
		constraint->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( constraint );
	}
	
	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.2f, 0.15f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,0.0f,0.0f);
		auto bodyA = mBodies[BODYPART_SPINE];
		b.setOrigin( bt::toBullet( vec3( 0.0f, -0.18f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,0.0f,float(M_PI_2) );
		auto bodyB = mBodies[BODYPART_RIGHT_UPPER_ARM];
		
		auto cone = make_shared<btConeTwistConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b );
		cone->setLimit( float(M_PI_2), float(M_PI_2), 0.0f );
		cone->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( cone );
	}

	{
		btTransform a, b;
		a.setOrigin( bt::toBullet( vec3( 0.0f, 0.18f, 0.0f ) ) );
		a.getBasis().setEulerZYX(0.0f,float(M_PI_2),0.0f);
		auto bodyA = mBodies[BODYPART_RIGHT_UPPER_ARM];
		b.setOrigin( bt::toBullet( vec3( 0.0f, -0.14f, 0.0f ) ) );
		b.getBasis().setEulerZYX( 0.0f,float(M_PI_2),0.0f );
		auto bodyB = mBodies[BODYPART_RIGHT_LOWER_ARM];
		
		auto constraint = make_shared<btHingeConstraint>( *bodyA->getRigidBody(), *bodyB->getRigidBody(), a, b, false );
		constraint->setLimit( btScalar(0), btScalar(M_PI_2) );
		constraint->setDbgDrawSize( CONSTRAINT_DEBUG_SIZE );
		mConstraints.push_back( constraint );
	}
	
	{
		auto jointIt = mConstraints.begin();
		auto end = mConstraints.end();
		for( ; jointIt != end; ++jointIt ) {
			mOwner->addConstraint( (*jointIt), true );
		}
	}
}

RagDollRef RagDoll::create( const bullet::ContextRef &context, const ci::vec3 &positionOffset, const ci::gl::GlslProgRef &glsl  )
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
