//
//  Common.cpp
//  BulletTestFirstProgram
//
//  Created by Ryan Bartley on 12/30/13.
//
//

#include "Physics/Common.h"
#include "Physics/BulletContext.h"

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
	
}
