#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "cinder/Rand.h"
#include "cinder/ObjLoader.h"

#include "Cinder-Bullet3D/BulletContext.h"

#include "Light.h"
#include "Body.h"

using namespace ci;
using namespace ci::app;
using namespace std;

int NUM_GLOWS = 2;

float getMassFromRadius( float r ){
	return ( ( r * r * r ) * (float)M_PI * 4.0f ) * 0.33333f;
}

class BulletSpheresApp : public AppNative {
public:
	void prepareSettings( Settings *settings ){ settings->setWindowSize( 1280, 720 ); };
	void resize();
	void setup();
	void keyDown( KeyEvent event );
	void updateTime();
	void update();
	void draw();
	void drawToDepthFbo();
	void createSphere( int index, const vec3 &pos, float radius, gl::GlslProgRef shader );
	void createBigSphere( int index, const vec3 &pos, float radius, gl::GlslProgRef shader );
	
	// time
	float					mTime;
	float					mTimeElapsed;
	float					mTimeDelta;
	
	// camera
	CameraPersp				mCam;
	
	// shaders
	gl::GlslProgRef			mShadowsGlsl;	// shader for the smaller spheres (includes cast shadows)
	gl::GlslProgRef			mGlowGlsl;		// shader for the larger spheres
	
	// textures
	gl::TextureRef			mStripeTex;		// stripe so we can see the smaller spheres rotate
	
	// light source
	LightRef				mLight;			// light source for casting shadows
	
	// bullet
	bt::ContextRef			mContext;		// bullet context
	vector<Body>			mRigidBodies;	// container for the rigid bodies
	
	// glow colors
	vector<Color>			mGlowColors;	// colors for the larger spheres
};

void BulletSpheresApp::setup()
{
	// time
	mTime				= app::getElapsedSeconds();
	mTimeElapsed		= 0.0f;
	mTimeDelta			= 0.0f;
	
	// camera
	mCam = CameraPersp();
	mCam.lookAt( vec3( 0, 0, 100 ), vec3( 0 ) );
	
	// shaders
	try{
		mShadowsGlsl	= gl::GlslProg::create( loadAsset( "shadows.vert" ), loadAsset( "shadows.frag" ) );
		mGlowGlsl		= gl::GlslProg::create( loadAsset( "glow.vert" ), loadAsset( "glow.frag" ) );
	} catch( gl::GlslProgCompileExc e ){
		std::cout << e.what() << std::endl;
		quit();
	}
	
	// textures
	mStripeTex			= gl::Texture::create( loadImage( loadAsset( "stripe.png" ) ) );
	
	// bullet
	mContext = bullet::Context::create( bullet::Context::Format().drawDebug( false ).createDebugRenderer( true ) );
	createBigSphere( 0, vec3(  40.0f, 0, 0 ), 15.0f, mGlowGlsl );
	createBigSphere( 1, vec3( -40.0f, 0, 0 ), 15.0f, mGlowGlsl );
	for( int i=0; i<500; i++ ){
		vec3 randPos		= Rand::randVec3f() * randFloat( 40.0f, 60.0f );
		float randRadius	= randFloat( 2.5f, 3.0f );
		if( randFloat() < 0.03f ) randRadius = randFloat( 5.0f, 8.0f );
		createSphere( i + 2, randPos, randRadius, mShadowsGlsl );
	}
	
	// light source
	auto shadowMapSize		= 2048;
	mLight					= Light::create( vec4( 100.0f, 150.0f, 70.0f, 1.0f ), shadowMapSize );
	
	// glow colors
	mGlowColors.push_back( Color( 1, 0, 0 ) );
	mGlowColors.push_back( Color( 0, 0, 1 ) );
	
	// set gl state
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void BulletSpheresApp::resize()
{
	mCam.setPerspective( 70.0f, getWindowAspectRatio(), 1.0f, 1000.0f );
}

void BulletSpheresApp::keyDown( KeyEvent event )
{
	switch( event.getChar() ){
		case 'f': setFullScreen( ! isFullScreen() );	break;
		case 'd': mContext->toggleDebugDraw();			break;
		default : break;
	}
}

void BulletSpheresApp::createSphere( int index, const vec3 &pos, float radius, gl::GlslProgRef shader )
{
	auto batch		= gl::Batch::create( geom::Sphere().enable( geom::Attrib::NORMAL ).radius( radius ).segments( 16 ), shader );
	auto rigidBody	= bt::RigidBody::create( bt::RigidBody::Format()
											.collisionShape( bt::createSphereShape( radius ) )
											.mass( getMassFromRadius( radius ) + 10.0f )
											.initialPosition( pos )
											.addToWorld( true ) );
	mRigidBodies.emplace_back( index, radius, batch, rigidBody );
}

void BulletSpheresApp::createBigSphere( int index, const vec3 &pos, float radius, gl::GlslProgRef shader )
{
	auto batch		= gl::Batch::create( geom::Sphere().enable( geom::Attrib::NORMAL ).radius( radius ).segments( 64 ), shader );
	auto rigidBody	= bt::RigidBody::create( bt::RigidBody::Format()
											.collisionShape( bt::createSphereShape( radius ) )
											.initialPosition( pos )
											.addToWorld( true ) );
	mRigidBodies.emplace_back( index, radius, batch, rigidBody );
}

void BulletSpheresApp::updateTime()
{
	float prevTime	= mTime;
	mTime			= (float)app::getElapsedSeconds();
	mTimeDelta		= mTime - prevTime;
	if( mTimeDelta > 1.0f/30.0f )
		mTimeDelta = 1.0f/30.0f;
	mTimeElapsed += mTimeDelta;
}

void BulletSpheresApp::update()
{
	// update time
	updateTime();
	
	// update bullet context
	mContext->setStepVal( mTimeDelta );
	mContext->setGravity( vec3( 0 ) );
	mContext->update();
	
	// update rigid bodies
	for( auto &body : mRigidBodies ){
		body.update( mTimeElapsed );
	}
	
	// add attractive force
	for( auto &body : mRigidBodies ){
		int index = body.getId();
		if( index >=2 ){		// small orbs
			for( int i=0; i<2; i++ ){
				vec3 pos		= mRigidBodies[i].getPos();
				float size		= mRigidBodies[i].getSize();
				float strength	= mRigidBodies[i].getStrength();
				body.attract( pos, size, strength );
			}
		} else {				// big orbs
			float size			= mRigidBodies[index].getSize();
			body.attract( vec3( 0 ), size, 1.0f );
		}
	}
	
	// update light (unnecessary since light is stationary)
	mLight->update( mTimeDelta );
}

void BulletSpheresApp::drawToDepthFbo()
{
	// draw into the depth fbo for shadow casting
	gl::enable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	
	gl::setMatrices( mLight->getCam() );
	gl::viewport( mLight->getFbo()->getSize() );
	
	gl::ScopedFramebuffer bindFbo( mLight->getFbo() );
	gl::clear();
	for( auto &body : mRigidBodies ){
		body.draw();
	}
	
	gl::disable( GL_CULL_FACE );
}

void BulletSpheresApp::draw()
{
	drawToDepthFbo();
	
	gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
	gl::setMatrices( mCam );
	gl::viewport( vec2( 0 ), getWindowSize() );
	
	// Draw small spheres
	mLight->getFbo()->getDepthTexture()->bind( 0 );
	mStripeTex->bind( 1 );
	mShadowsGlsl->bind();
	mShadowsGlsl->uniform( "uShadowMap",	0 );
	mShadowsGlsl->uniform( "uStripeTex",	1 );
	mShadowsGlsl->uniform( "uLightPos",		mLight->getPos() );
	mShadowsGlsl->uniform( "uDepthBias",	mLight->getDepthBias() );
	mShadowsGlsl->uniform( "uLightPos1",	mRigidBodies[0].getPos() );
	mShadowsGlsl->uniform( "uLightPos2",	mRigidBodies[1].getPos() );
	mShadowsGlsl->uniform( "uRadius1",		mRigidBodies[0].getSize() );
	mShadowsGlsl->uniform( "uRadius2",		mRigidBodies[1].getSize() );
	for( auto &body : mRigidBodies ){
		auto index = body.getId();
		if( index >= NUM_GLOWS ){
			mShadowsGlsl->uniform( "uShadowMvp", mLight->getViewProjection() );
			body.draw();
		}
	}
	
	// Draw big spheres
	mGlowGlsl->bind();
	mGlowGlsl->uniform( "uLightPos", mLight->getPos() );
	for( auto &body : mRigidBodies ){
		auto index = body.getId();
		if( index < NUM_GLOWS ){
			mGlowGlsl->uniform( "uColor", mGlowColors[index] );
			body.draw();
		}
	}
	
	// Draw light source (if point light)
	if( mLight->getPos().w > 0.5f )
		mLight->draw();
	
	mContext->debugDraw();
	
	if( getElapsedFrames()%60 == 0 ) std::cout << getAverageFps() << std::endl;
}

CINDER_APP_NATIVE( BulletSpheresApp, RendererGl )
