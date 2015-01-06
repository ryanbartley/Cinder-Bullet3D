//
//  Light.cpp
//  ShadowMap
//
//  Created by Robert Hodgin on 7/31/14.
//
//

#include "cinder/gl/Shader.h"
#include "cinder/gl/Context.h"
#include "Light.h"

using namespace ci;
using namespace std;

Light::Light( const vec4 &pos, size_t shadowMapSize )
: mPos( pos )
{
	mCam = CameraPersp();
	mCam.setPerspective( 70.0f, 1.0f, 10.0f, 500.0f );
	
	mBatch			= gl::Batch::create( geom::Sphere().subdivisions( 20 ).radius( 2.0f ), getStockShader( gl::ShaderDef().color() ) );
	
	// From Eríc Renault Houde's ShadowMapping sample
	gl::Texture2d::Format depthFormat;
	depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
	depthFormat.setMagFilter( GL_LINEAR );
	depthFormat.setMinFilter( GL_LINEAR );
	depthFormat.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
	depthFormat.setDataType( GL_FLOAT );
	depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
	depthFormat.setCompareFunc( GL_LEQUAL );
	mShadowMapTex = gl::Texture2d::create( shadowMapSize, shadowMapSize, depthFormat );
	
	gl::Fbo::Format fboFormat;
	fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowMapTex );
	mShadowMapFbo		= gl::Fbo::create( shadowMapSize, shadowMapSize, fboFormat );
	
	mSpotMask			= gl::Texture::create( loadImage( app::loadAsset( "spotlightMask.png" ) ) );
	
	mDepthBias			= -0.0005f;
}

ci::mat4 Light::getViewProjection()
{
	return mCam.getProjectionMatrix() * mCam.getViewMatrix();
}

void Light::update( float dt )
{
	mCam.lookAt( vec3( mPos ), vec3( 0 ) );
}

void Light::draw()
{
	gl::pushModelMatrix();
	gl::translate( vec3( mPos ) );
	mBatch->draw();
	gl::popModelMatrix();
}

void Light::prepareDraw()
{
	gl::context()->pushFramebuffer( mShadowMapFbo );
	gl::clear();
	gl::setMatrices( mCam );
	gl::viewport( vec2( 0 ), mShadowMapFbo->getSize() );
}

void Light::finishDraw()
{
	gl::context()->popFramebuffer();
}
