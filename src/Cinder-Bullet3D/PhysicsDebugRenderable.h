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
#include "Physics/Common.h"

namespace bullet {
	
typedef std::shared_ptr<class PhysicsDebugRenderable> DebugRendererRef;

class PhysicsDebugRenderable : public btIDebugDraw {
public:
	
	~PhysicsDebugRenderable() {}
	
	void initBuffers();
	
	void update();
	void draw();
	
	void drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	void reportErrorWarning(const char* warningString) {}
	
	void draw3dText( const btVector3& location, const char* textString );
	
	void setDebugMode( const int debugMode ) { mDebugMode = debugMode; }
	int	 getDebugMode() const { return mDebugMode; }
	void toggleDebugFlag( const int flag );
	
private:
	
	PhysicsDebugRenderable();
	
	ci::gl::VaoRef			mVao;
	ci::gl::VboRef			mPositionVbo;
	ci::gl::TextureFontRef	mTextureFont;
	std::vector<ci::Vec3f>	mPositionBuffer;
	std::vector<std::pair<ci::Vec3f, std::string>> mText;
	int			mDebugMode;
	bool		mNewInfo;
	
	friend class Context;
};
	
}
