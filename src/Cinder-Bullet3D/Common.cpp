//
//  Common.cpp
//  BulletTestFirstProgram
//
//  Created by Ryan Bartley on 12/30/13.
//
//

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/BulletContext.h"
#include "Cinder-Bullet3D/SoftBody.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/GeomIo.h"

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionShapes/btConvexShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletSoftBody/btSoftBody.h"


using namespace ci;
using namespace std;

namespace bullet {

class Context* Context() {
	return Context::getCurrent();
}

btQuaternion toBullet( const ci::quat & cinderQuat )
{
	btQuaternion bulletQuat( cinderQuat.x, cinderQuat.y, cinderQuat.z, cinderQuat.w );
	return bulletQuat;
}

ci::quat fromBullet( const btQuaternion & bulletQuaternion )
{
	ci::quat cinderQuat( bulletQuaternion.w(), bulletQuaternion.x(), bulletQuaternion.y(), bulletQuaternion.z() );
	return cinderQuat;
}

btVector3 toBullet( const ci::vec3 & cinderVec )
{
	btVector3 bulletVec( cinderVec.x, cinderVec.y, cinderVec.z );
	return bulletVec;
}

ci::vec3 fromBullet( const btVector3 & bulletVec )
{
	ci::vec3 cinderVec( bulletVec.x(), bulletVec.y(), bulletVec.z() );
	return cinderVec;
}

ci::mat4 fromBullet( const btTransform &bulletTrans )
{
	ATTRIBUTE_ALIGNED16( btScalar m[16] );
	bulletTrans.getOpenGLMatrix( m );
	return glm::make_mat4( m );
}

btTransform toBullet( const ci::mat4 &cinderMat4 )
{
	btTransform trans;
	trans.setFromOpenGLMatrix( &cinderMat4[0][0] );
	return trans;
}

ci::mat3 fromBullet( const btMatrix3x3 &bulletMat3 )
{
	ATTRIBUTE_ALIGNED16( btScalar m[12] );
	bulletMat3.getOpenGLSubMatrix( m );
	return glm::make_mat3( m );
}

btMatrix3x3 toBullet( const ci::mat3 &cinderMat3 )
{
	btMatrix3x3 trans;
	trans.setFromOpenGLSubMatrix( &cinderMat3[0][0] );
	return trans;
}

	
BoxShapeRef createBoxShape( const ci::vec3 &halfExtents )
{
	return BoxShapeRef( new btBoxShape( toBullet( halfExtents ) ) );
}
	
ConeShapeRef createConeShape( btScalar radius, btScalar height )
{
	return ConeShapeRef( new btConeShape( radius, height ) );
}
	
CapsuleShapeRef createCapsuleShape( btScalar radius, btScalar height )
{
	return CapsuleShapeRef( new btCapsuleShape( radius, height ) );
}
	
CylinderShapeRef createCylinderShape( const ci::vec3 &halExtents )
{
	return CylinderShapeRef( new btCylinderShape( toBullet( halExtents ) ) );
}
	
SphereShapeRef createSphereShape( btScalar radius )
{
	return SphereShapeRef( new btSphereShape( radius ) );
}
	
StaticPlaneShapeRef createStaticPlaneShape( const ci::vec3 &normal, btScalar offset )
{
	return StaticPlaneShapeRef( new btStaticPlaneShape( toBullet( normal ), offset ) );
}
	
MultiSphereShapeRef createMultiSphereShape( const std::vector<ci::vec3> &positions, const std::vector<btScalar> &radii )
{
	assert( positions.size() == radii.size() );
	std::vector<btVector3> bulletPositions(positions.size());
	int i = 0;
	auto positionIt = positions.begin();
	auto end = positions.end();
	for(; positionIt != end; ++positionIt ) {
		bulletPositions[i] = toBullet( *positionIt );
		++i;
	}
	return createMultiSphereShape( bulletPositions, radii );
}
	
MultiSphereShapeRef createMultiSphereShape( const std::vector<btVector3> &positions, const std::vector<btScalar> &radii )
{
	assert( positions.size() == radii.size() );
	return MultiSphereShapeRef( new btMultiSphereShape( positions.data(), radii.data(), positions.size() ) );
}
	
CompoundShapeRef createCompoundShape( const ShapesAndOffsets &shapesAndOffsets )
{
	CompoundShapeRef pCompound( new btCompoundShape() );
	
	auto shapeIt = shapesAndOffsets.begin();
	auto end = shapesAndOffsets.end();
	while(  shapeIt != end ) {
		pCompound->addChildShape( shapeIt->second, shapeIt->first.get() );
		++shapeIt;
	}
	
	return pCompound;
}
	
ConvexHullShapeRef createConvexHull( const ci::TriMeshRef &mesh )
{
	auto mMesh = mesh->getPositions<3>();
	std::vector<btVector3> mBulletMesh( mesh->getNumVertices() );
	
	auto bulletIt = mBulletMesh.begin();
	auto end = mBulletMesh.end();
	while( bulletIt != end ) {
		*bulletIt = toBullet( *mMesh++ );
		++bulletIt;
	}
	
	ConvexHullShapeRef convexShape( new btConvexHullShape( &mBulletMesh.data()->getX(), mBulletMesh.size() ) );
	convexShape->initializePolyhedralFeatures();
	
	return convexShape;
}
	
HeightfieldTerrainShapeRef createHeightfieldShape( const ci::Channel32f *heightData, float maxHeight, float minHeight, ci::vec3 scale )
{
	int32_t length	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	float heightScale = math<float>::abs( minHeight ) + math<float>::abs( maxHeight );
	HeightfieldTerrainShapeRef shape( new btHeightfieldTerrainShape( width, length, heightData->getData(), heightScale, minHeight, maxHeight, 1, PHY_FLOAT, false ) );
	shape->setLocalScaling( toBullet( scale ) );
	
	return shape;
}
	
UniformScalingShapeRef createUniformScalingShape( const btCollisionShapeRef &shape, float uniformScalingFactor )
{
	return UniformScalingShapeRef( new btUniformScalingShape( (btConvexShape*)shape.get(), uniformScalingFactor ) );
}
	
namespace drawableHelpers {
	
ci::gl::VboMeshRef getDrawableHeightfield( const Channel32f *heightData )
{
	int32_t depth	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	auto plane = TriMesh::create( geom::Plane().subdivisions( ivec2( width - 1, depth - 1 ) ).size( vec2( width - 1, depth - 1 ) ) );
	
	for( int z = 0; z < depth; ++z ) {
		for( int x = 0; x < width; ++x ) {
			auto & vert = plane->getPositions<3>()[z + depth * x];
			vert.y = heightData->getValue( ivec2( x, z ) );
		}
	}
	
	auto ret = gl::VboMesh::create( *plane );
	return ret;
}
	
ci::gl::VboMeshRef getDrawablePlane( const StaticPlaneShapeRef &plane )
{
	// TODO: Test this.
	auto normal = fromBullet( plane->getPlaneNormal() );
	auto constant = plane->getPlaneConstant();
	auto geomPlane = geom::Plane().subdivisions( ivec2( 1, 1 ) )
								.size( vec2( 1000, 1000 ) )
								.normal( normal )
								.origin( normal * constant );
	
	return ci::gl::VboMesh::create( geomPlane );
	
}
	
ci::gl::VboMeshRef getDrawableSoftBody( const SoftBodyRef &softBody, bool interleaved )
{
	if( interleaved ) {
		auto& softNodes = softBody->getNodes();
		std::vector<vec3> verts( softNodes.size() * 2 );
		auto vertIt = verts.begin();
		for( auto i = 0; i < softNodes.size(); ++i ) {
			*vertIt++ = fromBullet( softNodes[i].m_x );
			*vertIt++ = fromBullet( softNodes[i].m_n );
		}
		auto vbo = gl::Vbo::create( GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_DYNAMIC_DRAW );
		geom::BufferLayout layout;
		layout.append( geom::Attrib::POSITION, geom::DataType::FLOAT, 3, 2 * sizeof(vec3), 0 );
		layout.append( geom::Attrib::NORMAL, geom::DataType::FLOAT, 3, 2 * sizeof(vec3), 0 );
		auto& softFaces = softBody->getFaces();
		std::vector<uint32_t> indices( softFaces.size() * 3 );
		auto indexIt = indices.begin();
		for( int i = 0; i < softFaces.size(); ++i ) {
			auto node_0 = softFaces[i].m_n[0];
			auto node_1 = softFaces[i].m_n[1];
			auto node_2 = softFaces[i].m_n[2];
			*indexIt++ = uint32_t(node_0 - &softNodes[0]);
			*indexIt++ = uint32_t(node_1 - &softNodes[0]);
			*indexIt++ = uint32_t(node_2 - &softNodes[0]);
		}
		auto indexVbo = gl::Vbo::create( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW );
		auto vboMesh = gl::VboMesh::create( verts.size(), GL_TRIANGLES, { make_pair( layout, vbo ) }, indices.size(), GL_UNSIGNED_INT, indexVbo );
		return vboMesh;
	}
	else {
		auto& softNodes = softBody->getNodes();
		std::vector<vec3> pos( softNodes.size() );
		std::vector<vec3> norm( softNodes.size() );
		auto posIt = pos.begin();
		auto normIt = norm.begin();
		for( auto i = 0; i < softNodes.size(); ++i ) {
			*posIt++ = fromBullet( softNodes[i].m_x );
			*normIt++ = fromBullet( softNodes[i].m_n );
		}
		auto posVbo = gl::Vbo::create( GL_ARRAY_BUFFER, pos.size() * sizeof(vec3), pos.data(), GL_DYNAMIC_DRAW );
		auto normVbo = gl::Vbo::create( GL_ARRAY_BUFFER, norm.size() * sizeof(vec3), norm.data(), GL_DYNAMIC_DRAW );
		geom::BufferLayout posLayout;
		posLayout.append( geom::Attrib::POSITION, geom::DataType::FLOAT, 3, 2 * sizeof(vec3), 0 );
		geom::BufferLayout normLayout;
		normLayout.append( geom::Attrib::NORMAL, geom::DataType::FLOAT, 3, 2 * sizeof(vec3), 0 );
		auto& softFaces = softBody->getFaces();
		std::vector<uint32_t> indices( softFaces.size() * 3 );
		auto indexIt = indices.begin();
		for( int i = 0; i < softFaces.size(); ++i ) {
			auto node_0 = softFaces[i].m_n[0];
			auto node_1 = softFaces[i].m_n[1];
			auto node_2 = softFaces[i].m_n[2];
			*indexIt++ = uint32_t(node_0 - &softNodes[0]);
			*indexIt++ = uint32_t(node_1 - &softNodes[0]);
			*indexIt++ = uint32_t(node_2 - &softNodes[0]);
		}
		auto indexVbo = gl::Vbo::create( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW );
		std::vector<std::pair<cinder::geom::BufferLayout, cinder::gl::VboRef>> layout;
		layout.push_back( make_pair( posLayout, posVbo ) );
		layout.push_back( make_pair( normLayout, normVbo ) );
		auto vboMesh = gl::VboMesh::create( pos.size(), GL_TRIANGLES, layout, indices.size(), GL_UNSIGNED_INT, indexVbo );
		return vboMesh;
	}
}
	
void updateVboMesh( ci::gl::VboMeshRef &mesh, const SoftBodyRef &softBody )
{
	auto meshPos = mesh->mapAttrib3f( geom::POSITION );
	auto meshNorm = mesh->mapAttrib3f( geom::NORMAL );
	auto& softPos = softBody->getNodes();
	for( auto i = 0; i < softPos.size(); ++i ) {
		meshPos[i] = fromBullet( softPos[i].m_x );
		meshNorm[i] = fromBullet( softPos[i].m_n );
	}
	meshPos.unmap();
	meshNorm.unmap();
}
	
void drawSoftBodyLineImpl( const SoftBodyRef &softBody )
{
	auto links = softBody->getLinks();
	
	gl::begin( GL_LINES );
	for(int j = 0; j < links.size(); ++j )
	{
		auto node_0 = links[j].m_n[0];
		auto node_1 = links[j].m_n[1];
		gl::vertex( bt::fromBullet( node_0->m_x ) );
		gl::vertex( bt::fromBullet( node_1->m_x ) );
		
		/* Or if you need indices...      */
		//			const int indices[]={   int(node_0-&nodes[0]),
		//				int(node_1-&nodes[0])};
	}
	gl::end();
}
	
void drawSoftBodyTriImpl( const SoftBodyRef &softBody )
{
	auto faces = softBody->getFaces();
	gl::begin( GL_TRIANGLES );
	for(int j=0;j<faces.size();++j)
	{
		auto node_0 = faces[j].m_n[0];
		auto node_1 = faces[j].m_n[1];
		auto node_2 = faces[j].m_n[2];
		gl::vertex( bt::fromBullet( node_0->m_x ) );
		gl::vertex( bt::fromBullet( node_1->m_x ) );
		gl::vertex( bt::fromBullet( node_2->m_x ) );
	
		/* Or if you need indices...      */
		//			const int indices[]={   int(node_0-&nodes[0]),
		//				int(node_1-&nodes[0]),
		//				int(node_2-&nodes[0])};
	}
	gl::end();
}
	
void drawSoftBodyPointImpl( const SoftBodyRef &softBody )
{
	auto nodes = softBody->getNodes();
	/* Then, you can draw vertices...      */
	/* Node::m_x => position            */
	/* Node::m_n => normal (if meaningful)   */
	gl::begin( GL_POINTS );
	for(int j = 0; j < nodes.size(); ++j ) {
		gl::vertex( bt::fromBullet( nodes[j].m_x ) );
	}
	gl::end();
}
	
void drawSoftBody( const SoftBodyRef &softBody, SoftBodyDrawType type )
{
	switch ( type ) {
		case SoftBodyDrawType::POINTS:
			drawSoftBodyPointImpl( softBody );
		break;
		case SoftBodyDrawType::LINES:
			drawSoftBodyLineImpl( softBody );
			break;
		case SoftBodyDrawType::TRIANGLES:
			drawSoftBodyTriImpl( softBody );
		break;
		default:
		break;
	}
}
	
} // namespace drawableHelpers
	
} // namespace bullet
