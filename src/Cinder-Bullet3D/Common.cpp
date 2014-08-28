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
	
CompoundShapeRef createCompoundShapeRef( const ShapesAndOffsets &shapesAndOffsets )
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
	std::vector<uint32_t>	cIndices;
	
	int32_t height	= heightData->getHeight();
	int32_t width	= heightData->getWidth();
	
	std::vector<HeightfieldData> cVertices( height * width );
	
	float halfHeight	= (float)height * 0.5f - 0.5f;
	float halfWidth		= (float)width * 0.5f - 0.5f;
	
	auto vertIt = cVertices.begin();

	for ( int32_t y = 0; y < height; y++ ) {
		cout << "-------------------------------------------" << endl;
		for ( int32_t x = 0; x < width; x++ ) {
			cout << endl;
			float value = heightData->getValue( ivec2( x, y ) );
			
			vertIt->mPosition = vec3( (float)x - halfWidth, value, (float)y - halfHeight );
			vertIt->mTexCoord = vec2( (float)x / (float)width, (float)y / (float)height );
			
			int32_t xn = x + 1 >= width ? 0 : 1;
			int32_t yn = y + 1 >= height ? 0 : 1;
			//  0-----3
			//  |     |
			//  |     |
			//	1_____2
			cIndices.push_back( x + width * y );
			cIndices.push_back( ( x + xn ) + width * y );
			cIndices.push_back( ( x ) + width * ( y + yn ) );
			
			cIndices.push_back( x + width * y );
			cIndices.push_back( x + width * ( y + yn ) );
			cIndices.push_back( ( x + xn ) + width * ( y + yn ) );
			
			++vertIt;
		}
	}
	
	for ( int32_t y = 0; y < height - 1; y++ ) {
		for ( int32_t x = 0; x < width - 1; x++ ) {
			vec3 vert0 = cVertices[ cIndices[ ( x + height * y ) * 6 ] ].mPosition;
			vec3 vert1 = cVertices[ cIndices[ ( ( x + 1 ) + height * y ) * 6 ] ].mPosition;
			vec3 vert2 = cVertices[ cIndices[ ( ( x + 1 ) + height * ( y + 1 ) ) * 6 ] ].mPosition;
			cVertices[ x + height * y ].mNormal = normalize( cross( ( vert1 - vert0 ), ( vert1 - vert2 ) ) );
		}
	}
	
	auto gVertices  = gl::Vbo::create( GL_ARRAY_BUFFER, cVertices.size() * sizeof(HeightfieldData), cVertices.data(), GL_STATIC_DRAW );
	auto gIndices	= gl::Vbo::create( GL_ELEMENT_ARRAY_BUFFER, cIndices.size() * sizeof(uint32_t), cIndices.data(), GL_STATIC_DRAW );
	
	geom::BufferLayout bufferLayout;
	bufferLayout.append( geom::Attrib::POSITION,	geom::DataType::FLOAT, 3, sizeof(HeightfieldData), 0 );
	bufferLayout.append( geom::Attrib::NORMAL,		geom::DataType::FLOAT, 3, sizeof(HeightfieldData), 3 );
	bufferLayout.append( geom::Attrib::TEX_COORD_0, geom::DataType::FLOAT, 2, sizeof(HeightfieldData), 6 );
	
	auto pair = std::make_pair( bufferLayout, gVertices );
	
	auto ret = gl::VboMesh::create( cVertices.size(), GL_LINES, { pair }, cIndices.size(), GL_UNSIGNED_INT, gIndices );
	cout << ret->getGlPrimitive();
	return ret;
}
	
} // namespace drawableHelpers
	
} // namespace bullet
