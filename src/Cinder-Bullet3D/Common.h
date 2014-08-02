//
//  Common.h
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/25/13.
//
//

#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "btSoftRigidDynamicsWorld.h"

#include "cinder/Log.h"

#include "Conversions.hpp"

//! Useful for creating collision groups and Masks
#define BIT(x) (1<<(x))

class btHeightfieldTerrainShape;

namespace bullet {

class Context;
typedef std::shared_ptr<Context> ContextRef;
	
typedef std::shared_ptr<btCollisionShape> btCollisionShapeRef;
typedef std::shared_ptr<class OpenGLMotionState> OpenGLMotionStateRef;
typedef std::shared_ptr<btRigidBody> btRigidBodyRef;
typedef std::shared_ptr<btDiscreteDynamicsWorld> btDiscreteDynamicsWorldRef;
typedef std::shared_ptr<btDynamicsWorld> btDynamicsWorldRef;
typedef std::shared_ptr<btCollisionObject> btCollisionObjectRef;
typedef std::shared_ptr<btBoxShape> btBoxShapeRef;
typedef std::shared_ptr<btConeShape> btConeShapeRef;
typedef std::shared_ptr<btCapsuleShape> btCapsuleShapeRef;
typedef std::shared_ptr<btCylinderShape> btCylinderShapeRef;
typedef std::shared_ptr<btSphereShape> btSphereShapeRef;
typedef std::shared_ptr<btStaticPlaneShape> btStaticPlaneShapeRef;
typedef std::shared_ptr<btMultiSphereShape> btMultiSphereShapeRef;
typedef std::shared_ptr<btCompoundShape> btCompoundShapeRef;
typedef std::shared_ptr<btConvexHullShape> btConvexHullShapeRef;
typedef std::shared_ptr<btHeightfieldTerrainShape> btHeightfieldTerrainShapeRef;
	
typedef std::shared_ptr<class Context>					ContextRef;
typedef std::shared_ptr<class PhysicsDebugRenderable>	DebugRendererRef;
typedef std::shared_ptr<class PhyObjBase>				PhyObjBaseRef;
class PhyObjPrimitive;
typedef std::shared_ptr<PhyObjPrimitive>				PhyObjPrimitiveRef;
class PhyObjConvexHull;
typedef std::shared_ptr<PhyObjConvexHull>				PhyObjConvexHullRef;
class PhyObjCompoundShape;
typedef std::shared_ptr<PhyObjCompoundShape>			PhyObjCompoundShapeRef;
class PhyObjHeightfieldTerrain;
typedef std::shared_ptr<PhyObjHeightfieldTerrain>		PhyObjHeightfieldTerrainRef;
	
// Pointer to the main Bullet Context
Context* Context();
	
btBoxShapeRef createBoxShape( const ci::Vec3f &halfExtents );
btConeShapeRef createConeShape( btScalar radius, btScalar height );
btCapsuleShapeRef createCapsuleShape( btScalar radius, btScalar height );
btCylinderShapeRef createCylinderShape( const ci::Vec3f &halfExtents );
btSphereShapeRef createSphereShape( btScalar radius );
btStaticPlaneShapeRef createStaticPlaneShape( const ci::Vec3f &normal, btScalar offset );
btMultiSphereShapeRef createMultiSphereShape( const std::vector<ci::Vec3f> &positions, const std::vector<btScalar> &radii );
btMultiSphereShapeRef createMultiSphereShape( const std::vector<btVector3> &positions, const std::vector<btScalar> &radii );
	
enum PhyObjType {
	BOX = 0,
	SPHERE,
	CONE,
	CYLINDER,
	CAPSULE,
	MULTI_SPHERE,
	CONVEX_TRIANGLE_MESH,
	BVH_TRIANGLE_MESH,
	CONVEX_HULL,
	HEIGHTFIELD,
	STATIC_PLANE,
	COMPOUND_SHAPE,
	NO_SHAPE
};
	
static PhyObjType getPhyObjType( const char * name )
{
	if( strcmp( name, "Box" ) == 0 ) {
		return PhyObjType::BOX;
	}
	else if( strcmp( name, "SPHERE" ) == 0 ) {
		return PhyObjType::SPHERE;
	}
	else if( strcmp( name, "Cone" ) == 0 ) {
		return PhyObjType::CONE;
	}
	else if( strcmp( name, "CylinderY" ) == 0 ) {
		return PhyObjType::CYLINDER;
	}
	else if( strcmp( name, "CapsuleShape" ) == 0) {
		return PhyObjType::CAPSULE;
	}
	else if( strcmp( name, "MultiSphere" ) == 0 ) {
		return PhyObjType::MULTI_SPHERE;
	}
	else if( strcmp( name, "ConvexTrimesh" ) == 0 ) {
		return PhyObjType::CONVEX_TRIANGLE_MESH;
	}
	else if( strcmp( name, "BVHTRIANGLEMESH" ) == 0 ) {
		return PhyObjType::BVH_TRIANGLE_MESH;
	}
	else if( strcmp( name, "Convex" ) == 0 ) {
		return PhyObjType::CONVEX_HULL;
	}
	else if( strcmp( name, "HEIGHTFIELD" ) == 0 ) {
		return PhyObjType::HEIGHTFIELD;
	}
	else if( strcmp( name, "STATICPLANE" ) == 0 ) {
		return PhyObjType::STATIC_PLANE;
	}
	else if( strcmp( name, "Compound" ) == 0 ) {
		return PhyObjType::COMPOUND_SHAPE;
	}
	else {
		CI_LOG_W("getPhyObjType found no shape");
		return PhyObjType::NO_SHAPE;
	}
}

static const char * getPhyObjType( PhyObjType type )
{
	switch (type) {
		case PhyObjType::BOX:
			return "Box";
		case PhyObjType::SPHERE:
			return "SPHERE";
		case PhyObjType::CONE:
			return "Cone";
		case PhyObjType::CYLINDER:
			return "CylinderY";
		case PhyObjType::CAPSULE:
			return "CapsuleShape";
		case PhyObjType::MULTI_SPHERE:
			return "MultiSphere";
		case PhyObjType::CONVEX_TRIANGLE_MESH:
			return "ConvexTrimesh";
		case PhyObjType::BVH_TRIANGLE_MESH:
			return "BVHTRIANGLEMESH";
		case PhyObjType::CONVEX_HULL:
			return "Convex";
		case PhyObjType::HEIGHTFIELD:
			return "HEIGHTFIELD";
		case PhyObjType::STATIC_PLANE:
			return "STATICPLANE";
		case PhyObjType::COMPOUND_SHAPE:
			return "Compound";
		default: {
			CI_LOG_W("getPhyObjType found no shape");
			return "";
		}
	}
}

	
}