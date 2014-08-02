//
//  PrimitivePhyObj.h
//  Prototype_3
//
//  Created by Ryan Bartley on 3/25/14.
//
//

#pragma once

#include "PhyObjBase.h"

namespace bullet {
	
typedef std::shared_ptr<class PhyObjPrimitive> PhyObjPrimitiveRef;
	
class PhyObjPrimitive : public PhyObjBase {
public:
	
	struct Format : public PhyObjBase::Format {
		
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
		
		friend class PhyObjPrimitive;
	};
	
	//! Helper function that creates a Box Shape using halfextents, i.e. length in x, y, z, for the collision shape at initialPosition and initialRotation with mass. From the Bullet Wiki, A cuboid shape, the length in each dimension can be chosen arbitrarily.
	static PhyObjPrimitiveRef createBox( const ci::Vec3f &halfExtents, btScalar mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation = ci::Quatf::identity(), bool kinematic = false );
	//! Helper function that creates a Capsule Shape using radius and height for the collision shape at initialPosition and initialRotation with mass. From the Bullet Wiki, An elongated sphere, useful for modelling humanoid characters as it abstracts from details like limbs and gait, but can still skip up stairs so long as the steps are not too high.
	static PhyObjPrimitiveRef createCapsule( btScalar radius, btScalar height, btScalar mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation = ci::Quatf::identity(), bool kinematic = false  );
	//! Helper function that creates a Cone Shape using radius and height for the collision shape at initialPosition and initialRotation with mass. From the Bullet Wiki, A cone pointing along the given axis with the point at (0,0,0).
	static PhyObjPrimitiveRef createCone( btScalar radius, btScalar height, btScalar mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation = ci::Quatf::identity(), bool kinematic = false );
	//! Helper function that creates a Cylinder Shape using halfextents, i.e. radius in x, y, z, for the collision shape at initialPosition and initialRotation with mass. From the Bullet Wiki, A cylinder much like the box shape.
	static PhyObjPrimitiveRef createCylinder( const ci::Vec3f &halfExtents, btScalar mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation = ci::Quatf::identity(), bool kinematic = false );
	//! Helper function that creates a Sphere Shape of radius  at initialPosition and initialRotation with mass. From Bullet Wiki, A very fast and simple shape.
	static PhyObjPrimitiveRef createSphere( btScalar radius, btScalar mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation = ci::Quatf::identity(), bool kinematic = false );
	//! Helper function that creates a Static Plane Shape using normal, i.e. Vec3f( 0, 1, 0) would make y the normal, and offset would be how many units to move along the normal for the collision shape at initialPosition and initialRotation with mass at 0.0f. From The Bullet Wiki, A static plane that is solid to infinity on one side. Several of these can be used to confine a convex space in a manner that completely prevents tunneling to the outside. The plane itself is specified with a normal and distance as is standard in mathematics.
	static PhyObjPrimitiveRef createStaticPlane( const ci::Vec3f &normal, btScalar offset );
	//! Helper function that creates a MultiSphere Shape using positions and radii for the collision shape at initialPosition and initialRotation with mass. From The Bullet Wiki, A convex shape defined by spheres. Useful for representing bevelled boxes or other shapes without sharp corners. Also since the spheres can be independently scaled in each of the 3 dimensions, a btMultiSphereShape containing just one sphere can be useful to create ellipsoids (squashed spheres), which is not possible with btSphereShape.
	static PhyObjPrimitiveRef createMultiSphere( const std::vector<ci::Vec3f> &positions, const std::vector<btScalar> &radii, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation = ci::Quatf::identity(), bool kinematic = false );
	//! Creates a PhyObjPrimitiveRef based on the format.
	static PhyObjPrimitiveRef create( const Format &format );
	
private:
	PhyObjPrimitive( const Format &format );
	
	void init( const Format &format );
	
};
	
}
