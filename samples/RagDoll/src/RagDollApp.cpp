#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "RagDoll.h"
#include "Cinder-Bullet3D/BulletContext.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RagDollApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void update();
	void draw();
	
	bullet::ContextRef		mContext;
	std::vector<RagDollRef> mRagDolls;
	bullet::RigidBodyRef	mPlane;
	CameraPersp				mCam;
	bool					mDraw;
};

void RagDollApp::setup()
{
	// Create a context. This Context stores all of the bullet world.
	// Take a look at the Context::Format to find out just what can be
	// controlled. Specifically here, we're creating a format that allows
	// debug Drawing
	bullet::Context::Format format;
	format.drawDebug( true ).createDebugRenderer( true );
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	format.broadphase( new btAxisSweep3 (worldAabbMin, worldAabbMax) );
	
	mContext = bullet::Context::create( format );
	
	mRagDolls.push_back( RagDoll::create( mContext, Vec3f( 1, .5, 0 ) ) );
	
	mPlane = bullet::RigidBody::create( bullet::RigidBody::Format().collisionShape( bullet::createStaticPlaneShape( Vec3f( 0, 1, 0 ), -1 ) ).mass( 0.0 ).addToWorld( true ) );
	
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), .01f, 1000.0 );
	mCam.lookAt( Vec3f( 0, 5, 5 ), Vec3f::zero() );
	
	mDraw = false;
}

void RagDollApp::mouseDown( MouseEvent event )
{
	mRagDolls.push_back( RagDoll::create( mContext, Vec3f( -1, .5, 0 ) ) );
}

void RagDollApp::keyDown( KeyEvent event )
{
	if( event.getCode() == KeyEvent::KEY_RIGHT ) {
		mDraw = false;
	}
}

void RagDollApp::update()
{
//	if( ! mDraw ) {
		mContext->update();
	
//		cout << mContext->world()->getNumConstraints() << " " << mContext->world()->getNumCollisionObjects() << endl;
//		mDraw = true;
//	}
}

void RagDollApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::setMatrices( mCam );
	
	mContext->debugDraw();
}

CINDER_APP_NATIVE( RagDollApp, RendererGl )
