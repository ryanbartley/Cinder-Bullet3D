//
//  VisPhy.cpp
//  VisualPhysics
//
//  Created by Ryan Bartley on 8/2/14.
//
//

#include "VisPhy.h"
#include "Cinder-Bullet3D/MotionState.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace bullet;

VisPhy::VisPhy( const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics )
: mVisObj( visual ), mPhyObj( physics )
{
	mMotionState = SimpleGlDynamicMotionStateRef( new SimpleGlDynamicMotionState( mPhyObj->getCenterOfMassTransform() ) );
	mPhyObj->setMotionState( mMotionState );
}

void VisPhy::update()
{
	// This grabs the GL world transform from the synchronized Motion State. Everytime bullet
	// updates, it will update the value inside of the motion state. There's no scale because
	// scale is stored inside the collision shape. You can call the Collision Shape methods,
	// setLocalScaling, getLocalScaling but you can't reuse the shape at different scales if you do.
	mMotionState->getGLWorldTransform( &mModelMatrix );
}

void VisPhy::draw()
{
	// All we really have to do is setup the model matrix.
	gl::setModelMatrix( mModelMatrix );
	mVisObj->draw();
}
