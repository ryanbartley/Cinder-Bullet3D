#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/Shader.h"
#include "cinder/gl/Batch.h"

#include "PhyObjPrimitive.h"
#include "BulletContext.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PhysicsPrimitivesApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	// These two functions signal the beginning of a collision
	// and the end of a collision. If a collision has begun,
	// you can rest assured that they'll still be touching until
	// these two objects are in the collisionEnd function.
	void collisionBegin( btRigidBody *rigid0, btRigidBody *rigid1 );
	void collisionEnd( btRigidBody *rigid0, btRigidBody *rigid1 );
	void update();
	void draw();
	
	bullet::ContextRef mContext;
	bullet::PhyObjPrimitiveRef mBox, mCapsule,
								mPlane, mSphere,
								mCone, mCylinder,
								mMultiSphere;
	CameraPersp			mCam;
};

void PhysicsPrimitivesApp::setup()
{
	// Create a context. This Context stores all of the bullet world.
	// Take a look at the Context::Format to find out just what can be
	// controlled. Specifically here, we're creating a format that allows
	// debug Drawing
	mContext = bullet::Context::create( bullet::Context::Format().drawDebug( true ).createDebugRenderer( true ) );
	
	// Here we're creating the primitives for our physics world. These
	// are helpers on the Primitive shapes that ship with bullet. We
	// store a Reference to them in our application.
	mBox = bullet::PhyObjPrimitive::createBox( Vec3f( 1, 1, 1 ), 1, Vec3f( 0, 10, 0 ) );
	mCapsule = bullet::PhyObjPrimitive::createCapsule( 2, 5, 1, Vec3f( 3, 10, 0 ) );
	mSphere = bullet::PhyObjPrimitive::createSphere( 1, 1, Vec3f( 0.5, 20, 0 ) );
	mCone = bullet::PhyObjPrimitive::createCone( 1, 2, 1, Vec3f( -0.5, 20, 0 ) );
	mCylinder = bullet::PhyObjPrimitive::createCylinder( Vec3f( 1, 1, 1 ), 1, Vec3f( -3, 10, 3 ) );
	mPlane = bullet::PhyObjPrimitive::createStaticPlane( Vec3f( 0, 1, 0 ), 0 );

	std::vector<ci::Vec3f> positions = {
		Vec3f( 1, 0, 0 ),
		Vec3f( 0, 0, 0 ),
		Vec3f( -1, 0, 0 )
	};
	std::vector<btScalar> radii = {
		1,
		1,
		1
	};
	mMultiSphere = bullet::PhyObjPrimitive::createMultiSphere( positions, radii, 1, Vec3f( -1.5, 15, 0 ) );

	// Once a physics object is created, all we need to do is tell the
	// context about them and it'll add them to the bullet world.
	mContext->addRigidBody( mBox );
	mContext->addRigidBody( mCapsule );
	mContext->addRigidBody( mSphere );
	mContext->addRigidBody( mCone );
	mContext->addRigidBody( mCylinder );
	mContext->addRigidBody( mMultiSphere );
	mContext->addRigidBody( mPlane );
	
	// Now we'll add some collision callbacks to the world
	mContext->addCollisionBeginSignal( std::bind( &PhysicsPrimitivesApp::collisionBegin, this, std::placeholders::_1, std::placeholders::_2 ) );
	mContext->addCollisionEndSignal( std::bind( &PhysicsPrimitivesApp::collisionBegin, this, std::placeholders::_1, std::placeholders::_2 )  );
	
	// For this example we'll make a simple camera to represent
	// the physics object
	mCam.setPerspective( 75.0f, getWindowAspectRatio(), .01f, 1000.0f );
	mCam.lookAt( Vec3f( 0, 5, 20 ), Vec3f::zero() );
}

void PhysicsPrimitivesApp::collisionBegin( btRigidBody *rigid0, btRigidBody *rigid1 )
{
	using namespace bullet;
	// Unless you set the rigid body's User Pointer manually, it will
	// be set with the PhyObjPrimitives 'this' pointer. This is the way
	// all of the PhyObj's work.
	PhyObjPrimitive *phyObj0 = static_cast<PhyObjPrimitive*>( rigid0->getUserPointer() );
	PhyObjPrimitive *phyObj1 = static_cast<PhyObjPrimitive*>( rigid1->getUserPointer() );
	
	std::cout << "PhyObj0 shape: " << phyObj0->getName() << " PhyObj1 shape: " << phyObj1->getName() << std::endl;
}

void PhysicsPrimitivesApp::collisionEnd( btRigidBody *rigid0, btRigidBody *rigid1 )
{
	using namespace bullet;
	// Unless you set the rigid body's User Pointer manually, it will
	// be set with the PhyObjPrimitives 'this' pointer. This is the way
	// all of the PhyObj's work.
	PhyObjPrimitive *phyObj0 = static_cast<PhyObjPrimitive*>( rigid0->getUserPointer() );
	PhyObjPrimitive *phyObj1 = static_cast<PhyObjPrimitive*>( rigid1->getUserPointer() );
	
	std::cout << "PhyObj0 shape: " << phyObj0->getName() << " PhyObj1 shape: " << phyObj1->getName() << std::endl;
}

void PhysicsPrimitivesApp::mouseDown( MouseEvent event )
{
	
}

void PhysicsPrimitivesApp::update()
{
	// We'll update the world during update which also controls
	// the update of the debug drawer if present
	mContext->update();
}

void PhysicsPrimitivesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::setMatrices( mCam );
	// We can use this helper function to draw the world. Debug
	// Draw is a very simple class used to show us where bullet
	// thinks the objects are.
	mContext->debugDraw();
}

CINDER_APP_NATIVE( PhysicsPrimitivesApp, RendererGl )
