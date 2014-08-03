//
//  DefaultPhysicsObject.h
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/27/13.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/OpenGLMotionState.h"

#include "cinder/Log.h"
#include "cinder/Sphere.h"
#include "cinder/AxisAlignedBox.h"

namespace bullet {
	
typedef std::shared_ptr<class RigidBody> RigidBodyRef;

class RigidBody : public boost::noncopyable, public std::enable_shared_from_this<RigidBody> {
public:
	
	struct Format {
		Format();
		virtual ~Format() {}
		
		//! Gets the Collision shape of the format.
		const btCollisionShapeRef& getCollisionShape() const { return mCollShape; }
		//! Gets the Motion state of the format.
		const PhyObjMotionStateRef& getMotionState() const { return mMotionState; }
		//! Sets the mass of the object.
		const btScalar getMass() const { return mMass; }
		//! Sets the Vec3f for the initial position of the object.
		const ci::Vec3f& getInitPosition() const { return mInitialPosition; }
		//! Sets the Vec3f for the collision shapes local scaling.
		const ci::Vec3f& getInitScale() const { return mInitialScale; }
		//! Sets the Quatf for the initial rotation of the object.
		const ci::Quatf& getInitRotation() const { return mInitialRotation; }
		//! Sets the friction of the object. Can set friction after construction too.
		const btScalar getFriction() const { return mFriction; }
		//! Returns the Restitution of the object. Can set restitution after construction too.
		const btScalar getRestitution() const { return mRestitution; }
		//! Returns the Collision Group that this object is in. This is used for bitwise operations against CollMask. Helper in Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		const int16_t getCollGroup() const { return mCollisionGroup; }
		//! Returns the Collision Mask of this object. This is used for bitwise operations against CollGroup. Helper in Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		const int16_t getCollMask() const { return mCollisionMask; }
		//! Returns whether to auto add to the world. Default is true.
		const bool getAddToWorld() const { return mAddToWorld; }
		//! Returns Collision Shape User Pointer. Defaults to nullptr.
		const bool getCSUserPointer() const { return mRigidBodyUserPtr; }
		//! Returns the object to kinematic and zeros the mass on construction and sets the appropriate flags.
		const bool getIsKinematic() const { return mSetKinematic; }
		//! Sets the Collision shape of the object.
		void setCollisionShape( const btCollisionShapeRef &shape ) { mCollShape = shape; }
		//! Sets the Motion state of the object.
		void setMotionState( const PhyObjMotionStateRef &motionState ) { mMotionState = motionState; }
		//! Sets the mass of the object.
		void setMass( btScalar mass ) { mMass = mass; }
		//! Sets the Vec3f for the initial position of the object.
		void setInitPosition( const ci::Vec3f &initialPosition ) { mInitialPosition = initialPosition; }
		//! Sets the Vec3f for the collision shapes local scaling.
		void setInitScale( const ci::Vec3f &initialScale ) { mInitialScale = initialScale; }
		//! Sets the Quatf for the initial rotation of the object.
		void setInitRotation( const ci::Quatf &initialRotation ) { mInitialRotation = initialRotation; }
		//! Sets the friction of the object. Can set friction after construction too.
		void setFriction( btScalar friction ) { mFriction = friction; }
		//! Sets the Restitution of the object. Can set restitution after construction too.
		void setRestitution( btScalar restitution ) { mRestitution = restitution; }
		//! Sets the Collision Group that this object is in. This is used for bitwise operations against CollMask. Helper in Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		void setCollGroup( int16_t collGroup ) { mCollisionGroup = collGroup; }
		//! Sets the Collision Mask of this object. This is used for bitwise operations against CollGroup. Helper in Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		void setCollMask( int16_t collMask ) { mCollisionMask = collMask; }
		//! Sets whether to auto add to the world. Default is true.
		void setAddToWorld( bool addToWorld = true ) { mAddToWorld = addToWorld; }
		//! Sets whether to add "this" instance to Rigid Body User Pointer. Defaults to the RigidBody constructed object.
		void setRBUserPointer( void* userPtr ) { mRigidBodyUserPtr = userPtr; }
		//! Sets the object to kinematic and zeros the mass on construction and sets the appropriate flags.
		void setToKinematic( bool shouldBeKinematic = true ) { mSetKinematic = shouldBeKinematic; }
		
	protected:
		btCollisionShapeRef		mCollShape;
		PhyObjMotionStateRef	mMotionState;
		btScalar				mMass, mFriction, mRestitution;
		int16_t					mCollisionGroup, mCollisionMask;
		ci::Vec3f				mInitialPosition;
		ci::Vec3f				mInitialScale;
		ci::Quatf				mInitialRotation;
		bool					mSetKinematic, mAddToWorld;
		void				   *mRigidBodyUserPtr;
		
		friend class RigidBody;
	};
	
	virtual ~RigidBody();
	
	//! Returns the const char* name assigned to the Collision Shape.
	const char* getName() const { return mCollisionShape->getName(); }
	//! Returns a string constructed from the const char* name assigned to the Collision Shape.
	std::string getName() { return mCollisionShape->getName(); }
	//! Returns the PhyObjType enumeration of the type.
	PhyObjType getType() { return mType; }
	//! Returns whether this object is added to the current world.
	bool getIsAddedToWorld() { return mAddedToWorld; }
	
	//! Sets Collision flags for the rigid body
	void setFlag( int flag ) { mRigidBody->setFlags( mRigidBody->getFlags() | flag ); }
	//! Removes Collision flags from the rigid body
	void removeFlag( int flag ) { mRigidBody->setFlags( mRigidBody->getFlags() | ~(flag) ); }
	//! Returns the current Collision flags
	int getFlags() { return mRigidBody->getFlags(); }
	
	//! Sets the activation state of this rigid body. Normal Choices are DISABLE_DEACTIVATION or ACTIVE_TAG.
	void setActivationState( int state ) { mRigidBody->setActivationState( state ); }
	
	//! Returns the pointer to the collision shape of this Object.
	btCollisionShapeRef&		getCollisionShape() { return mCollisionShape; }
	//! Returns the const pointer to the collision shape of this Object.
	const btCollisionShapeRef&	getCollisionShape() const { return mCollisionShape; }
	//! Returns the pointer to the Rigid Body of this object.
	btRigidBodyRef&					getRigidBody() { return mRigidBody; }
	//! Returns the const pointer to the Rigid Body of this object.
	const btRigidBodyRef&			getRigidBody() const { return mRigidBody; }
	//! Returns the pointer to the Motion State of this object.
	PhyObjMotionStateRef&			getMotionState() { return mMotionState; }
	//! Returns the const pointer to the Motion State of this object.
	const PhyObjMotionStateRef&		getMotionState() const { return mMotionState; }
	
	void setMotionState( const PhyObjMotionStateRef &motionState )
	{
		mRigidBody->setMotionState( motionState.get() );
	}
	const btTransform& getCenterOfMassTransform() const
	{
		return mRigidBody->getCenterOfMassTransform();
	}
	const btVector3& getCenterOfMassPoint() const
	{
		return mRigidBody->getCenterOfMassPosition();
	}
	
	void setCollisionGroup( int16_t collGroup );
	void setCollisionMask( int16_t collMask );
	
	int16_t getCollisionGroup() { return mCollGroup; }
	int16_t getCollisionMask() { return mCollMask; }
	
	//! Returns the radius float from the current bounding sphere measure.
	float				getBoundingSphereRadius() { return mBoundingSphereRadius; }
	//! Returns the Vec3f center from the current bounding sphere measure.
	ci::Vec3f&			getBoundingSphereCenter() { return mBoundingSphereCenter; }
	//! Returns the const Vec3f center from the current bounding sphere measure.
	const ci::Vec3f&	getBoundingSphereCenter() const { return mBoundingSphereCenter; }
	
	bool isKinematic() { return mRigidBody->isKinematicObject(); }
	bool isStatic() { return mRigidBody->isStaticObject(); }
	
	bool isCollisionShapeScaled() { return mUpdatedScale; }
	
	//! Returns the collision shapes bounding sphere for quick culling purposes
	inline void getBoundingSphere( ci::Sphere &sphere )
	{
		sphere.setCenter( fromBullet( mMotionState->m_graphicsWorldTrans.getOrigin() ) );
		sphere.setRadius( getBoundingSphereRadius() );
	}
	
	//! Returns the collision shapes axis aligned bounding box for culling purposes
	inline void getAabb( ci::AxisAlignedBox3f &box )
	{
		btVector3 min; btVector3 max;
		mCollisionShape->getAabb( mMotionState->m_graphicsWorldTrans, min, max );
		box = ci::AxisAlignedBox3f( fromBullet( min ), fromBullet( max ) );
	}
	
	//! Applies central force to the contained rigid body
	inline void applyCentralForce( const ci::Vec3f &force )
	{
		mRigidBody->applyCentralForce( toBullet( force ) );
	}
	
	//! Applies torque to the contained rigid body
	inline void applyTorque( const ci::Vec3f &torque )
	{
		mRigidBody->applyTorque( toBullet( torque ) );
	}
	
	//! Applies an impulse to the center of the contained rigid body
	inline void applyCentralImpulse( const ci::Vec3f &centralImpulse )
	{
		mRigidBody->applyCentralImpulse( toBullet( centralImpulse ) );
	}
	
	//! Sets the linear velocity for the contained rigid body
	inline void setLinearVelocity( const ci::Vec3f &linearVelocity )
	{
		mRigidBody->setLinearVelocity( toBullet( linearVelocity ) );
	}
	
	//! Sets the angular velocity for the contained rigid body
	inline void setAngularVelocity( const ci::Vec3f &angularVelocity )
	{
		mRigidBody->setAngularVelocity( toBullet( angularVelocity ) );
	}
	
	//! Returns the current Linear Velocity of the contained Rigid Body
	inline ci::Vec3f getLinearVelocity()
	{
		return fromBullet( mRigidBody->getLinearVelocity() );
	}
	
	//! Returns the current Angular Velocity of the contained Rigid Body
	inline ci::Vec3f getAngularVelocity()
	{
		return fromBullet( mRigidBody->getAngularVelocity() );
	}
	
	//! Sets the Rigid Body User Pointer.
	inline void setUserPointerRigidBody( void *ptr ) { mRigidBody->setUserPointer( ptr ); }
	
	//! Sets the Collision Shape User Pointer.
	inline void setUserPointerCollisionShape( void *ptr ) { mCollisionShape->setUserPointer( ptr ); }
	
	//! Sets the Motion State User Pointer. This pointer will be used to set and get back the transform for this object.
	inline void setUserPointerMotionState( void *ptr ) { mMotionState->setUserPointer( ptr ); }
	
protected:
	//! Base Class for a Physics Object.
	RigidBody();
	
	//! Initializes this object.
	virtual void init( const Format &format );
	
	void recalculateBoundingSphere();
	
	//! Sets whether this object has been added to the world. This is called internally, don't mess with it unless you know what you're doing.
	void setIsAddedToWorld( bool addedToWorld ) { mAddedToWorld = addedToWorld; }
	
	btRigidBodyRef			mRigidBody;
	btCollisionShapeRef		mCollisionShape;
	PhyObjMotionStateRef	mMotionState;
	
	ci::Vec3f			mBoundingSphereCenter;
	btScalar			mBoundingSphereRadius;
	PhyObjType			mType;
	bool				mAddedToWorld, mUpdatedScale;
	int16_t				mCollGroup, mCollMask;
	
	friend class Context;
};
		
}