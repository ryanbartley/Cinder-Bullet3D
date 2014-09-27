#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Cinder-Bullet3D/BulletContext.h"
#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/Common.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/Perlin.h"
#include "btHeightfieldTerrainShape.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"

// This sample exemplifies the use of the btHeightfieldTerrainShape and
// the opengl helpers that accompany it. You don't have to use these
// helpers but there are a couple of gotcha's that are worked out in
// the helpers that you can look at.

using namespace ci;
using namespace ci::app;
using namespace std;

const uint32_t Width = 50;
const uint32_t Depth = 50;

class HeightfieldTerrainApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	void setupHeightfield();
	void setupShader();
	
	void addBox();
	
	bt::ContextRef		mContext;
	
	gl::GlslProgRef		mPhongShader;
	gl::BatchRef		mBatch;
	gl::VaoRef			mVao;
	gl::VboMeshRef		mDrawableHeightfield;
	std::vector<bt::RigidBodyRef>	mRigidBodies;
	bt::RigidBodyRef	mHeightfieldTerrain;
	Channel32f			mHeightfieldMap;
	CameraPersp			mCam;
};

void HeightfieldTerrainApp::setup()
{
	// Create a regular context as in the PhysicsPrimitives example.
	mContext = bt::Context::create( bt::Context::Format().drawDebug( true ).createDebugRenderer( true ) );
	
	// First we setup our shaders so that they can be used in our
	// heightfield batch.
	setupShader();
	// Next we setup the heightfield.
	setupHeightfield();
	
	// Next let's test our heightfield by adding a box to the environment
	addBox();
	
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
	mCam.lookAt( vec3( 0, 5, 10 ), vec3( 0.0f ) );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void HeightfieldTerrainApp::setupShader()
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
}

void HeightfieldTerrainApp::setupHeightfield()
{
	mHeightfieldMap = Channel32f( Width, Depth );
	
	auto minHeight = -1.0f;
	auto maxHeight = 1.0f;
	
	// Now we'll give our Map some Height values.
	for( int y = 0; y < Depth; ++y ) {
		for( int x = 0; x < Width; ++x ) {
			// Now create our heightfield, which is just a two-dimensional array that has
			// a value, height. Instead of this you could easily use Perline to get smoother
			// values or create a black and white image.
			mHeightfieldMap.setValue( ivec2( x, y ), randFloat( minHeight, maxHeight ) );
		}
	}
	
	// Create our heightfieldShape with our heightfieldMap and maxHeight and minHeight
	auto heightField = bt::createHeightfieldShape( &mHeightfieldMap, maxHeight, minHeight );
	
	// Make our rigidbody out of this collision shape.
	mRigidBodies.push_back( bt::RigidBody::create( bt::RigidBody::Format().collisionShape( heightField ) ) );
	// And add that rigidbody to the world.
	mContext->addRigidBody( mRigidBodies.back() );
	
	// also I'm going to take a reference to the mHeightfieldTerrain
	mHeightfieldTerrain = mRigidBodies.back();
	
	// Now we can create a batch using the draw helper from bullet which passes back a vboMesh based on our map.
	mBatch = gl::Batch::create( bt::drawableHelpers::getDrawableHeightfield( &mHeightfieldMap ), mPhongShader );
}

void HeightfieldTerrainApp::addBox()
{
	// This is just a simple function to add some box shapes to our world. We're not
	// creating a corresponding visual object so you'll only be able to see them if
	// you have debug draw working.
	auto box = bt::createBoxShape( vec3( 1.0f ) );
	mRigidBodies.push_back( bt::RigidBody::create( bt::RigidBody::Format()
												  .collisionShape( box )
												  .initialPosition( vec3( randFloat(-5, 5), 10, randFloat(-5, 5) ) )
												  .mass( 1.0f ) ) );
	mContext->addRigidBody( mRigidBodies.back() );
}

void HeightfieldTerrainApp::mouseDown( MouseEvent event )
{
	// anytime we click we'll get a box.
	addBox();
}

void HeightfieldTerrainApp::update()
{
	// must update the world.
	mContext->update();
}

void HeightfieldTerrainApp::draw()
{
	static float rotation = 0.0f;
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	// set our camera
	gl::setMatrices( mCam );
	// rotate the camera position
	gl::multViewMatrix( rotate( toRadians( rotation += 0.1 ), vec3( 0.0f, 1.0f, 0.0f ) ) );
	{
		gl::ScopedModelMatrix scope;
		// Transform it with the physics heightfield
		gl::multModelMatrix( translate( bt::fromBullet( mHeightfieldTerrain->getRigidBody()->getWorldTransform().getOrigin() ) ) );
		mBatch->draw();
	}
	// If we want to draw the debug we can set this and just toggle on and off using...
	//
	// mContext->toggleDebugDraw()
	//
	mContext->debugDraw();
}


CINDER_APP_NATIVE( HeightfieldTerrainApp, RendererGl )
