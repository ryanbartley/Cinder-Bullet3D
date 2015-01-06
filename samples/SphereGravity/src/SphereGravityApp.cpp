#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "cinder/Rand.h"
#include "cinder/gl/VboMesh.h"

#include "Cinder-Bullet3D/BulletContext.h"

#include "Light.h"
#include "Body.h"

using namespace ci;
using namespace ci::app;
using namespace std;

int NUM_GLOWS = 2;

const int NUM_PARTICLES = 1000;

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
	void createPhysics();
	void createBuffers();
	void createGl();
	void createSphere( int index, const vec3 &pos, float radius );
	void createBigSphere( int index, const vec3 &pos, float radius, gl::GlslProgRef shader );
	
	// time
	float					mTime;
	float					mTimeElapsed;
	float					mTimeDelta;
	
	// camera
	CameraPersp				mCam;
	
	// shaders
	gl::GlslProgRef			mShadowsGlsl;	// shader for the smaller spheres (includes cast shadows)
	gl::GlslProgRef			mTestGlsl;
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
	gl::VboRef				mMatrices, mPositions, mNormals, mTexCoords, mElements;
	gl::VaoRef				mVao;
	TriMeshRef				mTrimesh;
};

void BulletSpheresApp::setup()
{
	// time
	mTime				= app::getElapsedSeconds();
	mTimeElapsed		= 0.0f;
	mTimeDelta			= 0.0f;
	
	mCam = CameraPersp();
	mCam.lookAt( vec3( 0, 0, 100 ), vec3( 0 ) );
	
	try{
		mShadowsGlsl	= gl::GlslProg::create( gl::GlslProg::Format()
											   .vertex( loadAsset( "render.vert" ) )
											   .fragment( loadAsset( "render.frag" ) ) );
		mTestGlsl = gl::GlslProg::create( gl::GlslProg::Format().vertex( loadAsset( "depth.vert" ) ).fragment( loadAsset( "depth.frag" ) )
										 .attribLocation( geom::POSITION, 0 )
										 .attribLocation( geom::NORMAL, 1 )
										 .attribLocation( geom::TEX_COORD_0, 2 )
										 .attrib( geom::CUSTOM_0, "model_view" )
										 .attribLocation( geom::CUSTOM_0, 3 )
										 .attribLocation( geom::CUSTOM_1, 4 )
										 .attribLocation( geom::CUSTOM_2, 5 )
										 .attribLocation( geom::CUSTOM_3, 6 ) );
		auto attrType = mShadowsGlsl->getActiveAttribTypes();
		for( auto attrIt = attrType.begin(); attrIt != attrType.end(); ++attrIt ) {
			cout << attrIt->first << " " << attrIt->second << endl;
		}
		mGlowGlsl		= gl::GlslProg::create( loadAsset( "glow.vert" ), loadAsset( "glow.frag" ) );
	} catch( gl::GlslProgCompileExc e ){
		std::cout << e.what() << std::endl;
		quit();
	}
	
	createPhysics();
	
	createGl();
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

void BulletSpheresApp::createGl()
{
	// textures
	mStripeTex			= gl::Texture::create( loadImage( loadAsset( "stripe.png" ) ) );
	
	// light source
	auto shadowMapSize		= 1024;
	mLight					= Light::create( vec4( 100.0f, 150.0f, 70.0f, 1.0f ), shadowMapSize );
	
	mGlowColors.push_back( Color( 1, 0, 0 ) );
	mGlowColors.push_back( Color( 0, 0, 1 ) );
	
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	createBuffers();
}

void BulletSpheresApp::createBuffers()
{
	mTrimesh = TriMesh::create( geom::Sphere().subdivisions( 16 ) );
	mPositions = gl::Vbo::create( GL_ARRAY_BUFFER, mTrimesh->getNumVertices() * sizeof(vec3), mTrimesh->getPositions<3>(), GL_STATIC_DRAW );
	mNormals = gl::Vbo::create( GL_ARRAY_BUFFER, mTrimesh->getNumVertices() * sizeof(vec3), mTrimesh->getNormals().data(), GL_STATIC_DRAW );
	mTexCoords = gl::Vbo::create( GL_ARRAY_BUFFER, mTrimesh->getNumVertices() * sizeof(vec2), mTrimesh->getTexCoords0<2>(), GL_STATIC_DRAW );
	mElements = gl::Vbo::create( GL_ELEMENT_ARRAY_BUFFER, mTrimesh->getNumIndices() * sizeof(uint32_t), mTrimesh->getIndices().data(), GL_STATIC_DRAW );
	
	mMatrices = gl::Vbo::create( GL_ARRAY_BUFFER, (NUM_PARTICLES-2)*sizeof(mat4) );
	
	mVao = gl::Vao::create();
	{
		gl::ScopedVao scopeVao( mVao );
		{
			
			gl::ScopedBuffer scopeBuffer( mPositions );
			gl::enableVertexAttribArray( 0 );
			gl::vertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		}
		{
			gl::ScopedBuffer scopeBuffer( mNormals );
			gl::enableVertexAttribArray( 1 );
			gl::vertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		}
		{
			gl::ScopedBuffer scopeBuffer( mTexCoords );
			gl::enableVertexAttribArray( 2 );
			gl::vertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		}
		{
			gl::ScopedBuffer scopeBuffer( mMatrices );
			gl::enableVertexAttribArray( 3 );
			gl::vertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*)0 );
			glVertexAttribDivisor( 3, 1 );
			
			gl::enableVertexAttribArray( 4 );
			gl::vertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*) (sizeof(vec4) ) );
			glVertexAttribDivisor( 4, 1 );
			
			gl::enableVertexAttribArray( 5 );
			gl::vertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*) (sizeof(vec4) * 2) );
			glVertexAttribDivisor( 5, 1 );
			
			gl::enableVertexAttribArray( 6 );
			gl::vertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (GLvoid*) (sizeof(vec4) * 3 ) );
			glVertexAttribDivisor( 6, 1 );
		}
	}

}

void BulletSpheresApp::createPhysics()
{
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	// bullet
	mContext = bullet::Context::create( bullet::Context::Format().drawDebug( false ).createDebugRenderer( true ).broadphase( new btAxisSweep3 (worldAabbMin, worldAabbMax) ) );
	
	createBigSphere( 0, vec3(  40.0f, 0, 0 ), 15.0f, mGlowGlsl );
	createBigSphere( 1, vec3( -40.0f, 0, 0 ), 15.0f, mGlowGlsl );
	
	for( int i=0; i< NUM_PARTICLES; i++ ){
		vec3 randPos		= Rand::randVec3f() * randFloat( 40.0f, 60.0f );
		float randRadius	= randFloat( 0.5, 3.0f );
		if( randFloat() < 0.03f ) randRadius = randFloat( 5.0f, 8.0f );
		createSphere( i + 2, randPos, randRadius );
	}
}

void BulletSpheresApp::createSphere( int index, const vec3 &pos, float radius )
{
	static auto shape = bt::createSphereShape( 1.0f );
	auto rigidBody	= bt::RigidBody::create( bt::RigidBody::Format()
											.collisionShape( bt::createUniformScalingShape( shape, radius ) )
											.mass( getMassFromRadius( radius ) + 10.0f )
											.initialPosition( pos )
											.addToWorld( true ).restitution( 1.0f ) );
	mRigidBodies.emplace_back( index, radius, rigidBody );
}

void BulletSpheresApp::createBigSphere( int index, const vec3 &pos, float radius, gl::GlslProgRef shader )
{
	auto batch		= gl::Batch::create( geom::Sphere().radius( radius ).subdivisions( 64 ), shader );
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
	// update bullet context
	updateTime(); 
	
	mContext->setGravity( vec3( 0 ) );
	mContext->update();
	
	// add attractive force
	mat4 * matrices = (mat4*)mMatrices->map( GL_WRITE_ONLY );
	for( auto &body : mRigidBodies ){
		body.update( mTimeElapsed );
		int index = body.getId();
		if( index >=2 ){		// small orbs
			for( int i=0; i<2; i++ ){
				vec3 pos		= mRigidBodies[i].getPos();
				float size		= mRigidBodies[i].getSize();
				float strength	= mRigidBodies[i].getStrength();
				body.attract( pos, size, strength );
			}
			matrices[index-2] = body.getMatrix();
		} else {				// big orbs
			float size			= mRigidBodies[index].getSize();
			body.attract( vec3( 0 ), size, 1.0f );
		}
	}
	mMatrices->unmap();
}



void BulletSpheresApp::drawToDepthFbo()
{
	// draw into the depth fbo for shadow casting
	gl::ScopedFramebuffer bindFbo( mLight->getFbo() );
	gl::clear();

	gl::ScopedState scopeCull( GL_CULL_FACE, true );
	glCullFace( GL_FRONT );
	
	gl::ScopedMatrices scopeMat;
	gl::setMatrices( mLight->getCam() );
	gl::viewport( mLight->getFbo()->getSize() );
	
	gl::ScopedVao scopeVao( mVao );
	gl::ScopedGlslProg scopeGlsl( mTestGlsl );
	gl::ScopedBuffer scopeIndex( mElements );
	
	gl::setDefaultShaderVars();
	
	glDrawElementsInstanced( GL_TRIANGLES, mTrimesh->getNumIndices(), GL_UNSIGNED_INT, 0, NUM_PARTICLES-2 );
}

void BulletSpheresApp::draw()
{
	drawToDepthFbo();
	
	gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
	gl::setMatrices( mCam );
	gl::viewport( vec2( 0 ), getWindowSize() );
	
	// Draw small spheres
	{
		gl::ScopedVao scopeVao( mVao );
		gl::ScopedGlslProg scopeGlsl( mShadowsGlsl );
		gl::ScopedBuffer scopeIndex( mElements );
		gl::ScopedTextureBind scopeTex0( mLight->getFbo()->getDepthTexture(), 0 );
		gl::ScopedTextureBind scopeTex1( mStripeTex, 1 );
		
		mShadowsGlsl->uniform( "uShadowMap",	0 );
		mShadowsGlsl->uniform( "uStripeTex",	1 );
		mShadowsGlsl->uniform( "uLightPos",		mLight->getPos() );
		mShadowsGlsl->uniform( "uDepthBias",	mLight->getDepthBias() );
		mShadowsGlsl->uniform( "uLightPos1",	mRigidBodies[0].getPos() );
		mShadowsGlsl->uniform( "uLightPos2",	mRigidBodies[1].getPos() );
		mShadowsGlsl->uniform( "uRadius1",		mRigidBodies[0].getSize() );
		mShadowsGlsl->uniform( "uRadius2",		mRigidBodies[1].getSize() );
		mShadowsGlsl->uniform( "uShadowMvp",	mLight->getViewProjection() );
		
		gl::setDefaultShaderVars();
	
		glDrawElementsInstanced( GL_TRIANGLES, mTrimesh->getNumIndices(), GL_UNSIGNED_INT, 0, NUM_PARTICLES-2 );
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
}

CINDER_APP_NATIVE( BulletSpheresApp, RendererGl )
