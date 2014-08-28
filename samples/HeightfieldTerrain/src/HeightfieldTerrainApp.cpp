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

const uint32_t Width = 10;
const uint32_t Depth = 10;

struct DefaultDrawableGuts {
	ci::gl::GlslProgRef mGlsl;
	ci::gl::VaoRef		mVao;
	ci::gl::VboRef		mPositionVbo, mNormalVbo, mTexCoordVbo, mElementVbo;
	int					mNumIndices;
	GLenum				mPrimitiveType;
};

using DefaultDrawableGutsRef = std::shared_ptr<DefaultDrawableGuts>;

DefaultDrawableGutsRef getDrawableHeightField( const Channel32f *heightData )
{
	DefaultDrawableGutsRef sHeightFieldDrawableGuts = DefaultDrawableGutsRef( new DefaultDrawableGuts );
	std::vector<vec3>		mPositions;
	std::vector<vec3>		mNormals;
	std::vector<uint32_t>	mIndices;
	std::vector<vec2>		mTexCoords;
	
	
	int32_t height	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	for ( int32_t y = 0; y < height; y++ ) {
		for ( int32_t x = 0; x < width; x++ ) {
			mTexCoords.push_back( vec2( (float)x / (float)width, (float)y / (float)height ) );
			
			int32_t xn = x + 1 >= width ? 0 : 1;
			int32_t yn = y + 1 >= height ? 0 : 1;
			mIndices.push_back( x + height * y );
			mIndices.push_back( ( x + xn ) + height * y);
			mIndices.push_back( ( x + xn ) + height * ( y + yn ) );
			mIndices.push_back( x + height * ( y + yn ) );
			mIndices.push_back( ( x + xn ) + height * ( y + yn ) );
			mIndices.push_back( x + height * y );
		}
	}
	
	mNormals.clear();
	mPositions.clear();
	
	float halfHeight	= (float)height * 0.5f - 0.5f;
	float halfWidth		= (float)width * 0.5f - 0.5f;
	
	for ( int32_t y = 0; y < height; y++ ) {
		for ( int32_t x = 0; x < width; x++ ) {
			float value = heightData->getValue( ivec2( x, y ) );
			
			vec3 position( (float)x - halfWidth, value, (float)y - halfHeight );
			mPositions.push_back( position );
			
			mNormals.push_back( vec3( 0.0f ) );
		}
	}
	
	for ( int32_t y = 0; y < height - 1; y++ ) {
		for ( int32_t x = 0; x < width - 1; x++ ) {
			vec3 vert0 = mPositions[ mIndices[ ( x + height * y ) * 6 ] ];
			vec3 vert1 = mPositions[ mIndices[ ( ( x + 1 ) + height * y ) * 6 ] ];
			vec3 vert2 = mPositions[ mIndices[ ( ( x + 1 ) + height * ( y + 1 ) ) * 6 ] ];
			mNormals[ x + height * y ] = vec3( normalize( cross( ( vert1 - vert0 ), ( vert1 - vert2 ) ) ) );
		}
	}
	
	sHeightFieldDrawableGuts->mVao = gl::Vao::create();
	sHeightFieldDrawableGuts->mPositionVbo = gl::Vbo::create( GL_ARRAY_BUFFER, mPositions );
	sHeightFieldDrawableGuts->mNormalVbo = gl::Vbo::create( GL_ARRAY_BUFFER, mNormals );
	sHeightFieldDrawableGuts->mTexCoordVbo = gl::Vbo::create( GL_ARRAY_BUFFER, mTexCoords );
	sHeightFieldDrawableGuts->mElementVbo = gl::Vbo::create( GL_ELEMENT_ARRAY_BUFFER, mIndices );
	
	
	gl::ScopedVao mVaoScope( sHeightFieldDrawableGuts->mVao );
	gl::ScopedGlslProg mGlsl( sHeightFieldDrawableGuts->mGlsl );
	
	{
		gl::ScopedBuffer mBufferPosScope( sHeightFieldDrawableGuts->mPositionVbo );
		gl::enableVertexAttribArray( 0 );
		gl::vertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}
	{
		gl::ScopedBuffer mBufferNormalScope( sHeightFieldDrawableGuts->mNormalVbo );
		gl::enableVertexAttribArray( 1 );
		gl::vertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}
	{
		gl::ScopedBuffer mBufferTexCoordScope( sHeightFieldDrawableGuts->mTexCoordVbo );
		gl::enableVertexAttribArray( 2 );
		gl::vertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	}
	
	sHeightFieldDrawableGuts->mNumIndices = mIndices.size();
	sHeightFieldDrawableGuts->mPrimitiveType = gl::toGl( geom::Primitive::TRIANGLES );
	
	return sHeightFieldDrawableGuts;
}

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
	bt::RigidBodyRef	mPhysicsHeightfield;
	Channel32f			mHeightfieldMap;
	CameraPersp			mCam;
	DefaultDrawableGutsRef			mDrawable;
};

void HeightfieldTerrainApp::setup()
{
	mContext = bt::Context::create( bt::Context::Format().drawDebug( true ).createDebugRenderer( true ) );
	
	setupShader();
	setupHeightfield();
	
	mDrawable->mGlsl = gl::GlslProg::create( gl::GlslProg::Format()
											.vertex( loadAsset( "basic.vert" ) )
											.fragment( loadAsset( "basic.frag" ) )
											.attrib( geom::POSITION, "position" )
											.attribLocation( geom::POSITION, 0 ) );
	
	mBatch = gl::Batch::create( mDrawableHeightfield, mDrawable->mGlsl );
	cout << mBatch->getPrimitive() << endl;
	
	mContext->addRigidBody( mPhysicsHeightfield );
	
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
	mCam.lookAt( vec3( 0, 5, 10 ), vec3( 0.0f ) );
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
	Perlin perlin;
	mHeightfieldMap = Channel32f( Width, Depth );
	for( int y = 0; y < Depth; ++y ) {
		for( int x = 0; x < Width; ++x ) {
			float height = ci::randFloat(-1, 1);
			mHeightfieldMap.setValue( ivec2( x, y ), height );
		}
	}
	
	mPhysicsHeightfield = bt::RigidBody::create( bt::RigidBody::Format()
												.collisionShape( bt::createHeightfieldShape( &mHeightfieldMap, 1.0f, -1.0f, vec3( 1, 1, 1 ) ) ) );
	auto heightField = mPhysicsHeightfield->getCollisionShape();
	mDrawable = getDrawableHeightField( &mHeightfieldMap );
	mDrawableHeightfield = bt::drawableHelpers::getDrawableHeightfield( &mHeightfieldMap );
	
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
	
//	gl::ScopedVao scopeVao( mBatch->getVao() );
//	gl::ScopedBuffer scopeVbo( mDrawableHeightfield->getIndexVbo() );
//	gl::ScopedGlslProg scopeGlsl( mDrawable->mGlsl );
//
//	gl::setDefaultShaderVars();
//	gl::drawElements( GL_TRIANGLE_STRIP, mBatch->getNumIndices(), GL_UNSIGNED_INT, 0 );
	mBatch->draw();
//	gl::draw( mDrawableHeightfield );
	mContext->debugDraw();
}


CINDER_APP_NATIVE( HeightfieldTerrainApp, RendererGl )
