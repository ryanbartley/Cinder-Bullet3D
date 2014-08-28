#include "Body.h"
#include "Cinder-Bullet3D/MotionState.h"
#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace bullet;

Body::Body( int index, float size, const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics )
: mId( index ), mSize( size ), mVisObj( visual ), mPhyObj( physics )
{
	mMotionState	= SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( mPhyObj->getCenterOfMassTransform() ) );
	mPhyObj->setMotionState( mMotionState );
	auto s			= fromBullet( mPhyObj->getCollisionShape()->getLocalScaling() );
	mScaleMatrix	= mat4();
	mScaleMatrix	= scale( mScaleMatrix, s );
	
	mScale			= 1.0f;
	mStrength		= 1.0f;
}

void Body::update( float time )
{
	mMotionState->getGLWorldTransform( &mModelMatrix );
	btTransform trans;
	mMotionState->getWorldTransform( trans  );
	mPos = fromBullet( trans.getOrigin() );
	
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
	mPhyObj->setLinearVelocity( mPhyObj->getLinearVelocity() - vel );
}

void Body::draw()
{
	gl::pushModelMatrix();
	gl::setModelMatrix( mModelMatrix );
	gl::multModelMatrix( mScaleMatrix );
	gl::scale( vec3( mScale ) );
	mVisObj->draw();
	gl::popModelMatrix();
}
