//
//  PhyisicsDebugRenderable.cpp
//  BulletChapter1
//
//  Created by Ryan Bartley on 1/24/14.
//
//

#include "Cinder-Bullet3D/PhysicsDebugRenderable.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/TextureFont.h"

using namespace cinder;

namespace bullet {
	
PhysicsDebugRenderable::PhysicsDebugRenderable()
: mNewInfo( false ), mDebugMode( 0 ), mTextureFont( gl::TextureFont::create( Font( Font::getNames()[0], 20 ) ) )
{
}
	
void PhysicsDebugRenderable::drawLine( const btVector3& from,const btVector3& to,const btVector3& color )
{
	mPositionBuffer.push_back( fromBullet( from ) );
	mPositionBuffer.push_back( fromBullet( color ) );

	mPositionBuffer.push_back( fromBullet( to ) );
	mPositionBuffer.push_back( fromBullet( color ) );
	mNewInfo = true;
}
	
void PhysicsDebugRenderable::drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
	// draws a line between two contact points
	btVector3 const startPoint = pointOnB;
	btVector3 const endPoint = pointOnB + normalOnB * distance;
	drawLine( startPoint, endPoint, color );
}
	
void PhysicsDebugRenderable::toggleDebugFlag( const int flag ) {
	// checks if a flag is set and enables/
	// disables it
	if (mDebugMode & flag)
		// flag is enabled, so disable it
		mDebugMode = mDebugMode & (~flag);
	else
		// flag is disabled, so enable it
		mDebugMode |= flag;
}
	
void PhysicsDebugRenderable::initBuffers()
{
	if( !mNewInfo )
		return;
	
	if( !mPositionVbo )
		mPositionVbo = gl::Vbo::create( GL_ARRAY_BUFFER );
	
	mPositionVbo->bufferData( mPositionBuffer.size() * sizeof(vec3), mPositionBuffer.data(), GL_DYNAMIC_DRAW );
	
	if( mVao ) return;
	
	mVao = gl::Vao::create();
	
	gl::ScopedVao mVaoScope( mVao );
	gl::ScopedBuffer mVboScope( mPositionVbo );
	
	gl::enableVertexAttribArray(0);
	gl::vertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), 0 );
	
	gl::enableVertexAttribArray(1);
	gl::vertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)sizeof(vec3) );
}
	
void PhysicsDebugRenderable::draw3dText( const btVector3 &location, const char *textString )
{
	mText.push_back( std::make_pair( fromBullet( location ), textString ) );
}
	
void PhysicsDebugRenderable::draw()
{
	initBuffers();
	
	gl::ScopedVao mVaoScope( mVao );
	gl::ScopedGlslProg mGlslScope( gl::getStockShader( gl::ShaderDef().color() ) );
	
	gl::setDefaultShaderVars();
	
	gl::drawArrays( GL_LINES, 0, mPositionBuffer.size() / 2 );
	
	if( mText.size() > 0 ) {
		gl::pushMatrices();
		auto textIt = mText.begin();
		auto end = mText.end();
		while( textIt != end ) {
			gl::pushModelMatrix();
			gl::translate( (*textIt).first );
			mTextureFont->drawString( (*textIt).second, vec2( 0.0f ) );
			gl::popModelMatrix();
			++textIt;
		}
		gl::popMatrices();
	}
	
	mNewInfo = false;
	mPositionBuffer.clear();
	mText.clear();
}
	
	
}