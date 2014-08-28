#pragma once

#include "Cinder-Bullet3D/RigidBody.h"
#include "cinder/gl/Batch.h"

class Body {
public:
	Body( int index, float size, const ci::gl::BatchRef &visual, const bullet::RigidBodyRef &physics );
	~Body() {}
	
	void update( float time );
	void attract( const ci::vec3 &attractorPos, float attractorScale, float strength );
	void draw();
	ci::mat4 getMatrix(){ return mModelMatrix * mScaleMatrix; };
	
	int						getId(){		return mId; }
	ci::vec3				getPos(){		return mPos; }
	float					getSize(){		return mSize * mScale; }
	float					getStrength(){	return mStrength; }
	
private:
	int						mId;
	float					mSize;
	ci::vec3				mPos;
	float					mScale;
	float					mStrength;
	
	ci::gl::BatchRef		mVisObj;
	bullet::RigidBodyRef	mPhyObj;
	bullet::SimpleGlDynamicMotionStateRef	mMotionState;
	ci::mat4				mModelMatrix;
	ci::mat4				mScaleMatrix;
};
