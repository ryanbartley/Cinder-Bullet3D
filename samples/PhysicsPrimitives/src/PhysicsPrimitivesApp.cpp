#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/Shader.h"
#include "cinder/gl/Batch.h"

#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/BulletContext.h"

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
	bullet::RigidBodyRef mBox, mCapsule,
							mPlane, mSphere,
							mCone, mCylinder,
							mMultiSphere;
	CameraPersp			mCam;
};

void PhysicsPrimitivesApp::setup()
{
	using namespace bullet;
	// Create a context. This Context stores all of the bullet world.
	// Take a look at the Context::Format to find out just what can be
	// controlled. Specifically here, we're creating a format that allows
	// debug Drawing
	mContext = Context::create( Context::Format().drawDebug( true ).createDebugRenderer( true ) );
	
	// Here we're creating the primitives for our physics world. These
	// are helpers on the Primitive shapes that ship with bullet. We
	// store a Reference to them in our application.
	mBox = RigidBody::create( RigidBody::Format().collisionShape( createBoxShape( vec3( 1, 1, 1 ) ) ).initialPosition( vec3( 0, 10, 0 ) ).mass( 1 ) );
	mCapsule = RigidBody::create( RigidBody::Format().collisionShape( createCapsuleShape( 2, 5 ) ).initialPosition( vec3( 3, 10, 0 ) ).mass( 1 ) );
	mSphere = RigidBody::create( RigidBody::Format().collisionShape( createSphereShape( 1 ) ).initialPosition( vec3( 0.5, 20, 0 ) ).mass( 1 ) );
	mCone = RigidBody::create( RigidBody::Format().collisionShape( createConeShape( 1, 2 ) ).initialPosition( vec3( -0.5, 20, 0 ) ).mass( 1 ) );
	mCylinder = RigidBody::create( RigidBody::Format().collisionShape( createCylinderShape( vec3( 1, 5, 1 ) ) ).initialPosition( vec3( -0.5, 20, 0 ) ).mass( 1 ) );
	
	// Static Plane is the only interesting one really. It's infinite,
	// So you give it a Normal first (Vec3f(0, 1, 0), is pointing up)
	// and an offset on that normal, in this case 0. It's also static
	// therefore we don't need to add a mass to it. The default
	// RigidBody::Format value for mass is 0, which makes the rigidbody
	// static, meaning not moving.
	mPlane = RigidBody::create( RigidBody::Format().collisionShape( createStaticPlaneShape( vec3( 0, 1, 0 ), 0 ) ) );

	std::vector<ci::vec3> positions = {
		vec3( 1, 0, 0 ),
		vec3( 0, 0, 0 ),
		vec3( -1, 0, 0 )
	};
	std::vector<btScalar> radii = {
		1,
		1,
		1
	};
	mMultiSphere = RigidBody::create( RigidBody::Format().collisionShape( createMultiSphereShape( positions, radii ) ).initialPosition( vec3( -1.5, 15, 0 ) ).mass( 1 ) );

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
	mCam.lookAt( vec3( 0, 5, 20 ), vec3( 0.0f ) );
}

void PhysicsPrimitivesApp::collisionBegin( btRigidBody *rigid0, btRigidBody *rigid1 )
{
	using namespace bullet;
	// Unless you set the rigid body's User Pointer manually, it will
	// be set with the PhyObjPrimitives 'this' pointer. This is the way
	// all of the PhyObj's work.
	RigidBody *phyObj0 = static_cast<RigidBody*>( rigid0->getUserPointer() );
	RigidBody *phyObj1 = static_cast<RigidBody*>( rigid1->getUserPointer() );
	
	std::cout << "PhyObj0 shape: " << phyObj0->getCollShapeName() << " PhyObj1 shape: " << phyObj1->getCollShapeName() << std::endl;
}

void PhysicsPrimitivesApp::collisionEnd( btRigidBody *rigid0, btRigidBody *rigid1 )
{
	using namespace bullet;
	// Unless you set the rigid body's User Pointer manually, it will
	// be set with the PhyObjPrimitives 'this' pointer. This is the way
	// all of the PhyObj's work.
	RigidBody *phyObj0 = static_cast<RigidBody*>( rigid0->getUserPointer() );
	RigidBody *phyObj1 = static_cast<RigidBody*>( rigid1->getUserPointer() );
	
	std::cout << "PhyObj0 shape: " << phyObj0->getCollShapeName() << " PhyObj1 shape: " << phyObj1->getCollShapeName() << std::endl;
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
