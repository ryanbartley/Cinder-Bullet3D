#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HelloWorldApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void HelloWorldApp::setup()
{
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	
	dynamicsWorld->setGravity(btVector3(0,-10,0));
	
	
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	
	btCollisionShape* fallShape = new btSphereShape(1);
	
	
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	btRigidBody::btRigidBodyConstructionInfo
	groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);
	
	
	btDefaultMotionState* fallMotionState =
	new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
	btScalar mass = 1;
	btVector3 fallInertia(0,0,0);
	fallShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
	
	
	for (int i=0 ; i<300 ; i++) {
		dynamicsWorld->stepSimulation(1/60.f,10);
		
		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);
		
		std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
	}
	
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;
	
	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;
	
	delete fallShape;
	
	delete groundShape;
	
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
	
	exit(0);
}

void HelloWorldApp::mouseDown( MouseEvent event )
{
}

void HelloWorldApp::update()
{
}

void HelloWorldApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( HelloWorldApp, RendererGl )
