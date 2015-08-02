//
//  system/Common.h
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/25/13.
//
//

#pragma once

#include <map>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "cinder/Log.h"
#include "cinder/Channel.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/gl.h"

//! Useful for creating collision groups and Masks
#define BIT(x) (1<<(x))

class btHeightfieldTerrainShape;

namespace bullet {

class Context;
using ContextRef = std::shared_ptr<Context>;
	
using btCollisionShapeRef = std::shared_ptr<btCollisionShape>;
using OpenGLMotionStateRef = std::shared_ptr<class OpenGLMotionState>;
using btRigidBodyRef = std::shared_ptr<btRigidBody>;
using btSoftBodyRef = std::shared_ptr<class btSoftBody>;
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
using UniformScalingShapeRef = std::shared_ptr<btUniformScalingShape>;
	
using DebugRendererRef = std::shared_ptr<class PhysicsDebugRenderable>;
using RigidBodyRef = std::shared_ptr<class RigidBody>;
using SoftBodyRef = std::shared_ptr<class SoftBody>;
	
// Pointer to the main Bullet Context
Context* Context();

btQuaternion toBullet( const ci::quat & cinderQuat );
ci::quat fromBullet( const btQuaternion & bulletQuaternion );
btVector3 toBullet( const ci::vec3 & cinderVec );
ci::vec3 fromBullet( const btVector3 & bulletVec );
ci::mat4 fromBullet( const btTransform &bulletTrans );
btTransform toBullet( const ci::mat4 &cinderMat4 );
ci::mat3 fromBullet( const btMatrix3x3 &bulletMat3 );
btMatrix3x3 toBullet( const ci::mat3 &cinderMat3 );
	
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
ConvexHullShapeRef createConvexHull( const ci::TriMeshRef &mesh );
HeightfieldTerrainShapeRef createHeightfieldShape( const ci::Channel32f *heightData,
													float minHeight,
													float maxHeight,
													ci::vec3 scale = ci::vec3( 1.0f ) );
UniformScalingShapeRef createUniformScalingShape( const btCollisionShapeRef &shape, float uniformScalingFactor );
	
namespace drawableHelpers {
	
enum class SoftBodyDrawType {
	POINTS,
	LINES,
	TRIANGLES
};
ci::gl::VboMeshRef getDrawableHeightfield( const ci::Channel32f *heightData );
ci::gl::VboMeshRef getDrawablePlane( const StaticPlaneShapeRef &plane );
//! Creates a VboMesh from a SoftBody. Attaches an index, position (geom::Attrib::POSITION) and normal(geom::Attrib::NORMAL) buffer interleaved based on the value of \a interleaved, which defaults to false.
ci::gl::VboMeshRef getDrawableSoftBody( const SoftBodyRef &softBody, bool interleaved = true );
//! Updates a VboMesh's Positions and Normals with the values from the softBody. Expects that Positions will be contained in the buffer associated with geom::Attrib::POSITION and Normals will be contained in the buffer associated with geom::Attrib::NORMALS. Also, expects that you're using an Index Vbo.
void updateVboMesh( ci::gl::VboMeshRef &mesh, const SoftBodyRef &softBody );
//! Draws
void drawSoftBody( const SoftBodyRef &softBody, SoftBodyDrawType type = SoftBodyDrawType::TRIANGLES );
	
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
	UNIFORM_SCALING_SHAPE,
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
	else if( strcmp( name, "UniformScalingShape") == 0 ) {
		return PhyObjType::UNIFORM_SCALING_SHAPE;
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
		case PhyObjType::UNIFORM_SCALING_SHAPE:
			return "UniformScalingShape";
		default: {
			CI_LOG_W("getPhyObjType found no shape");
			return "";
		}
	}
}
	
}

namespace bt = bullet;