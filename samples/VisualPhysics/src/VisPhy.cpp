//
//  VisPhy.cpp
//  VisualPhysics
//
//  Created by Ryan Bartley on 8/2/14.
//
//

#include "VisPhy.h"
#include "Cinder-Bullet3D/OpenGLMotionState.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace bullet;

VisPhy::VisPhy( const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics )
: mVisObj( visual ), mPhyObj( physics )
{
}

void VisPhy::update()
{
	if(  ! mPhyObj->isKinematic() ) {
		auto motionState = static_pointer_cast<SimpleGlDynamicMotionState>(mPhyObj->getMotionState());
		motionState->getGLWorldTransform( &mModelMatrix );
	}
	else {
		auto motionState = static_pointer_cast<SimpleGlKinematicMotionState>( mPhyObj->getMotionState() );
		
	}
}

void VisPhy::draw()
{
	gl::setModelMatrix( mModelMatrix );
	
	gl::setDefaultShaderVars();
	
	mVisObj->draw();
}
