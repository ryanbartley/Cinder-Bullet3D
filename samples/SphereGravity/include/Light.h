#pragma once

#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

typedef std::shared_ptr<class Light> LightRef;

class Light
{
public:
	static LightRef create( const ci::vec4 &pos, size_t shadowMapSize ) {
		return LightRef( new Light{ pos, shadowMapSize } ); }
	Light( const ci::vec4 &pos, size_t shadowMapSize );
	void update( float dt );
	void draw();
	void prepareDraw();
	void finishDraw();
	void toggleW(){ mPos.w = ( 1.0f - mPos.w ); };
	
	ci::vec4			getPos(){				return mPos; };
	ci::gl::FboRef		getFbo(){				return mShadowMapFbo; };
	ci::gl::TextureRef	getTexture(){			return mShadowMapTex; };
	
	ci::CameraPersp		getCam(){				return mCam; };
	ci::gl::TextureRef	getMask(){				return mSpotMask; };
	float				getDepthBias(){			return mDepthBias; };
	ci::mat4			getViewProjection();
	
private:
	ci::vec4			mPos;
	ci::gl::BatchRef	mBatch;
	
	ci::CameraPersp		mCam;
	ci::gl::FboRef		mShadowMapFbo;
	ci::gl::TextureRef	mShadowMapTex;
	ci::gl::TextureRef	mSpotMask;
	
	float				mDepthBias;
};