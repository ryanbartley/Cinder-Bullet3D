#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Cinder-Bullet3D/BulletContext.h"
#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/Common.h"

// NOTE: This sample is not finished. It is experimental as I'm trying to figure out
// how best to wrap the constraints. Use constraints at your own risk.

using namespace ci;
using namespace ci::app;
using namespace std;

class ConstraintsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	
	void setupGearConstraint();
	
	bt::ContextRef				mBulletContext;
	bt::RigidBodyRef			mBodyA, mBodyB;
	bt::ConstraintHingeRef		mHinge;
	bt::btTypedConstraintRef	mGear;
	std::vector<bt::btCollisionShapeRef> mShapes;
	CameraPersp					mCam;
};

void ConstraintsApp::setup()
{
	mBulletContext = bt::Context::create( bt::Context::Format().drawDebug( true ).createDebugRenderer( true ) );
	setupGearConstraint();
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
	mCam.lookAt( vec3( -8, 1, 0 ), vec3( -8, 1, -8 ) );
}

void ConstraintsApp::mouseDown( MouseEvent event )
{
}

void ConstraintsApp::update()
{
	mBulletContext->update();
}

void ConstraintsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::setMatrices( mCam );
	
	mBulletContext->debugDraw();
}

#define THETA SIMD_PI/4.f
#define L_1 (2 - tan(THETA))
#define L_2 (1 / cos(THETA))
#define RATIO L_2 / L_1

void ConstraintsApp::setupGearConstraint()
{
	using namespace bt;
	
	
	{
		auto cylA = make_pair( createCylinderShape( vec3(0.2,0.25,0.2) ), btTransform::getIdentity() );
		auto cylB = make_pair( createCylinderShape( vec3(L_1,0.025,L_1) ), btTransform::getIdentity() );
		auto cyl0 = createCompoundShape( { cylA, cylB } );
		
		mShapes.push_back( cylA.first );
		mShapes.push_back( cylB.first );
		mShapes.push_back( cyl0 );
		
		auto body = RigidBody::create( RigidBody::Format()
									  .mass( 6.28 )
									  .initialPosition( vec3( -8, 1, -8 ) )
									  .collisionShape( cyl0 ) );
		
		mBulletContext->addRigidBody(body);
		
		body->setLinearFactor( vec3( 0, 0, 0 ) );
		body->setAngularFactor( vec3( 0, 1, 0 ) );
		mBodyA = body;
	}
	
	{
		auto cylA = make_pair( createCylinderShape( vec3( 0.2, 0.26, 0.2 ) ), btTransform::getIdentity() );
		auto cylB = make_pair( createCylinderShape( vec3( L_2, 0.025, L_2 ) ), btTransform::getIdentity() );
		auto cyl0 = createCompoundShape( { cylA, cylB } );
		
		mShapes.push_back( cylA.first );
		mShapes.push_back( cylB.first );
		mShapes.push_back( cyl0 );
		
		auto body = RigidBody::create( RigidBody::Format()
									  .mass( 6.28 )
									  .initialPosition( vec3( -10, 2, -8 ) )
									  .initialRotation( quat( -THETA, vec3( 0, 0, 1 ) ) )
									  .collisionShape( cyl0 ) );
		
		body->setLinearFactor( vec3( 0 ) );
	
		mBodyB = body;
		body->setAngularVelocity( vec3( 0, 3, 0 ) );
		
		mBulletContext->addRigidBody(body);
	}

	btVector3	axisA(0,1,0);
	btVector3	axisB(0,1,0);
	btQuaternion orn(btVector3(0,0,1),-THETA);
	btMatrix3x3 mat(orn);
	axisB = mat.getRow(1);

	mGear =  btTypedConstraintRef( new btGearConstraint(*( mBodyA.get()->getRigidBody()),*( mBodyB.get()->getRigidBody()), axisA,axisB,RATIO) );
	mBulletContext->addConstraint( mGear, true );
}



CINDER_APP( ConstraintsApp, RendererGl )
