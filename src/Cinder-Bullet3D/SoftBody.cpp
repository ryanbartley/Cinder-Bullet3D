//
//  SoftBody.cpp
//  SoftBodyDemo
//
//  Created by Ryan Bartley on 9/7/14.
//
//

#include "SoftBody.h"



using namespace std;
using namespace ci;

namespace bullet {
	
SoftBody::SoftBody( const Format &format )
	: mCollGroup( -1 ), mCollMask( -1 )
{
	
}
	
SoftBody::SoftBody( const btSoftBodyRef softBody )
: mSoftBody( softBody ), mCollGroup( -1 ), mCollMask( -1 )
{
	
}
	
std::array<btVector3, 8> getCubePoints( const vec3 &c, const vec3 &size )
{
	vec3 s = size * 0.5f;																			// origin points
	std::array<btVector3, 8> points = { btVector3(  c.x + 1.0f * s.x, c.y + 1.0f * s.y, c.z + 1.0f * s.z ),	// upper front right
									btVector3(  c.x + 1.0f * s.x, c.y - 1.0f * s.y, c.z + 1.0f * s.z ),	// lower front right
									btVector3(  c.x + 1.0f * s.x, c.y - 1.0f * s.y, c.z - 1.0f * s.z ),	// lower back right
									btVector3(  c.x + 1.0f * s.x, c.y + 1.0f * s.y, c.z - 1.0f * s.z ),	// upper back right
									btVector3(  c.x - 1.0f * s.x, c.y + 1.0f * s.y, c.z + 1.0f * s.z ),	// upper front left
									btVector3(  c.x - 1.0f * s.x, c.y - 1.0f * s.y, c.z + 1.0f * s.z ),	// lower front left
									btVector3(  c.x - 1.0f * s.x, c.y - 1.0f * s.y, c.z - 1.0f * s.z ),	// lower back left
									btVector3(  c.x - 1.0f * s.x, c.y + 1.0f * s.y, c.z - 1.0f * s.z ) };	// upper back left
	return points;
}

btSoftBodyRef createFromTriMesh( btSoftBodyWorldInfo& worldInfo, const btScalar* vertices, const uint32_t* triangles, int ntriangles, bool randomizeConstraints )
{
	uint32_t maxidx = 0;
	int i, j, ni;
	
	for( i=0, ni = ntriangles * 3; i < ni; ++i ) {
		maxidx = ci::max( triangles[i], maxidx );
	}
	++maxidx;
	btAlignedObjectArray<bool>		chks;
	btAlignedObjectArray<btVector3>	vtx;
	chks.resize( maxidx * maxidx, false );
	vtx.resize( maxidx );
	for( i = 0, j = 0, ni = maxidx * 3; i < ni ; ++j, i += 3 ) {
		vtx[j] = btVector3( vertices[i], vertices[i+1], vertices[i+2] );
	}
	
	auto psb = btSoftBodyRef( new btSoftBody( &worldInfo, vtx.size(), &vtx[0], 0 ) );
	
	for( i = 0, ni = ntriangles * 3; i < ni; i += 3 ) {
		const uint32_t idx[] = { triangles[i], triangles[i+1], triangles[i+2] };
#define IDX(_x_,_y_) ( (_y_) * maxidx + (_x_) )
			for(int j=2,k=0;k<3;j=k++)
			{
				if(!chks[IDX(idx[j],idx[k])])
				{
					chks[IDX(idx[j],idx[k])]=true;
					chks[IDX(idx[k],idx[j])]=true;
					psb->appendLink(idx[j],idx[k]);
				}
			}
#undef IDX
		psb->appendFace(idx[0],idx[1],idx[2]);
	}
	
	if ( randomizeConstraints ) {
		psb->randomizeConstraints();
	}
	
	return( psb );
}
	
SoftBodyRef SoftBody::create( const ci::TriMeshRef &mesh, bullet::SoftBody::Format &format )
{
//	auto con = createConvexHull( mesh );
	auto body = btSoftBodyRef( btSoftBodyHelpers::CreateEllipsoid( format.mInfo, btVector3( 0, 0, 0 ),  btVector3( 1.0, 1.0f, 1.0f ), 10 ) );
//	auto body = createFromTriMesh( format.mInfo, &cube.data()->x, mesh->getIndices().data(), mesh->getNumTriangles(), true );
	body->generateBendingConstraints(2);
	body->generateBendingConstraints(2);
	body->m_cfg.piterations=2;
	body->m_cfg.collisions|=btSoftBody::fCollision::VF_SS;
	body->randomizeConstraints();
	btMatrix3x3	m;
	m.setEulerZYX(0,0,0);
	body->transform(btTransform(m,btVector3(0, 0, 0)));
//	body->scale(btVector3(2,2,2));
	body->setTotalMass(50,true);
	return SoftBodyRef( new SoftBody( body ) );
}
	
}