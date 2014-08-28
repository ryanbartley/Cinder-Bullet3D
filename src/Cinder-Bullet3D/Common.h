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

#include "Cinder-Bullet3D/Conversions.hpp"

//! Useful for creating collision groups and Masks
#define BIT(x) (1<<(x))

class btHeightfieldTerrainShape;


using TriMeshRef = std::shared_ptr<ci::TriMesh>;

namespace bullet {

class Context;
using ContextRef = std::shared_ptr<Context>;
	
using btCollisionShapeRef = std::shared_ptr<btCollisionShape>;
using OpenGLMotionStateRef = std::shared_ptr<class OpenGLMotionState>;
using btRigidBodyRef = std::shared_ptr<btRigidBody>;
using btDiscreteDynamicsWorldRef = std::shared_ptr<btDiscreteDynamicsWorld>;
using btDynamicsWorldRef = std::shared_ptr<btDynamicsWorld>;
using btCollisionObjectRef = std::shared_ptr<btCollisionObject>;
	
	
using ConstraintBaseRef = std::shared_ptr<class ConstraintBase>;
using btTypedConstraintRef = std::shared_ptr<class btTypedConstraint>;
using ConstraintConeTwistRef = std::shared_ptr<class ConstraintConeTwist>;
using btConeTwistConstraintRef = std::shared_ptr<class btConeTwistConstraint>;
using ConstraintHingeRef = std::shared_ptr<class ConstraintHinge>;
using btHingeConstraintRef = std::shared_ptr<class btHingeConstraint>;
using ConstraintPoint2PointRef = std::shared_ptr<class ConstraintPoint2Point>;
using btPoint2PointConstraintRef = std::shared_ptr<class btPoint2PointConstraint>;
using ConstraintGeneric6DofRef = std::shared_ptr<class ConstraintGeneric6Dof>;
using btGeneric6DofConstraintRef = std::shared_ptr<class btGeneric6DofConstraint>;
	

using BoxShapeRef = std::shared_ptr<btBoxShape>;
using ConeShapeRef = std::shared_ptr<btConeShape>;
using CapsuleShapeRef = std::shared_ptr<btCapsuleShape>;
using CylinderShapeRef = std::shared_ptr<btCylinderShape>;
using SphereShapeRef = std::shared_ptr<btSphereShape>;
using StaticPlaneShapeRef = std::shared_ptr<btStaticPlaneShape>;
using MultiSphereShapeRef = std::shared_ptr<btMultiSphereShape>;
using CompoundShapeRef = std::shared_ptr<btCompoundShape>;
using ConvexHullShapeRef = std::shared_ptr<btConvexHullShape>;
using HeightfieldTerrainShapeRef = std::shared_ptr<btHeightfieldTerrainShape>;
	
using DebugRendererRef = std::shared_ptr<class PhysicsDebugRenderable>;
using RigidBodyRef = std::shared_ptr<class RigidBody>;
	
// Pointer to the main Bullet Context
Context* Context();
	
BoxShapeRef createBoxShape( const ci::vec3 &halfExtents );
ConeShapeRef createConeShape( btScalar radius, btScalar height );
CapsuleShapeRef createCapsuleShape( btScalar radius, btScalar height );
CylinderShapeRef createCylinderShape( const ci::vec3 &halfExtents );
SphereShapeRef createSphereShape( btScalar radius );
StaticPlaneShapeRef createStaticPlaneShape( const ci::vec3 &normal, btScalar offset );
MultiSphereShapeRef createMultiSphereShape( const std::vector<ci::vec3> &positions, const std::vector<btScalar> &radii );
MultiSphereShapeRef createMultiSphereShape( const std::vector<btVector3> &positions, const std::vector<btScalar> &radii );
	
using ShapesAndOffsets = std::map<btCollisionShapeRef, btTransform>;
CompoundShapeRef createCompoundShape( const ShapesAndOffsets &shapesAndOffsets );
ConvexHullShapeRef createConvexHull( const TriMeshRef &mesh );
HeightfieldTerrainShapeRef createHeightfieldShape( const ci::Channel32f *heightData,
													float maxHeight,
													float minHeight,
													ci::vec3 scale = ci::vec3( 1.0f ) );
	
namespace drawableHelpers {
	ci::gl::VboMeshRef getDrawableHeightfield( const ci::Channel32f *heightData );
}

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

namespace bt = bullet;