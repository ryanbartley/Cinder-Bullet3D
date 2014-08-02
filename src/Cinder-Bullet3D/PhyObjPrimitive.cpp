//
//  PrimitivePhysicsObject.cpp
//  Prototype_3
//
//  Created by Ryan Bartley on 3/25/14.
//
//


#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/PhyObjPrimitive.h"
#include "Cinder-Bullet3D/PhyObjBase.h"

namespace bullet {
	
PhyObjPrimitive::PhyObjPrimitive( const Format &format )
{
	mType = getPhyObjType( format.mCollShape->getName() );
	init( format );
}
	
PhyObjPrimitiveRef PhyObjPrimitive::create( const Format &format )
{
	return PhyObjPrimitiveRef( new PhyObjPrimitive( format ) );
}
	
void PhyObjPrimitive::init( const Format &format )
{
	mType = getPhyObjType( format.mCollShape->getName() );
	PhyObjBase::init( format );
}
	
PhyObjPrimitiveRef PhyObjPrimitive::createBox( const ci::Vec3f &halfExtents, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation, bool kinematic )
{
	return create( Format::Format().collisionShape( createBoxShape( halfExtents ) ).initialPosition( initialPosition ).initialRotation( initialRotation ).mass( mass ).kinematic( kinematic ) );
}

PhyObjPrimitiveRef PhyObjPrimitive::createCapsule( btScalar radius, btScalar height, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation, bool kinematic  )
{
	return create( Format::Format().collisionShape( createCapsuleShape( radius, height ) ).initialPosition( initialPosition ).initialRotation( initialRotation ).mass( mass ).kinematic( kinematic ) );
}

PhyObjPrimitiveRef PhyObjPrimitive::createCone( btScalar radius, btScalar height, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation, bool kinematic )
{
	return create( Format::Format().collisionShape( createConeShape( radius, height ) ).initialPosition( initialPosition ).initialRotation( initialRotation ).mass( mass ).kinematic( kinematic ) );
}

PhyObjPrimitiveRef PhyObjPrimitive::createCylinder( const ci::Vec3f &halfExtents, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation, bool kinematic )
{
	return create( Format::Format().collisionShape( createCylinderShape( halfExtents ) ).initialPosition( initialPosition ).initialRotation( initialRotation ).mass( mass ).kinematic( kinematic ) );
}

PhyObjPrimitiveRef PhyObjPrimitive::createSphere( btScalar radius, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation, bool kinematic )
{
	return create( Format::Format().collisionShape( createSphereShape( radius ) ).initialPosition( initialPosition ).initialRotation( initialRotation ).mass( mass ).kinematic( kinematic ) );
}

PhyObjPrimitiveRef PhyObjPrimitive::createStaticPlane( const ci::Vec3f &normal, btScalar offset )
{
	return create( Format::Format().collisionShape( createStaticPlaneShape( normal, offset ) ) );
}

PhyObjPrimitiveRef PhyObjPrimitive::createMultiSphere( const std::vector<ci::Vec3f> &positions, const std::vector<btScalar> &radii, float mass, const ci::Vec3f &initialPosition, const ci::Quatf &initialRotation, bool kinematic )
{
	return create( Format::Format().collisionShape( createMultiSphereShape( positions, radii ) ).initialPosition( initialPosition ).initialRotation( initialRotation ).mass( mass ).kinematic( kinematic ) );
}
	
}