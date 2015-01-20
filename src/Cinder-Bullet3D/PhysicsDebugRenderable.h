//
//  PhyisicsDebugRenderable.h
//  BulletChapter1
//
//  Created by Ryan Bartley on 1/24/14.
//
//

#pragma once

#include "cinder/gl/Vao.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/TextureFont.h"
#include "Cinder-Bullet3D/Common.h"

namespace bullet {
	
typedef std::shared_ptr<class PhysicsDebugRenderable> DebugRendererRef;

class PhysicsDebugRenderable : public btIDebugDraw {
public:
	~PhysicsDebugRenderable() {}
	
	//! This will be called by BulletContext. It will push the buffered colors and positions
	//!	to the GPU and draw these using the already set Camera Matrices.
	void draw();
	
	//! This is used by Bullet and shouldn't necessarily be used by your application.
	void drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	//! This is used by Bullet and shouldn't necessarily be used by your application.
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	//! This is used by Bullet and shouldn't necessarily be used by your application.
	void reportErrorWarning(const char* warningString) {}
	//! This is used by Bullet and shouldn't necessarily be used by your application.
	void draw3dText( const btVector3& location, const char* textString );
	
	//! Sets what to draw in the Debug Renderer
	void setDebugMode( int debugMode ) override { mDebugMode = debugMode; }
	//! Returns the Current Debug Renderer Mode.
	int	 getDebugMode() const { return mDebugMode; }
	//! This will simply toggle a specific debug flag.
	void toggleDebugFlag( const int flag );
	
private:
	PhysicsDebugRenderable();
	
	//! This pushes data to the gpu and initializes all of the gl. Must be called after
	//! Cinder has initialized.
	void initBuffers();
	
	ci::gl::VaoRef			mVao;
	ci::gl::VboRef			mPositionVbo;
	ci::gl::TextureFontRef	mTextureFont;
	std::vector<ci::vec3>	mPositionBuffer;
	std::vector<std::pair<ci::vec3, std::string>> mText;
	int			mDebugMode;
	bool		mNewInfo;
	
	friend class Context;
};
	
}
