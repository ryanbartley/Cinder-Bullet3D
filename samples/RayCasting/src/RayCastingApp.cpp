#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/BulletContext.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// this is a simple but useful struct for allowing us to
// not delete raw pointers in shared_ptr. See use below
struct null_deleter
{
	void operator()(void const *) const
	{
	}
};

class RayCastingApp : public AppNative {
public:
	void setup() override;
	void prepareSettings( Settings *settings ) { settings->enableMultiTouch( false ); }
	void keyDown( KeyEvent event ) override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void mouseUp( MouseEvent event ) override;
	void mouseMove( MouseEvent event ) override;
	void updateMousePosition( const ci::vec2 &mousePos );
	void update() override;
	void draw() override;
	
	void createPickingConstraint( const bt::RayResult &result );
	void removePickingConstraint();
	
	bt::ContextRef				mContext;
	std::list<bt::RigidBodyRef> mBoxes;
	bt::RigidBodyRef			mPlane;
	
	// Pure bullet objects
	bt::btRigidBodyRef				mPickedBody;
	bt::btGeneric6DofConstraintRef	mPickConstraint;
	btScalar						mOldPickingDistance;
	
	vec2						mCurrentMousePos;
	CameraPersp					mCam;
};

void RayCastingApp::setup()
{
	// create a simple context with a debug renderer
	mContext = bt::Context::create( bt::Context::Format()
								   .drawDebug( true )
								   .createDebugRenderer( true ) );
	
	// put one box in the world
	mBoxes.push_back( bt::RigidBody::create( bt::RigidBody::Format()
											.collisionShape( bt::createBoxShape( vec3( 1 ) ) )
											.initialPosition( vec3( 0, 10, 0 ) )
											.addToWorld( true )
											.mass( 1.0f ) ) );
	// create a simple ground plane
	mPlane = bt::RigidBody::create( bt::RigidBody::Format()
								   .collisionShape( bt::createStaticPlaneShape( vec3( 0, 1, 0 ), 0 ) )
								   .addToWorld( true ) );
	
	// create a camera for debug drawing
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
	mCam.lookAt( vec3( 0, 3, 20 ), vec3( 0 ) );
}

void RayCastingApp::createPickingConstraint( const bt::RayResult &result )
{
	// store the body for future reference, since I want to use shared_ptr's
	// and I don't want the shared_ptr's to delete the body after I'm done
	// using the object (the object is owned by the list and raw by the bullet
	// context) i create the null_deleter functor that does nothing when passed a
	// raw pointer. See above.
	mPickedBody.reset( result.pBody, null_deleter() );
	// prevent the picked object from falling asleep
	mPickedBody->setActivationState( DISABLE_DEACTIVATION );
	
	// get the hit position relative to the body we hit
	auto localPivot = bt::fromBullet( mPickedBody->getCenterOfMassTransform().inverse() ) * vec4( result.hitPoint, 1.0 );
	
	// create a transform for the pivot point
	btTransform pivot;
	pivot.setIdentity();
	pivot.setOrigin( bt::toBullet( vec3( localPivot ) ) );
	// create our constraint object
	mPickConstraint.reset( new btGeneric6DofConstraint( *mPickedBody, pivot, true ) );
	bool bLimitAngularMotion = true;
	if (bLimitAngularMotion) {
		mPickConstraint->setAngularLowerLimit(btVector3(0,0,0));
		mPickConstraint->setAngularUpperLimit(btVector3(0,0,0));
	}
	// add the constraint to the world
	mContext->addConstraint( mPickConstraint, true );
	// define the 'strength' of our constraint (each axis)
	float cfm = 0.5f;
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_CFM,cfm,0);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_CFM,cfm,1);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_CFM,cfm,2);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_CFM,cfm,3);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_CFM,cfm,4);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_CFM,cfm,5);
	// define the 'error reduction' of our constraint (each axis)
	float erp = 0.5f;
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_ERP,erp,0);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_ERP,erp,1);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_ERP,erp,2);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_ERP,erp,3);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_ERP,erp,4);
	mPickConstraint->setParam(BT_CONSTRAINT_STOP_ERP,erp,5);
	// save this data for future reference
	mOldPickingDistance  = ci::length(result.hitPoint - mCam.getEyePoint());
}

void RayCastingApp::removePickingConstraint()
{
	// if we don't have a pick constraint don't try to use or delete it
	if ( !mPickConstraint || !mContext )
		return;
	
	// remove the constraint from the world
	mContext->removeConstraint( mPickConstraint );
	// reactivate the body
	mPickedBody->forceActivationState(ACTIVE_TAG);
	mPickedBody->setDeactivationTime( 0.f );
	
	// clear the pointers
	mPickConstraint.reset();
	mPickedBody.reset();
}

void RayCastingApp::updateMousePosition( const ci::vec2 &mousePos )
{
	// update global mouse position
	mCurrentMousePos = ci::vec2( mousePos / ci::vec2(getWindowSize()) );
	mCurrentMousePos.y = 1.0f - mCurrentMousePos.y;
}

void RayCastingApp::mouseDown( MouseEvent event )
{
	auto ray = mCam.generateRay( mCurrentMousePos.x, mCurrentMousePos.y, getWindowAspectRatio() );
	bt::RayResult result;
	// raycast into the bullet context and just grab the closest hit object
	if( mContext->closestRayCast( mCam.getEyePoint(), ray.getDirection(), result ) ) {
		// if we've found something create a picking constraint
		createPickingConstraint( result );
	}
}

void RayCastingApp::mouseDrag( MouseEvent event )
{
	// must update global mouse position here because
	// the "mouseMove" function won't be called while
	// mouse button is down
	updateMousePosition( event.getPos() );
	
	// did we pick a body with the LMB?
	if ( mPickedBody ) {
		
		// make sure we actually have a constraint
		if ( ! mPickConstraint )
			return;
		
		// use another picking ray to get the target direction
		auto ray = mCam.generateRay( mCurrentMousePos.x, mCurrentMousePos.y, getWindowAspectRatio() );
		auto dir = ci::normalize(ray.getDirection());
		
		// use the same distance as when we originally picked the object
		dir *= mOldPickingDistance;
		auto newPivot = mCam.getEyePoint() + dir;
		// set the position of the constraint
		mPickConstraint->getFrameOffsetA().setOrigin( bt::toBullet( newPivot ) );
	}
}

void RayCastingApp::mouseUp( MouseEvent event )
{
	// remove our picking constraint and picked body
	removePickingConstraint();
}

void RayCastingApp::mouseMove( MouseEvent event )
{
	// I want to update mouse's position anytime the mouse moves
	// for the deleter in keyDown
	updateMousePosition( event.getPos() );
}

void RayCastingApp::keyDown( KeyEvent event )
{
	// we want to delete
	if( event.getChar() == 'd' ) {
		// get the ray from the mouse
		auto ray = mCam.generateRay( mCurrentMousePos.x, mCurrentMousePos.y, getWindowAspectRatio() );
		
		bt::RayResult result;
		// check if we've hit something
		if( mContext->closestRayCast( mCam.getEyePoint(), ray.getDirection(), result ) ) {
			// we must have hit an object
			auto chosenBody = result.pBody;
			// loop through our boxes
			for( auto beginIt = mBoxes.begin(); beginIt != mBoxes.end(); ++beginIt ) {
				// get the raw pointer to the rigidbody
				auto boxBody = (*beginIt)->getRigidBody().get();
				// if the stored pointer is the same as the picked pointer
				if( boxBody == chosenBody ) {
					// first remove it from the world
					mContext->removeRigidBody( (*beginIt) );
					// then erase it from our list
					mBoxes.erase( beginIt );
					break;
				}
			}
		}
	}
	// simple random box generator
	if( event.getCode() == KeyEvent::KEY_SPACE ) {
		mBoxes.push_back( bt::RigidBody::create( bt::RigidBody::Format()
												.collisionShape( bt::createBoxShape( vec3( 1 ) ) )
												.initialPosition( vec3( randFloat( -10, 10 ), 10, 0 ) )
												.addToWorld( true )
												.mass( 1.0f ) ) );
	}
}

void RayCastingApp::update()
{
	mContext->update();
}

void RayCastingApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::setMatrices( mCam );
	mContext->debugDraw();
}

CINDER_APP_NATIVE( RayCastingApp, RendererGl )
