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
	~VisPhy();
	
	void update();
	void draw();
	
private:
	VisPhy( const ci::geom::Source &source );
	
	bullet::PhyObjPrimitiveRef	mPhyObj;
	ci::gl::BatchRef			mVisObj;
};
