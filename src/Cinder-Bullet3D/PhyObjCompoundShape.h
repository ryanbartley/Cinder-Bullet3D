//
//  CompoundShape.h
//  Prototype_3
//
//  Created by Ryan Bartley on 3/25/14.
//
//

#pragma once

#include "PhyObjBase.h"

namespace bullet {

typedef std::vector<btTransform> Offsets;
typedef std::map<btCollisionShape*, Offsets> ShapesAndOffsets;
typedef std::shared_ptr<class PhyObjCompoundShape> PhyObjCompoundShapeRef;
	
class PhyObjCompoundShape : public PhyObjBase {
public:
	
	struct Format : PhyObjBase::Format {
		Format();
		
		//! Sets the Collision shape of the object. Default is nullptr.
		Format& collisionShape( const btCollisionShapeRef &shape ) { mCollShape = shape; return *this; }
		//! Sets the mass of the object. Default is 0.0f or static object.
		Format& mass( btScalar mass ) { mMass = mass; return *this; }
		//! Sets the Vec3f for the initial position of the object. Default is Vec3f::yAxis() or Vec3f( 0.0f, 1.0f, 0.0f ).
		Format& initialPosition( const ci::Vec3f &initialPosition ) { mInitialPosition = initialPosition; return *this; }
		//! Sets the Vec3f for the initial scale of the object. Default is Vec3f( 1.0f, 1.0f, 1.0f ).
		Format& initialScale( const ci::Vec3f &initialScale ) { mInitialScale = initialScale; return *this; }
		//! Sets the Quatf for the initial rotation of the object. Default is Quatf::identity()
		Format& initialRotation( const ci::Quatf &initialRotation ) { mInitialRotation = initialRotation; return *this; }
		//! Sets the friction of the object. Can set friction after construction too. Default is 0.5.
		Format& friction( btScalar friction ) { mFriction = friction; return *this; }
		//! Sets the Restitution of the object. Can set restitution after construction too. Default is 0.0.
		Format& restitution( btScalar restitution ) { mRestitution = restitution; return *this; }
		//! Sets the Collision Group that this object is in. This is used for bitwise operations against CollMask. Helper in Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default is -1 or all bits set.
		Format& collGroup( int16_t collGroup ) { mCollisionGroup = collGroup; return *this; }
		//! Sets the Collision Mask of this object. This is used for bitwise operations against CollGroup. Helper in Common.h #define BIT(). Other things with this bit in their mask will collide with this object. Default is -1 or all bits set.
		Format& collMask( int16_t collMask ) { mCollisionMask = collMask; return *this; }
		//! Sets whether to auto add to the world. Default is true.
		Format& addToWorld( bool addToWorld ) { mAddToWorld = addToWorld; return *this; }
		//! Sets whether to add "this" instance to Rigid Body User Pointer. Defaults to the PhyObjBase constructed object.
		Format& rigidUserPointer( void* userPtr ) { mMotionStateUserPtr = userPtr; return *this; }
		//! Sets whether to add "this" instance to Rigid Body User Pointer. Defaults to the PhyObjBase constructed object.
		Format& collisionUserPointer( void* userPtr ) { mCollisionShapeUserPtr = userPtr; return *this; }
		//! Sets whether to add "this" instance to Motion State User Pointer. Defaults to null.
		Format& motionUserPointer( void* userPtr ) { mMotionStateUserPtr = userPtr; return *this; }
		//! Sets the object to kinematic and zeros the mass on construction and sets the appropriate flags.
		Format& kinematic( bool shouldBeKinematic = true ) { mSetKinematic = shouldBeKinematic; return *this; }
		
		Format& shapesAndOffsets( const ShapesAndOffsets &shapesOffsets ) { mShapesAndOffsets = shapesOffsets; return *this; }
		
	protected:
		ShapesAndOffsets mShapesAndOffsets;
		
		friend class PhyObjCompoundShape;
	};
	
	static PhyObjCompoundShapeRef create( const Format &format );
	
	~PhyObjCompoundShape();
	
	inline void addChildShape( const btTransform &localTransform, btCollisionShape* shape ) {}
	inline void removeChildShape( btCollisionShape* shape ) { getCompoundShape()->removeChildShape( shape ); }
	
	inline btCompoundShapeChild* getChildList() { return getCompoundShape()->getChildList(); }
	inline btCollisionShape* getChildShape( int index ) { return getCompoundShape()->getChildShape( index ); }
	inline const btCollisionShape* getChildShape( int index ) const { return getCompoundShape()->getChildShape( index ); }
	
	inline btTransform& getChildTransform( int index ) { return getChildTransform( index ); }
	inline const btTransform& getChildTransform( int index ) const { return getCompoundShape()->getChildTransform( index ); }
	
	
	btCompoundShapeRef getCompoundShape() { return std::dynamic_pointer_cast<btCompoundShape>(mCollisionShape); }
	const btCompoundShapeRef getCompoundShape() const { return std::dynamic_pointer_cast<btCompoundShape>(mCollisionShape); }
	
	
private:
	PhyObjCompoundShape( const Format &format );
	
	void init( const Format &format );
	
};
	
}
