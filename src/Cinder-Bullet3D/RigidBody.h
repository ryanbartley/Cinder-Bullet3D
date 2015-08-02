//
//  DefaultPhysicsObject.h
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/27/13.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/MotionState.h"

#include "cinder/Log.h"
#include "cinder/Sphere.h"
#include "cinder/AxisAlignedBox.h"

namespace bullet {
	
typedef std::shared_ptr<class RigidBody> RigidBodyRef;

class RigidBody : public std::enable_shared_from_this<RigidBody> {
public:
	
	struct Format {
		Format();
		virtual ~Format() {}
		
		//! Gets the Collision shape of the format.
		const btCollisionShapeRef& getCollisionShape() const { return mCollShape; }
		//! Gets the Motion state of the format.
		const MotionStateRef& getMotionState() const { return mMotionState; }
		//! Sets the mass of the object.
		const btScalar getMass() const { return mMass; }
		//! Sets the vec3 for the initial position of the object.
		const ci::vec3& getInitPosition() const { return mInitialPosition; }
		//! Sets the vec3 for the collision shapes local scaling.
		const ci::vec3& getInitScale() const { return mInitialScale; }
		//! Sets the Quatf for the initial rotation of the object.
		const ci::quat& getInitRotation() const { return mInitialRotation; }
		//! Sets the friction of the object. Can set friction after construction too.
		const btScalar getFriction() const { return mFriction; }
		//! Returns the Restitution of the object. Can set restitution after construction too.
		const btScalar getRestitution() const { return mRestitution; }
		//! Returns the Collision Group that this object is in. This is used for bitwise operations against CollMask. Helper in system/Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		const int16_t getCollGroup() const { return mCollisionGroup; }
		//! Returns the Collision Mask of this object. This is used for bitwise operations against CollGroup. Helper in system/Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		const int16_t getCollMask() const { return mCollisionMask; }
		//! Returns whether to auto add to the world. Default is true.
		const bool getAddToWorld() const { return mAddToWorld; }
		//! Returns Collision Shape User Pointer. Defaults to nullptr.
		void* getCSUserPointer() const { return mRigidBodyUserPtr; }
		//! Returns the object to kinematic and zeros the mass on construction and sets the appropriate flags.
		const bool isKinematic() const { return mSetKinematic; }
		//! Sets the Collision shape of the object.
		void setCollisionShape( const btCollisionShapeRef &shape ) { mCollShape = shape; }
		//! Sets the Motion state of the object.
		void setMotionState( const MotionStateRef &motionState ) { mMotionState = motionState; }
		//! Sets the mass of the object.
		void setMass( btScalar mass ) { mMass = mass; }
		//! Sets the vec3 for the initial position of the object.
		void setInitPosition( const ci::vec3 &initialPosition ) { mInitialPosition = initialPosition; }
		//! Sets the vec3 for the collision shapes local scaling.
		void setInitScale( const ci::vec3 &initialScale ) { mInitialScale = initialScale; }
		//! Sets the Quatf for the initial rotation of the object.
		void setInitRotation( const ci::quat &initialRotation ) { mInitialRotation = initialRotation; }
		//! Sets the friction of the object. Can set friction after construction too.
		void setFriction( btScalar friction ) { mFriction = friction; }
		//! Sets the Restitution of the object. Can set restitution after construction too.
		void setRestitution( btScalar restitution ) { mRestitution = restitution; }
		//! Sets the Collision Group that this object is in. This is used for bitwise operations against CollMask. Helper in system/Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		void setCollGroup( int16_t collGroup ) { mCollisionGroup = collGroup; }
		//! Sets the Collision Mask of this object. This is used for bitwise operations against CollGroup. Helper in system/Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default will collide with everything.
		void setCollMask( int16_t collMask ) { mCollisionMask = collMask; }
		//! Sets whether to auto add to the world. Default is true.
		void setAddToWorld( bool addToWorld = true ) { mAddToWorld = addToWorld; }
		//! Sets whether to add "this" instance to Rigid Body User Pointer. Defaults to the RigidBody constructed object.
		void setRBUserPointer( void* userPtr ) { mRigidBodyUserPtr = userPtr; }
		//! Sets the object to kinematic and zeros the mass on construction and sets the appropriate flags.
		void setToKinematic( bool shouldBeKinematic = true ) { mSetKinematic = shouldBeKinematic; }
		
		//! Sets the Collision shape of the object. Default is nullptr.
		Format& collisionShape( const btCollisionShapeRef &shape ) { mCollShape = shape; return *this; }
		//! Sets the Motion State of the object. Default is nullptr.
		Format& motionState( const MotionStateRef &motionState ) { mMotionState = motionState; return *this; }
		//! Sets the mass of the object. Default is 0.0f or static object.
		Format& mass( btScalar mass ) { mMass = mass; return *this; }
		//! Sets the vec3 for the initial position of the object. Default is vec3::yAxis() or vec3( 0.0f, 1.0f, 0.0f ).
		Format& initialPosition( const ci::vec3 &initialPosition ) { mInitialPosition = initialPosition; return *this; }
		//! Sets the vec3 for the initial scale of the object. Default is vec3( 1.0f, 1.0f, 1.0f ).
		Format& initialScale( const ci::vec3 &initialScale ) { mInitialScale = initialScale; return *this; }
		//! Sets the Quatf for the initial rotation of the object. Default is Quatf::identity()
		Format& initialRotation( const ci::quat &initialRotation ) { mInitialRotation = initialRotation; return *this; }
		//! Sets the friction of the object. Can set friction after construction too. Default is 0.5.
		Format& friction( btScalar friction ) { mFriction = friction; return *this; }
		//! Sets the Restitution of the object. Can set restitution after construction too. Default is 0.0.
		Format& restitution( btScalar restitution ) { mRestitution = restitution; return *this; }
		//! Sets the Collision Group that this object is in. This is used for bitwise operations against CollMask. Helper in system/Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default is -1 or all bits set.
		Format& collGroup( int16_t collGroup ) { mCollisionGroup = collGroup; return *this; }
		//! Sets the Collision Mask of this object. This is used for bitwise operations against CollGroup. Helper in system/Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default is -1 or all bits set.
		Format& collMask( int16_t collMask ) { mCollisionMask = collMask; return *this; }
		//! Sets whether to auto add to the world. Default is true.
		Format& addToWorld( bool addToWorld ) { mAddToWorld = addToWorld; return *this; }
		//! Sets whether to add "this" instance to Rigid Body User Pointer. Defaults to the RigidBody constructed object.
		Format& rigidUserPointer( void* userPtr ) { mRigidBodyUserPtr = userPtr; return *this; }
		//! Sets the object to kinematic and zeros the mass on construction and sets the appropriate flags.
		Format& kinematic( bool shouldBeKinematic = true ) { mSetKinematic = shouldBeKinematic; return *this; }
		
	protected:
		btCollisionShapeRef		mCollShape;
		MotionStateRef	mMotionState;
		btScalar				mMass, mFriction, mRestitution;
		int16_t					mCollisionGroup, mCollisionMask;
		ci::vec3				mInitialPosition;
		ci::vec3				mInitialScale;
		ci::quat				mInitialRotation;
		bool					mSetKinematic, mAddToWorld;
		void				   *mRigidBodyUserPtr;
		
		friend class RigidBody;
	};
	
	//! Returns a shared_ptr of RigidBody created from \a format
	static RigidBodyRef create( const Format &format );
	
	RigidBody( const RigidBody &other ) = delete;
	RigidBody( RigidBody &&other ) = delete;
	RigidBody& operator=( const RigidBody &other ) = delete;
	RigidBody& operator=( RigidBody &&other ) = delete;
	
	virtual ~RigidBody();
	
	//! Returns the const char* name assigned to the Collision Shape.
	const char* getCollShapeName() const { return mCollisionShape->getName(); }
	//! Returns a string constructed from the const char* name assigned to the Collision Shape.
	std::string getCollShapeName() { return mCollisionShape->getName(); }
	//! Returns the PhyObjType enumeration of the type.
	PhyObjType getCollShapeType() { return mType; }
	//! Returns whether this object is added to the current world.
	bool isAddedToWorld() { return mAddedToWorld; }
	//! Returns the pointer to the collision shape of this Object.
	btCollisionShapeRef&		getCollisionShape() { return mCollisionShape; }
	//! Returns the const pointer to the collision shape of this Object.
	const btCollisionShapeRef&	getCollisionShape() const { return mCollisionShape; }
	
	void setFlag( int flag ) { mRigidBody->setFlags( mRigidBody->getFlags() | flag ); }
	void removeFlag( int flag ) { mRigidBody->setFlags( mRigidBody->getFlags() | ~(flag) ); }
	int getFlags() { return mRigidBody->getFlags(); }
	
	//! Sets Collision flags for the rigid body
	void setCollisionFlags( int collisionFlag ) { mRigidBody->setCollisionFlags( mRigidBody->getCollisionFlags() | collisionFlag ); }
	//! Removes Collision flags from the rigid body
	void removeCollisionFlags( int collisionFlag ) { mRigidBody->setCollisionFlags( mRigidBody->getCollisionFlags() | ~(collisionFlag) ); }
	//! Returns the current Collision flags
	int getCollisionFlags() { return mRigidBody->getCollisionFlags(); }
	
	//! Sets the activation state of this rigid body. Normal Choices are DISABLE_DEACTIVATION or ACTIVE_TAG.
	void setActivationState( int state ) { mRigidBody->setActivationState( state ); }
	
	//! Returns the pointer to the Rigid Body of this object.
	btRigidBodyRef&					getRigidBody() { return mRigidBody; }
	//! Returns the const pointer to the Rigid Body of this object.
	const btRigidBodyRef&			getRigidBody() const { return mRigidBody; }
	//! Returns the pointer to the Motion State of this object.
	MotionStateRef&			getMotionState() { return mMotionState; }
	//! Returns the const pointer to the Motion State of this object.
	const MotionStateRef&		getMotionState() const { return mMotionState; }
	
	void setMotionState( const MotionStateRef &motionState )
	{
		mMotionState = motionState;
		mRigidBody->setMotionState( mMotionState.get() );
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
	//! Returns the vec3 center from the current bounding sphere measure.
	ci::vec3&			getBoundingSphereCenter() { return mBoundingSphereCenter; }
	//! Returns the const vec3 center from the current bounding sphere measure.
	const ci::vec3&	getBoundingSphereCenter() const { return mBoundingSphereCenter; }
	
	bool isKinematic() { return mRigidBody->isKinematicObject(); }
	bool isStatic() { return mRigidBody->isStaticObject(); }
	
	bool isCollisionShapeScaled() { return mUpdatedScale; }
	
	inline void setDamping( float linear, float angular )
	{
		mRigidBody->setDamping( linear, angular );
	}
	
	inline void setDeactivationTime( float time )
	{
		mRigidBody->setDeactivationTime( time );
	}
	
	inline void setSleepingThresholds( btScalar linear, btScalar angular )
	{
		mRigidBody->setSleepingThresholds( linear, angular );
	}
	
	//! Returns the collision shapes bounding sphere for quick culling purposes
	inline void getBoundingSphere( ci::Sphere &sphere )
	{
		sphere.setCenter( fromBullet( mMotionState->m_graphicsWorldTrans.getOrigin() ) );
		sphere.setRadius( getBoundingSphereRadius() );
	}
	
	//! Returns the collision shapes axis aligned bounding box for culling purposes
	inline void getAabb( ci::AxisAlignedBox &box )
	{
		btVector3 min; btVector3 max;
		mRigidBody->getAabb( min, max );
		box = ci::AxisAlignedBox( fromBullet( min ), fromBullet( max ) );
	}
	
	//! Applies central force to the contained rigid body
	inline void applyCentralForce( const ci::vec3 &force )
	{
		mRigidBody->applyCentralForce( toBullet( force ) );
	}
	
	//! Applies torque to the contained rigid body
	inline void applyTorque( const ci::vec3 &torque )
	{
		mRigidBody->applyTorque( toBullet( torque ) );
	}
	
	//! Applies an impulse to the center of the contained rigid body
	inline void applyCentralImpulse( const ci::vec3 &centralImpulse )
	{
		mRigidBody->applyCentralImpulse( toBullet( centralImpulse ) );
	}
	
	//! Sets the linear velocity for the contained rigid body
	inline void setLinearVelocity( const ci::vec3 &linearVelocity )
	{
		mRigidBody->setLinearVelocity( toBullet( linearVelocity ) );
	}
	
	//! Sets the angular velocity for the contained rigid body
	inline void setAngularVelocity( const ci::vec3 &angularVelocity )
	{
		mRigidBody->setAngularVelocity( toBullet( angularVelocity ) );
	}
	
	//! Returns the current Linear Velocity of the contained Rigid Body
	inline ci::vec3 getLinearVelocity()
	{
		return fromBullet( mRigidBody->getLinearVelocity() );
	}
	
	//! Returns the current Angular Velocity of the contained Rigid Body
	inline ci::vec3 getAngularVelocity()
	{
		return fromBullet( mRigidBody->getAngularVelocity() );
	}
	
	//! Sets the Rigid Body User Pointer.
	inline void setUserPointerRigidBody( void *ptr ) { mRigidBody->setUserPointer( ptr ); }
	
	//! Sets the Collision Shape User Pointer.
	inline void setUserPointerCollisionShape( void *ptr ) { mCollisionShape->setUserPointer( ptr ); }
	
	//! Sets the Motion State User Pointer. This pointer will be used to set and get back the transform for this object.
	inline void setUserPointerMotionState( void *ptr ) { mMotionState->setUserPointer( ptr ); }
	
	inline void setLinearFactor( const ci::vec3 &factor ) { mRigidBody->setLinearFactor( toBullet( factor ) ); }
	inline void setAngularFactor( const ci::vec3 &factor ) { mRigidBody->setAngularFactor( toBullet( factor ) ); }
	
protected:
	//! Base Class for a Physics Object.
	RigidBody( const Format &format );
	
	//! Initializes this object.
	virtual void init( const Format &format );
	
	void recalculateBoundingSphere();
	
	//! Sets whether this object has been added to the world. This is called internally, don't mess with it unless you know what you're doing.
	void setIsAddedToWorld( bool addedToWorld ) { mAddedToWorld = addedToWorld; }
	
	btRigidBodyRef			mRigidBody;
	btCollisionShapeRef		mCollisionShape;
	MotionStateRef			mMotionState;
	
	ci::vec3			mBoundingSphereCenter;
	btScalar			mBoundingSphereRadius;
	PhyObjType			mType;
	bool				mAddedToWorld, mUpdatedScale;
	int16_t				mCollGroup, mCollMask;
	
	friend class Context;
};
		
}