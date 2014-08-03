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

#include "btHeightfieldTerrainShape.h"

using namespace cinder;

namespace bullet {

class Context* Context() {
	return Context::getCurrent();
}
	
btBoxShapeRef createBoxShape( const ci::Vec3f &halfExtents )
{
	return btBoxShapeRef( new btBoxShape( toBullet( halfExtents ) ) );
}
	
btConeShapeRef createConeShape( btScalar radius, btScalar height )
{
	return btConeShapeRef( new btConeShape( radius, height ) );
}
	
btCapsuleShapeRef createCapsuleShape( btScalar radius, btScalar height )
{
	return btCapsuleShapeRef( new btCapsuleShape( radius, height ) );
}
	
btCylinderShapeRef createCylinderShape( const ci::Vec3f &halExtents )
{
	return btCylinderShapeRef( new btCylinderShape( toBullet( halExtents ) ) );
}
	
btSphereShapeRef createSphereShape( btScalar radius )
{
	return btSphereShapeRef( new btSphereShape( radius ) );
}
	
btStaticPlaneShapeRef createStaticPlaneShape( const ci::Vec3f &normal, btScalar offset )
{
	return btStaticPlaneShapeRef( new btStaticPlaneShape( toBullet( normal ), offset ) );
}
	
btMultiSphereShapeRef createMultiSphereShape( const std::vector<ci::Vec3f> &positions, const std::vector<btScalar> &radii )
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
	
btMultiSphereShapeRef createMultiSphereShape( const std::vector<btVector3> &positions, const std::vector<btScalar> &radii )
{
	assert( positions.size() == radii.size() );
	return btMultiSphereShapeRef( new btMultiSphereShape( positions.data(), radii.data(), positions.size() ) );
}
	
btCompoundShapeRef createCompoundShapeRef( const ShapesAndOffsets &shapesAndOffsets )
{
	btCompoundShapeRef pCompound( new btCompoundShape() );
	
	auto shapeIt = shapesAndOffsets.begin();
	auto end = shapesAndOffsets.end();
	while(  shapeIt != end ) {
		auto offsetIt = shapeIt->second.begin();
		auto end = shapeIt->second.end();
		while( offsetIt != end ) {
			pCompound->addChildShape( *offsetIt, shapeIt->first );
			++offsetIt;
		}
		++shapeIt;
	}
	
	return pCompound;
}
	
btConvexHullShapeRef createConvexHull( const ci::TriMeshRef &mesh )
{
	auto mMesh = mesh->getVertices<3>();
	std::vector<btVector3> mBulletMesh( mesh->getNumVertices() );
	
	auto bulletIt = mBulletMesh.begin();
	auto end = mBulletMesh.end();
	while( bulletIt != end ) {
		*bulletIt = toBullet( *mMesh++ );
		++bulletIt;
	}
	
	btConvexHullShapeRef convexShape( new btConvexHullShape( &mBulletMesh.data()->getX(), mBulletMesh.size() ) );
	convexShape->initializePolyhedralFeatures();
	
	return convexShape;
}
	
btHeightfieldTerrainShapeRef createHeightFieldShape( const ci::Channel32f *heightData, float maxHeight, float minHeight, ci::Vec3f scale )
{
	int32_t length	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	float heightScale = math<float>::abs( minHeight ) + math<float>::abs( maxHeight );
	btHeightfieldTerrainShapeRef shape( new btHeightfieldTerrainShape( width, length, heightData->getData(), heightScale, minHeight, maxHeight, 1, PHY_FLOAT, false ) );
	shape->setLocalScaling( toBullet( scale ) );
	
	return shape;
}
	
}
