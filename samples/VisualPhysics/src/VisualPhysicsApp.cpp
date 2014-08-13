#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/TriMesh.h"

#include "VisPhy.h"
#include "Cinder-Bullet3D/BulletContext.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VisualPhysicsApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void prepareSettings( Settings * settings ) { settings->enableMultiTouch( false ); }
	void update();
	void draw();
	void createCube();
	void createSphere();
	void createCone();
	void createTeapot();
	void setupGround();
	
	bullet::ContextRef	mContext;
	std::vector<VisPhy> mVisualPhysicsObjs;
	gl::GlslProgRef		mPhongShader;
	CameraPersp			mCam;
	gl::BatchRef			mVisPlane;
	bullet::RigidBodyRef	mPhyPlane;
};

void VisualPhysicsApp::setup()
{
	// Create the shader that will represent all of the stuff
	mPhongShader = gl::GlslProg::create( gl::GlslProg::Format()
#if ! defined( CINDER_GL_ES )
										.vertex( loadAsset( "Phong.vert" ) )
										.fragment( loadAsset( "Phong.frag" ) )
#else
										.vertex( loadAsset( "Phong_ios.vert" ) )
										.fragment( loadAsset( "Phong_ios.frag" ) )
#endif
										);
	
	
	// Create a context. This Context stores all of the bullet world.
	// Take a look at the Context::Format to find out just what can be
	// controlled. Specifically here, we're creating a format that allows
	// debug Drawing
	mContext = bullet::Context::create( bullet::Context::Format().drawDebug( true ).createDebugRenderer( true ) );

	// Create each of the initial objects.
	createCube();
	createCone();
	createSphere();
	createTeapot();
	setupGround();
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	mCam.setPerspective( 60.0, getWindowAspectRatio(), .01f, 1000.0f );
	mCam.lookAt( Vec3f( 0, 5, 20 ), Vec3f::zero() );
}

void VisualPhysicsApp::createCube()
{
	using namespace bullet;
	auto scale = Vec3f( 1, 1, 1 );
	// We create a Cube batch. The cube's points are conveniently from 0, 0, 0 origin, with a scale of 1, 1, 1.
	// I'm enabling the normals because of the Phong shader.
	auto batch = gl::Batch::create( geom::Cube().enable( geom::Attrib::NORMAL ), mPhongShader );
	// All I need to do is create a Box Shape at a scale of 1, 1, 1.
	auto rigidBody = RigidBody::create( RigidBody::Format()
									   .collisionShape( createBoxShape( scale ) )
									   .mass( 1 )
									   .initialPosition( Vec3f( 0, 10, 0 ) )
									   .addToWorld( true ) );
	// Then put them together in the VisPhy object.
	mVisualPhysicsObjs.emplace_back( batch, rigidBody );
}

void VisualPhysicsApp::createSphere()
{
	using namespace bullet;
	auto radius = 1.0f;
	// We create a Sphere batch. The sphere's points are also conveniently from 0, 0, 0 origin, and I set the radius
	// to 1. I'm enabling normals because of the Phong shader.
	auto batch = gl::Batch::create( geom::Sphere().enable( geom::Attrib::NORMAL ).radius( radius ), mPhongShader );
	
	// All I need to do is create the sphere with radius 1.
	auto rigidBody = RigidBody::create( RigidBody::Format()
									   .collisionShape( createSphereShape( radius ) )
									   .mass( 1 )
									   .initialPosition( Vec3f( 0, 12, -.75 ) )
									   .addToWorld( true ) );
	
	mVisualPhysicsObjs.emplace_back( batch, rigidBody );
}

void VisualPhysicsApp::createCone()
{
	using namespace bullet;
	auto height = 1.0f;
	auto base = 1.0f;
	// This is a little different. The cone geometry's origin is at the base of the mesh. However, we can easily
	// shift the origin into the middle of the object, so that it'll be in line with bullet's origin.
	auto batch = gl::Batch::create( geom::Cone()
								   .enable( geom::Attrib::NORMAL )
								   .base( base )
								   .height( height )
								   .origin( Vec3f( 0, - (height / 2.0f), 0 ) ),
								   mPhongShader );
	
	// Now we just create the bullet Cone Shape with almost the exact same arguments.
	auto rigidBody = RigidBody::create( RigidBody::Format()
									   .collisionShape( createConeShape( base, height ) )
									   .mass( 1 )
									   .initialPosition( Vec3f( 1, 10, 1 ) )
									   .initialRotation( Quatf( Vec3f( 1, 1, 0 ), toRadians( 120.0f ) ) )
									   .addToWorld( true ) );
	
	mVisualPhysicsObjs.emplace_back( batch, rigidBody );
}

void VisualPhysicsApp::createTeapot()
{
	using namespace bullet;
	// Here's a cool example of more complicated geometry.
	auto teapot = geom::Teapot();
	
	// We create the teapot in any way we like.
	auto batch = gl::Batch::create( teapot.enable( geom::Attrib::NORMAL ), mPhongShader );
	
	// Then we just pass the geom::Source and create a trimesh and it'll create the convex hull from the points in
	// the trimesh. We also, cache the trimesh data because as long as we don't need to change the shape at all, we
	// can use this convexHullShape over and over.
	static auto teapotShape = createConvexHull( ci::TriMesh::create( teapot.disable( geom::Attrib::NORMAL ) ) );
	auto rigidBody = RigidBody::create( RigidBody::Format()
									   .collisionShape( teapotShape )
									   .mass( 1 )
									   .initialPosition( Vec3f( .9, 15, .9 ) )
									   .addToWorld( true ) );
	
	mVisualPhysicsObjs.emplace_back( batch, rigidBody );
}

void VisualPhysicsApp::setupGround()
{
	using namespace bullet;
	// Dealing with floors like this is a bit hard. Basically, the Static Plane Shape doesn't really have
	// the data we need. For instance, when I make this rect at a scale of 1000, 1000, it doesn't know about
	// Normal and Offset, the two arguments to make a plane shape.
	mVisPlane = gl::Batch::create( geom::Rect().scale( Vec2f( 1000, 1000 ) ), mPhongShader );
	
	// Therefore, we cache these in our program to draw seperately. Now I know that I want the plane's normal
	// to be the yaxis and the offset means what distance on the normal I'd like to shift this. If I put 1 insted
	// of zero I'd push the RigidBody to 0, 1, 0. Remember, that's not the normal, even though they're the same
	// value. Same as if the offset were 50 and the normal were 0, 1, 0, the position would be 0, 50, 0, not 0, 51, 0
	mPhyPlane = RigidBody::create( RigidBody::Format()
								  .collisionShape( createStaticPlaneShape( Vec3f::yAxis(), 0 ) )
								  .initialPosition( Vec3f( 0, 0, 0 ) )
								  .addToWorld( true ) );
}

void VisualPhysicsApp::mouseDown( MouseEvent event )
{
	auto random = rand() % 4;
	switch ( random ) {
		case 0:
			createCube();
		break;
		case 1:
			createSphere();
		break;
		case 2:
			createCone();
		break;
		case 3:
			createTeapot();
		break;
		default:
		break;
	}
}

void VisualPhysicsApp::keyDown( KeyEvent event )
{
	if( event.getChar() == KeyEvent::KEY_d )
		mContext->toggleDebugDraw();
}

void VisualPhysicsApp::update()
{
	
	mContext->update();
	// We'll update our Physics objects which will cash our Model Matrices.
	for( auto objIt = mVisualPhysicsObjs.begin(); objIt != mVisualPhysicsObjs.end(); ++objIt )
		objIt->update();
}

void VisualPhysicsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::setMatrices( mCam );
	
	// Now, we'll draw our plane first, by manipulating it's rotation. Since we know the plane's normal is up the yAxis
	// all we need to do is rotate the plane backwards 90 degrees along the x axis. That'll create our floor.
	gl::pushModelMatrix();
	gl::multModelMatrix( Matrix44f::createRotation( Vec3f( 1, 0, 0 ), toRadians( -90.0f ) ) );
		mVisPlane->draw();
	gl::popModelMatrix();
	
	// Then all we have to do is cycle through and draw the objects.
	gl::pushModelMatrix();
	for( auto objIt = mVisualPhysicsObjs.begin(); objIt != mVisualPhysicsObjs.end(); ++objIt )
		objIt->draw();
	gl::popModelMatrix();
	
	// This will draw our debug and show us whether our objects are where bullet thinks they are.
	mContext->debugDraw();
}

CINDER_APP_NATIVE( VisualPhysicsApp, RendererGl )
