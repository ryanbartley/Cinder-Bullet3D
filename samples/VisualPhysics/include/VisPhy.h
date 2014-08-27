//
//  VisPhy.h
//  VisualPhysics
//
//  Created by Ryan Bartley on 8/2/14.
//
//

#pragma once

#include "Cinder-Bullet3D/RigidBody.h"
#include "cinder/gl/Batch.h"

class VisPhy {
public:
	VisPhy( const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics );
	~VisPhy() {}
	
	void update();
	void draw();
	
	ci::gl::BatchRef&		getBatch() { return mVisObj; }
	bullet::RigidBodyRef&	getPhyObj() { return mPhyObj; }
private:
	
	ci::gl::BatchRef						mVisObj;
	bullet::RigidBodyRef					mPhyObj;
	bullet::SimpleGlDynamicMotionStateRef	mMotionState;
	ci::mat4								mModelMatrix;
};

