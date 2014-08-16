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
	
namespace drawableHelpers {
	
struct HeightFieldData {
	ci::Vec3f mPosition;
	ci::Vec3f mNormal;
	ci::Vec2f mTexCoord;
};
	
ci::gl::VboMeshRef getDrawableHeightField( const Channel32f *heightData )
{
	std::vector<uint32_t>	cIndices;
	
	int32_t height	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	std::vector<HeightFieldData> cVertices( height * width );
	
	float halfHeight	= (float)height * 0.5f - 0.5f;
	float halfWidth		= (float)width * 0.5f - 0.5f;
	
	auto vertIt = cVertices.begin();
	
	for ( int32_t y = 0; y < height; y++ ) {
		for ( int32_t x = 0; x < width; x++ ) {
			float value = heightData->getValue( Vec2i( x, y ) );
			
			vertIt->mPosition.set( (float)x - halfWidth, value, (float)y - halfHeight );
			
			vertIt->mTexCoord.set( (float)x / (float)width, (float)y / (float)height );
			
			int32_t xn = x + 1 >= width ? 0 : 1;
			int32_t yn = y + 1 >= height ? 0 : 1;
			cIndices.push_back( x + height * y );
			cIndices.push_back( ( x + xn ) + height * y);
			cIndices.push_back( ( x + xn ) + height * ( y + yn ) );
			cIndices.push_back( x + height * ( y + yn ) );
			cIndices.push_back( ( x + xn ) + height * ( y + yn ) );
			cIndices.push_back( x + height * y );
			
			++vertIt;
		}
	}
	
	for ( int32_t y = 0; y < height - 1; y++ ) {
		for ( int32_t x = 0; x < width - 1; x++ ) {
			Vec3f vert0 = cVertices[ cIndices[ ( x + height * y ) * 6 ] ].mPosition;
			Vec3f vert1 = cVertices[ cIndices[ ( ( x + 1 ) + height * y ) * 6 ] ].mPosition;
			Vec3f vert2 = cVertices[ cIndices[ ( ( x + 1 ) + height * ( y + 1 ) ) * 6 ] ].mPosition;
			cVertices[ x + height * y ].mNormal.set( ( vert1 - vert0 ).cross( vert1 - vert2 ).normalized() );
		}
	}
	
	auto gVertices  = gl::Vbo::create( GL_ARRAY_BUFFER, cVertices );
	auto gIndices	= gl::Vbo::create( GL_ELEMENT_ARRAY_BUFFER, cIndices );
	
	geom::BufferLayout bufferLayout;
	bufferLayout.append( geom::Attrib::POSITION, geom::DataType::FLOAT, 3, sizeof(HeightFieldData), 0 );
	bufferLayout.append( geom::Attrib::NORMAL, geom::DataType::FLOAT, 3, sizeof(HeightFieldData), 3 );
	bufferLayout.append( geom::Attrib::TEX_COORD_0, geom::DataType::FLOAT, 3, sizeof(HeightFieldData), 6 );
	
	auto pair = std::make_pair( bufferLayout, gVertices );
	
	auto ret = gl::VboMesh::create( cVertices.size(), geom::Primitive::TRIANGLES, { pair }, cIndices.size(), GL_UNSIGNED_INT, gIndices );
	
	return ret;
}
	
}
	
}
