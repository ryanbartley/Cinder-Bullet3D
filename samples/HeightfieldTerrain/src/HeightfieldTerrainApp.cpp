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
	
	bt::ContextRef		mContext;
	
	gl::GlslProgRef		mPhongShader;
	gl::BatchRef		mBatch;
	gl::VaoRef			mVao;
	gl::VboMeshRef		mDrawableHeightfield;
	bt::RigidBodyRef	mPhysicsHeightfield, box;
	Channel32f			mHeightfieldMap;
	CameraPersp			mCam;
};

void HeightfieldTerrainApp::setup()
{
	mContext = bt::Context::create( bt::Context::Format().drawDebug( true ).createDebugRenderer( true ) );
	
	setupShader();
	setupHeightfield();
	
	mContext->addRigidBody( mPhysicsHeightfield );
	box = bt::RigidBody::create( bt::RigidBody::Format().collisionShape( bt::createBoxShape( vec3( 1.0f ) ) ).initialPosition( vec3( 0, 4, 0 ) ).mass( 1.0f ) );
	mContext->addRigidBody( box );
	
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
	Perlin perlin( 5, 1 );
	mHeightfieldMap = Channel32f( Width, Depth );
	
	for( int y = 0; y < Depth; ++y ) {
		for( int x = 0; x < Width; ++x ) {
			mHeightfieldMap.setValue( ivec2( x, y ), randFloat( -1.0f, 1.0f ) );
		}
	}
	
	auto glsl = gl::GlslProg::create( gl::GlslProg::Format()
											.vertex( loadAsset( "basic.vert" ) )
											.fragment( loadAsset( "basic.frag" ) )
											.attrib( geom::POSITION, "position" )
											.attribLocation( geom::POSITION, 0 ) );
	
	mBatch = gl::Batch::create( bt::drawableHelpers::getDrawableHeightfield( &mHeightfieldMap ), mPhongShader );
	
	
	mPhysicsHeightfield = bt::RigidBody::create( bt::RigidBody::Format()
												.collisionShape( bt::createHeightfieldShape( &mHeightfieldMap, 1.0f, -1.0f, vec3( 1, 1, 1 ) ) ) );
	
}

void HeightfieldTerrainApp::mouseDown( MouseEvent event )
{
}

void HeightfieldTerrainApp::update()
{
	mContext->update();
}

void HeightfieldTerrainApp::draw()
{
	static float rotation = 0.0f;
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::setMatrices( mCam );
	gl::multModelMatrix( rotate( toRadians( rotation += 0.1 ), vec3( 0.0f, 1.0f, 0.0f ) ) );
	{
		gl::ScopedModelMatrix scope;
		// Transform it with the physics heightfield
		gl::multModelMatrix( translate( bt::fromBullet( mPhysicsHeightfield->getRigidBody()->getWorldTransform().getOrigin() ) ) );
		mBatch->draw();
	}
	mContext->debugDraw();
}


CINDER_APP_NATIVE( HeightfieldTerrainApp, RendererGl )
