#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "Cinder-Bullet3D/BulletContext.h"
#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/Common.h"

#include "cinder/gl/VboMesh.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "cinder/AxisAlignedBox.h"
#include "cinder/CameraUi.h"

// This sample exemplifies the use of the btHeightfieldTerrainShape and
// the opengl helpers that accompany it. You don't have to use these
// helpers but there are a couple of gotcha's that are worked out in
// the helpers that you can look at.

using namespace ci;
using namespace ci::app;
using namespace std;

const uint32_t Width = 50;
const uint32_t Depth = 50;

class HeightfieldTerrainApp : public App {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void update();
	void draw();
	
	void setupHeightfield();
	void setupShader();
	
	void addBox();
	
	bt::ContextRef		mContext;
	
	gl::GlslProgRef		mPhongShader;
	gl::BatchRef		mBatch;
	std::vector<bt::RigidBodyRef>	mRigidBodies;
	Channel32f			mHeightfieldMap;
	bt::RigidBodyRef	mHeightfieldTerrain;
	CameraPersp			mCam;
	CameraUi			mCamUi;
	float				mCamHeight;
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
	mCam.lookAt( vec3( 0, 15, 20 ), vec3( 1.0f ) );
	mCamUi.setCamera( &mCam );
	mCamUi.connect( getWindow() );
	
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
	Perlin perlin( 4, 12 );
	mHeightfieldMap = Channel32f( Width, Depth );

	// create a box to get the extents of our map.
	ci::AxisAlignedBox box;
	bool set = false;
	// Now we'll give our Map some Height values.
	for( int y = 0; y < Depth; ++y ) {
		for( int x = 0; x < Width; ++x ) {
			// Now create our heightfield, which is just a two-dimensional array that has
			// a value, height. Instead of this you could easily use Perlin to get smoother
			// values or create a black and white image.
			auto val = 25.0f * perlin.fBm( x/float(Depth), y/float(Width) );
			// set the values inside the box.
			if( !set ) {
				box.set( vec3( x, val, y ), vec3( x, val, y ) );
				set = true;
			}
			else
				box.include( vec3( x, val, y ) );
			// and add the height into the heightfield.
			mHeightfieldMap.setValue( ivec2( x, y ), val );
		}
	}
	// because bullet wants to recenter the heightfield, we'll first translate it here.
	auto centerOffset = box.getCenter().y;
	for( int y = 0; y < Depth; ++y ) {
		for( int x = 0; x < Width; ++x ) {
			// get the value and offset it from center y.
			auto translatedY = mHeightfieldMap.getValue( vec2( x, y ) ) - centerOffset;
			// reset the value stored in the heightmap.
			mHeightfieldMap.setValue( vec2( x, y ), translatedY );
		}
	}
	// now get the recentered min and max height.
	auto minHeight = box.getMin().y - centerOffset;
	auto maxHeight = box.getMax().y - centerOffset;

	// Create our heightfieldShape with our heightfieldMap and maxHeight and minHeight
	auto heightField = bt::createHeightfieldShape( &mHeightfieldMap, maxHeight, minHeight, vec3( 10 ) );
	heightField->setLocalScaling( bt::toBullet( vec3( 100 ) ) );
	
	// Make our rigidbody out of this collision shape.
	mHeightfieldTerrain = bt::RigidBody::create( bt::RigidBody::Format().collisionShape( heightField ) );
	
	// Collect it in my vector
	mRigidBodies.push_back( mHeightfieldTerrain );
	// And add that rigidbody to the world.
	mContext->addRigidBody( mHeightfieldTerrain );
	
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

void HeightfieldTerrainApp::keyDown( KeyEvent event )
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
	{
		gl::ScopedModelMatrix scope;
		// Transform it with the physics heightfield
		gl::multModelMatrix( translate( bt::fromBullet( mHeightfieldTerrain->getRigidBody()->getWorldTransform().getOrigin() ) ) );
		mBatch->draw();
	}
	// If we want to draw the debug we can set this and just toggle on and off using...
	mContext->debugDraw();
}


void prepareSettings( App::Settings *settings ) { settings->setMultiTouchEnabled( false ); }

CINDER_APP( HeightfieldTerrainApp, RendererGl, &prepareSettings )
