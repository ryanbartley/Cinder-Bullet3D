#pragma once

#include "Cinder-Bullet3D/RigidBody.h"
#include "cinder/gl/Batch.h"

class Body {
public:
	Body( int index, float size, const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics );
	Body( int index, float size, const bullet::RigidBodyRef &physics );
	~Body() {}
	
	inline void update( float time );
	inline void attract( const ci::vec3 &attractorPos, float attractorScale, float strength );
	inline void draw();
	inline ci::mat4 getMatrix() { return mModelMatrix * ci::scale( ci::vec3( getSize() ) ); };
	
	inline int						getId(){		return mId; }
	inline ci::vec3					getPos(){		return mPos; }
	inline float					getSize(){		return mSize * mScale; }
	inline float					getStrength(){	return mStrength; }
	
private:
	int						mId;
	float					mSize;
	ci::vec3				mPos;
	float					mScale;
	float					mStrength;
	
	ci::gl::BatchRef		mVisObj;
	bullet::RigidBodyRef	mPhyObj;
	bullet::SimpleGlDynamicMotionStateRef	mMotionState;
	ci::mat4				mModelMatrix;
};

#include "Body.h"
#include "Cinder-Bullet3D/MotionState.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace bullet;

Body::Body( int index, float size, const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics )
: mId( index ), mSize( size ), mVisObj( visual ), mPhyObj( physics ), mScale( 1.0f ), mStrength( 1.0f )
{
	mMotionState	= SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( mPhyObj->getCenterOfMassTransform() ) );
	mPhyObj->setMotionState( mMotionState );
}

Body::Body( int index, float size, const bullet::RigidBodyRef &physics )
: mId( index ), mSize( size ), mPhyObj( physics ), mScale( 1.0f ), mStrength( 1.0f )
{
	mMotionState	= SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( mPhyObj->getCenterOfMassTransform() ) );
	mPhyObj->setMotionState( mMotionState );
}

void Body::update( float time )
{
	mMotionState->getGLWorldTransform( &mModelMatrix );
	mPos = vec3(mModelMatrix[3]);
	
	if( mId < 2 ){	// if this is a big sphere, change size
		float sinVal	= sin( time + mId * M_PI ) * 0.5f + 0.5f;
		mScale			= sinVal * 2.5f + 0.5f;
		mStrength		= sinVal * 10.0f;
	}
	
	mPhyObj->getRigidBody()->getCollisionShape()->setLocalScaling( bt::toBullet( vec3( mScale ) ) );
}

void Body::attract( const vec3 &attractorPos, float attractorScale, float strength )
{
	auto dir	= mPos - attractorPos;
	auto dist	= length( dir );
	auto vel	= normalize( dir ) * ( 1.0f / ( dist * dist ) ) * attractorScale * mScale * 100.0f;
	mPhyObj->setLinearVelocity( (mPhyObj->getLinearVelocity() - vel) );
}

void Body::draw()
{
	gl::pushModelMatrix();
	gl::setModelMatrix( mModelMatrix );
	gl::scale( vec3( mScale ) );
	mVisObj->draw();
	gl::popModelMatrix();
}
