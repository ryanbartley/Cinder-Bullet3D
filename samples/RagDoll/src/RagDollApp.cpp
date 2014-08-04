#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "RagDoll.h"
#include "Cinder-Bullet3D/BulletContext.h"

#include "cinder/gl/GlslProg.h"

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
	gl::BatchRef			mVisPlane;
	CameraPersp				mCam;
	gl::GlslProgRef			mPhongShader;
};

void RagDollApp::setup()
{
	mPhongShader = gl::GlslProg::create( gl::GlslProg::Format()
#if ! defined( CINDER_GL_ES )
									 .vertex( loadAsset( "Phong.vert" ) )
									 .fragment( loadAsset( "Phong.frag" ) )
#else
									 .vertex( loadAsset( "Phong_ios.vert" ) )
									 .fragment( loadAsset( "Phong_ios.frag" ) )
#endif
									 );
	
	// Create a context. This Context stores all of the bullet world.
	// Take a look at the Context::Format to find out just what can be
	// controlled. Specifically here, we're creating a format that allows
	// debug Drawing
	bullet::Context::Format format;
	format.drawDebug( true ).createDebugRenderer( true );
	// I'm going to use a different broadphase, which I set up by creating
	// a bounding box.
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	// Then I pass this pointer to my format broadphase
	format.broadphase( new btAxisSweep3 (worldAabbMin, worldAabbMax) );
	// And create my context
	mContext = bt::Context::create( format );
	
	// Next I'll create my initial rag doll.
	mRagDolls.push_back( RagDoll::create( mContext, Vec3f( 1, .5, 0 ), mPhongShader ) );
	
	// Then I'll create my ground plane as normal
	mPlane = bullet::RigidBody::create( bullet::RigidBody::Format()
									   .collisionShape( bullet::createStaticPlaneShape( Vec3f( 0, 1, 0 ), -1 ) )
									   .mass( 0.0 )
									   .addToWorld( true ) );
	mVisPlane = gl::Batch::create( geom::Rect().scale( Vec2f( 1000, 1000 ) ), mPhongShader );
	
	// I'll create my camera so that I can see.
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), .01f, 1000.0 );
	mCam.lookAt( Vec3f( 0, 5, 5 ), Vec3f::zero() );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void RagDollApp::mouseDown( MouseEvent event )
{
	// This is just so that I can see more ragdolls
	mRagDolls.push_back( RagDoll::create( mContext, Vec3f( (rand() % 10) - 5 , .5, (rand() % 10) - 7 ), mPhongShader ) );
}

void RagDollApp::keyDown( KeyEvent event )
{
	if( event.getCode() == KeyEvent::KEY_d ) {
		mContext->toggleDebugDraw();
	}
}

void RagDollApp::update()
{
	mContext->update();
}

void RagDollApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::setMatrices( mCam );
	
	// Draw my plane
	gl::pushModelMatrix();
	gl::multModelMatrix( Matrix44f::createRotation( Vec3f( 1, 0, 0 ), toRadians( -90.0f ) ) );
		mVisPlane->draw();
	gl::popModelMatrix();
	
	// Draw the ragdolls
	gl::pushModelMatrix();
	auto ragIt = mRagDolls.begin();
	auto end = mRagDolls.end();
	while( ragIt != end ) {
		(*ragIt++)->draw();
	}
	gl::popModelMatrix();
	
	// If activated I'll draw the debug
	mContext->debugDraw();
}

CINDER_APP_NATIVE( RagDollApp, RendererGl )
