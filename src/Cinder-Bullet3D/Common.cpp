//
//  Common.cpp
//  BulletTestFirstProgram
//
//  Created by Ryan Bartley on 12/30/13.
//
//

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/BulletContext.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/VboMesh.h"

#include "btHeightfieldTerrainShape.h"

using namespace ci;
using namespace std;

namespace bullet {

class Context* Context() {
	return Context::getCurrent();
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
	auto mMesh = mesh->getVertices<3>();
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
	
namespace drawableHelpers {
	
struct HeightfieldData {
	ci::vec3 mPosition;
	ci::vec3 mNormal = ci::vec3( 0.0f );
	ci::vec2 mTexCoord;
};
	
ci::gl::VboMeshRef getDrawableHeightfield( const Channel32f *heightData )
{
	int32_t depth	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	auto plane = TriMesh::create( geom::Plane().subdivisions( ivec2( width - 1, depth - 1 ) ).size( vec2( width - 1, depth - 1 ) ) );
	
	for( int z = 0; z < depth; ++z ) {
		for( int x = 0; x < width; ++x ) {
			auto & vert = plane->getVertices<3>()[z + depth * x];
			vert.y = heightData->getValue( ivec2( x, z ) );
		}
	}
	
	auto ret = gl::VboMesh::create( *plane );
	return ret;
}
	
} // namespace drawableHelpers
	
} // namespace bullet
