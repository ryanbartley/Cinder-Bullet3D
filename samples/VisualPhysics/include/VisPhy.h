//
//  VisPhy.h
//  VisualPhysics
//
//  Created by Ryan Bartley on 8/2/14.
//
//

#pragma once

#include "Cinder-Bullet3D/PhyObjPrimitive.h"
#include "cinder/gl/Batch.h"

class VisPhy {
public:
	~VisPhy() {}
	
	void update();
	void draw();
	
	ci::gl::BatchRef&		getBatch() { return mVisObj; }
	bullet::RigidBodyRef&	getPhyObj() { return mPhyObj; }
	
private:
	VisPhy( const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics );
	
	ci::gl::BatchRef			mVisObj;
	bullet::RigidBodyRef		mPhyObj;
	ci::Matrix44f				mModelMatrix;
};
