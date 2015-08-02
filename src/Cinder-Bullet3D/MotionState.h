//
//  OpenGLMotionState.h
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/25/13.
//
//

#pragma once

#include "cinder/Matrix44.h"
#include "Cinder-Bullet3D/Common.h"

namespace bullet {
	
typedef std::shared_ptr<class MotionState>	MotionStateRef;
	
class MotionState : public btDefaultMotionState {
public:
	virtual ~MotionState() {}
	
	//! Returns whether this Motion state is a dynamic state, meaning controlled by Bullet.
	inline bool isDynamic() { return mDynamic; }
	//! Returns whether this Motion state is a kinematic state, meaning controlled by the user.
	inline bool isKinematic() { return ! mDynamic; }
	
	//! Sets user pointer for this object.
	inline void setUserPointer( void *userPointer ) { m_userPointer = userPointer; }
	//! Returns user pointer for this object.
	inline void* getUserPointer() { return m_userPointer; }
	
protected:
	//! This should be a base class with implementations for getting and setting world transform.
	MotionState( const btTransform &transform, bool dynamic )
	: btDefaultMotionState( transform ), mDynamic( dynamic ) {}
	
	btTransform		mPosition;
	const bool		mDynamic;
};
	
class SimpleGlDynamicMotionState : public MotionState {
public:
	//! Used for Dynamic objects that bullet controls.
	SimpleGlDynamicMotionState( const btTransform &initialPosition )
	: MotionState( initialPosition, true )
	{ mPosition = initialPosition; }
	
	virtual ~SimpleGlDynamicMotionState() {}
	
	//! Returns the cinder gl World transform from bullet after bullet has set the world transform
	inline void getGLWorldTransform( ci::mat4 *transform )
	{
		btTransform trans;
		getWorldTransform(trans);
		ATTRIBUTE_ALIGNED16(btScalar m[16]);
		trans.getOpenGLMatrix( m );
		*transform = glm::make_mat4( m );
	}
};
	
class SimpleGlKinematicMotionState : public MotionState {
public:
	//! Used for a kinematic object, Bullet will query the world transform for each step simulation. Implemented from http://www.bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates.
	SimpleGlKinematicMotionState( const btTransform &initialPosition )
	: MotionState( initialPosition, false )
	{ mPosition = initialPosition; }
	
	virtual ~SimpleGlKinematicMotionState() {}
	
	//! Sets the position for kinematic objects, controlling the where the object is for bullet.
	void setKinematicPos( const btTransform &trans ) { mPosition = trans; }
	
	//! Do not use this as it's empty because because we're setting the transform from kinematic position
	virtual void setWorldTransform( const btTransform &worldTrans ) {}
	
	//! Returns the Bullet World Transform. Used internally by Bullet when updating the step simulation.
	virtual void getWorldTransform( btTransform &worldTrans ) const
	{
		worldTrans = mPosition;
	}
};

using SimpleGlDynamicMotionStateRef = std::shared_ptr<SimpleGlDynamicMotionState>;
using SimpleGlKinematicMotionStateRef = std::shared_ptr<SimpleGlKinematicMotionState>;


}