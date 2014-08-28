#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Cinder-Bullet3D/BulletContext.h"
#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/ConstraintHinge.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class ConstraintsApp : public AppNative {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	
	void setupGearConstraint();
	
	bt::ContextRef		mBulletContext;
};

void ConstraintsApp::setup()
{
	mBulletContext = bt::Context::create();
}

void ConstraintsApp::mouseDown( MouseEvent event )
{
}

void ConstraintsApp::update()
{
}

void ConstraintsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

#define THETA SIMD_PI/4.f
#define L_1 (2 - tan(THETA))
#define L_2 (1 / cos(THETA))
#define RATIO L_2 / L_1

void ConstraintsApp::setupGearConstraint()
{
	using namespace bt;
	RigidBodyRef bodyA, bodyB;
	
	{
		auto cylA = make_pair( createCylinderShape( vec3(0.2,0.25,0.2) ), btTransform::getIdentity() );
		auto cylB = make_pair( createCylinderShape( vec3(L_1,0.025,L_1) ), btTransform::getIdentity() );
		auto cyl0 = createCompoundShape( { cylA, cylB } );
		
		auto body = RigidBody::create( RigidBody::Format()
									  .mass( 6.28 )
									  .initialPosition( vec3( -8, 1, -8 ) )
									  .collisionShape( cyl0 ) );
		
		mBulletContext->addRigidBody(body);
		
		body->setLinearFactor( vec3( 0, 0, 0 ) );
		body->setAngularFactor( vec3( 0, 1, 0 ) );
		bodyA = body;
	}
	
	{
		auto cylA = make_pair( createCylinderShape( vec3( 0.2, 0.26, 0.2 ) ), btTransform::getIdentity() );
		auto cylB = make_pair( createCylinderShape( vec3( L_2, 0.025, L_2 ) ), btTransform::getIdentity() );
		auto cyl0 = createCompoundShape( { cylA, cylB } );
		
		auto body = RigidBody::create( RigidBody::Format()
									  .mass( 6.28 )
									  .initialPosition( vec3( -10, 2, -8 ) )
									  .initialRotation( quat( -THETA, vec3( 0, 0, 1 ) ) )
									  .collisionShape( cyl0 ) );
		
		body->setLinearFactor( vec3( 0 ) );
	}
//		auto hinge = ConstraintHinge::create( ConstraintHinge::Format().objA( body ) );
//		btHingeConstraint(*body,btVector3(0,0,0),btVector3(0,1,0),true);
//		m_dynamicsWorld->addConstraint(hinge);
//		bodyB= body;
//		body->setAngularVelocity(btVector3(0,3,0));
//		
//		m_dynamicsWorld->addRigidBody(body);
//	}
//	
//	btVector3	axisA(0,1,0);
//	btVector3	axisB(0,1,0);
//	btQuaternion orn(btVector3(0,0,1),-THETA);
//	btMatrix3x3 mat(orn);
//	axisB = mat.getRow(1);
//	
//	btGearConstraint* gear = new btGearConstraint(*bodyA,*bodyB, axisA,axisB,RATIO);
//	m_dynamicsWorld->addConstraint(gear,true);
}



CINDER_APP_NATIVE( ConstraintsApp, RendererGl )
