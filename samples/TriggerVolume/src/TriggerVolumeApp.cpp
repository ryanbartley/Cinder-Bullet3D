#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/BulletContext.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const float EXPLOSION_STRENGTH = 50.0f;

class TriggerVolumeApp : public AppNative {
public:
	void setup() override;
	void keyDown( KeyEvent event ) override;
	void keyUp( KeyEvent event ) override;
	void mouseMove( MouseEvent event ) override;
	void addBox();
	void collisionBegin( btRigidBody *rigid0, btRigidBody *rigid1 );
	void collisionEnd( btRigidBody *rigid0, btRigidBody *rigid1 );
	void update() override;
	void draw() override;
	
	bullet::ContextRef				mContext;
	std::vector<bt::RigidBodyRef>	mBoxes;
	bt::BoxShapeRef					mBoxShape;
	bt::RigidBodyRef				mPlane, mTrigger, mExplosion;
	
	bool							mCanExplode;
	
	vec2							mCurrentMousePos;
	CameraPersp						mCam;
};

void TriggerVolumeApp::setup()
{
	mCanExplode = true;
	mContext = bt::Context::create( bt::Context::Format()
								   .drawDebug( true )
								   .createDebugRenderer( true ) );
	
	mBoxShape = bt::createBoxShape( vec3( 1, 1, 1 ) );
	mTrigger = bt::RigidBody::create( bt::RigidBody::Format()
									 .collisionShape( bt::createBoxShape( vec3( 10, 1, 1 ) ) )
									 .initialPosition( vec3( 0, 0, 0 ) ) );
	// This is the magic right here. It's pretty semantic. You want a rigid body that
	// is basically invisible to the system, other than the callbacks.
	mTrigger->setCollisionFlags( btCollisionObject::CF_NO_CONTACT_RESPONSE );
	
	auto planeShape = bt::createStaticPlaneShape( vec3( 0, 1, 0 ), -1 );
	mPlane = bt::RigidBody::create( bt::RigidBody::Format()
								   .collisionShape( planeShape ) );
	
	addBox();
	
	mContext->addRigidBody( mTrigger );
	mContext->addRigidBody( mPlane );
	
	mContext->addCollisionBeginSignal( std::bind( &TriggerVolumeApp::collisionBegin, this, std::placeholders::_1, std::placeholders::_2 ) );
	mContext->addCollisionEndSignal( std::bind( &TriggerVolumeApp::collisionEnd, this, std::placeholders::_1, std::placeholders::_2 ) );
	
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
	mCam.lookAt( vec3( 0, 0, 10 ), vec3( 0 ) );
}

void TriggerVolumeApp::addBox()
{
	mBoxes.push_back( bt::RigidBody::create( bt::RigidBody::Format()
											.collisionShape( mBoxShape )
											.initialPosition( vec3( randFloat(-10, 10), 10, 0 ) ).mass( 1.0f ) ) );
	mContext->addRigidBody( mBoxes.back() );
}

void TriggerVolumeApp::keyDown( KeyEvent event )
{
	// Impulse testing
	if( event.getChar() =='e' ) {
		// don't create a new explosion if one already exists
		// or we haven't released the key, yet
		if ( mExplosion || ! mCanExplode ) return;
		// don't let us create another explosion until the key is released
		mCanExplode = false;
		
		// get the position that we clicked using ray testing,
		// see RayCasting example for more details
		bt::RayResult result;
		auto ray = mCam.generateRay( mCurrentMousePos.x, mCurrentMousePos.y, getWindowAspectRatio() );
		
		if( mContext->closestRayCast( ray.getOrigin(), ray.getDirection(), result ) ) {
			// create a collision object for our explosion
			mExplosion = bt::RigidBody::create( bt::RigidBody::Format()
											   .collisionShape( bt::createSphereShape( 3.0f ) )
											   .initialPosition( result.hitPoint ) );
			// set the collision flag
			mExplosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
			// add the explosion trigger to our world
			mContext->addRigidBody( mExplosion );
		}
	}
}

void TriggerVolumeApp::keyUp( KeyEvent event )
{
	if( event.getChar() == 'e' ) {
		// reset the explode flag
		mCanExplode = true;
	}
}

void TriggerVolumeApp::mouseMove( MouseEvent event )
{
	mCurrentMousePos = vec2( event.getPos() ) / vec2( getWindowSize() );
	mCurrentMousePos.y = 1.0f - mCurrentMousePos.y;
}

void TriggerVolumeApp::collisionBegin( btRigidBody *rigid0, btRigidBody *rigid1 )
{
	auto triggerBody = mTrigger->getRigidBody().get();
	if( rigid0 == triggerBody || rigid1 == triggerBody ) {
		addBox();
	}
	
	if( ! mExplosion ) return;
	
	auto explosionBody = mExplosion->getRigidBody().get();
	// Impulse testing
	if ( rigid0 == explosionBody || rigid1 == explosionBody ) {
		// get the pointer of the other object
		btRigidBody* pOther;
		pOther = rigid0 == explosionBody ? (btRigidBody*)rigid1 : (btRigidBody*)rigid0;
		// wake the object up
		pOther->setActivationState(ACTIVE_TAG);
		// calculate the vector between the object and
		// the center of the explosion
		auto dir = bt::fromBullet( pOther->getWorldTransform().getOrigin() ) - bt::fromBullet( mExplosion->getCenterOfMassPoint() );
		// get the distance
		float dist = length( dir );
		// calculate the impulse strength
		float strength = EXPLOSION_STRENGTH;
		// follow an inverse-distance rule
		if (dist != 0.0) strength /= dist;
		// normalize the direction vector
		dir = normalize( dir );
		// apply the impulse
		pOther->applyCentralImpulse( bt::toBullet( dir * strength ) );
	}
}

void TriggerVolumeApp::collisionEnd( btRigidBody *rigid0, btRigidBody *rigid1 )
{
	if( rigid0 == mTrigger->getRigidBody().get() || rigid1 == mTrigger->getRigidBody().get() ) {
		addBox();
	}
}

void TriggerVolumeApp::update()
{
	mContext->update();
	
	// Impulse testing
	if ( mExplosion ) {
		// destroy the explosion object after one iteration
		mContext->removeRigidBody( mExplosion );
		mExplosion.reset();
	}
	
	mCam.lookAt( vec3( 0, 4, 15 ), vec3( 0 ) );
}

void TriggerVolumeApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::setMatrices( mCam );
	
	mContext->debugDraw();
}

CINDER_APP_NATIVE( TriggerVolumeApp, RendererGl )
