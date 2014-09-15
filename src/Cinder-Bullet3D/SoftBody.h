//
//  SoftBody.h
//  SoftBodyDemo
//
//  Created by Ryan Bartley on 9/7/14.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "BulletSoftBody/btSoftBody.h"
#include "cinder/TriMesh.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

namespace bullet {

class SoftBody {
public:
	struct Format {
		
		Format& worldInfo( const btSoftBodyWorldInfo &info ) { mInfo = info; return *this; }
		Format& config( const btSoftBody::Config &config ) { mConfig = config; return *this; }
		
	private:
		btSoftBodyWorldInfo mInfo;
		btSoftBody::Config mConfig;
		friend class SoftBody;
	};
	
	static inline SoftBodyRef create( const ci::TriMeshRef &mesh, Format &format );
	//! Returns a SoftBodyRef ellipsoid. Simply wraps btSoftBodyHelpers::CreateEllipsoid...
	static inline SoftBodyRef createEllipsoid( btSoftBodyWorldInfo &worldInfo, const ci::vec3 &center, const ci::vec3 &radius, uint16_t resolution );
	//! Returns a SoftBodyRef from a Convex Hull. Simply wraps btSoftBodyHelpers::CreateFromConvexHull...
	static inline SoftBodyRef createFromConvexHull( btSoftBodyWorldInfo &worldInfo, const ConvexHullShapeRef &convexHull );
	//! Returns a SoftBodyRef rope with numPoints defined by \a resolution from \a from to \a to. \a fixeds defines a
	//! bitfield to make either side fixed in space (0.0 for mass), for \a from to be fixed pass fixeds = 1 >> 0 (or 1),
	//! for \a to to be fixed pass fixeds = 1 >> 1 (or 2), for both to be fixed pass (1 >> 1 | 1 >> 0) or 3.
	static inline SoftBodyRef createRope( btSoftBodyWorldInfo &worldInfo, const ci::vec3 &from, const ci::vec3 &to, uint32_t resolution, int32_t fixeds );
	//! Returns a SoftBodyRef patch (plane) with  \a resx as the resolution of x, and \a resy as the resolution of y.
	//! Also, fixeds is a bitfield of the corners you want fixed, for instance 1 >> 0 would be corner00, etc. \a gendiags is a flag on whether to create
	//! diagonal links between the soft body.
	static inline SoftBodyRef createPatch( btSoftBodyWorldInfo& worldInfo,const ci::vec3 &corner00, const ci::vec3 &corner10, const ci::vec3 &corner01, const ci::vec3 &corner11, uint32_t resx, uint32_t resy, int32_t fixeds, bool gendiags );
	//! Returns a SoftBodyRef patch (plane) with  \a resx as the resolution of x, and \a resy as the resolution of y.
	//! Also, fixeds is a bitfield of the corners you want fixed, for instance 1 >> 0 would be corner00, etc. \a gendiags is a flag on whether to create
	//! diagonal links between the soft body. \a tex_coords will be where the texCoords of this patch will be placed. Checks to make sure there is enough
	//! room in tex_coords for all the coordinates and resizes it if not.
	static inline SoftBodyRef createPatchUV(btSoftBodyWorldInfo& worldInfo, const ci::vec3& corner00, const ci::vec3& corner10, const ci::vec3& corner01, const ci::vec3& corner11, uint32_t resx, uint32_t resy, int fixeds, bool gendiags, std::vector<ci::vec2>& tex_coords );
	
	//! Returns a reference to the underlying btSoftBodyRef.
	btSoftBodyRef& getSoftBody() { return mSoftBody; }
	//! Returns a const reference to the underlying btSoftBodyRef.
	const btSoftBodyRef& getSoftBody() const { return mSoftBody; }
	
	//! Adds \a force to all the points in the softBody.
	inline void addForce( const ci::vec3 &force ) { mSoftBody->addForce( toBullet( force ) ); }
	//! Transforms the entire softBody using \a transform.
	inline void transform( const ci::mat4 &transform ) { mSoftBody->transform( toBullet( transform ) ); }
	//! Rotates the entire softBody using \a rotation.
	inline void rotate( const ci::quat &rotation ) { mSoftBody->rotate( toBullet( rotation ) ); }
	//! Translates the entire softBody using \a translation.
	inline void translate( const ci::vec3 &translation ) { mSoftBody->translate( toBullet( translation ) ); }
	
	//! Sets the total mass of this softBody.
	inline void setTotalMass( float mass, bool fromFaces = false ) { mSoftBody->setTotalMass( mass, fromFaces ); }
	//! Sets the pose. \a bVolume flags whether to use btSoftBody::Config::kVC to conserve the
	//! volume of the object in the state it's in at the time of calling this function. \a bFrame
	//! flags whether to use btSoftBody::Config::kMT -> (Pose matching coefficient [0,1]) to match
	//! the objects current pose.
	inline void setPose( bool bVolume, bool bFrame ) { mSoftBody->setPose( bVolume, bFrame ); }
	
	//! Returns a reference to the node array. The node array represents the vertices of the softBody.
	//! For instance, mSoftBody->getNodes()[0].m_x -> position of this node, and
	//! mSoftBody->getNodes()[0].m_n -> normal of this node.
	inline btSoftBody::tNodeArray& getNodes() { return mSoftBody->m_nodes; }
	//! Returns a reference to the link array. The link array represents the connections between nodes in the softBody.
	inline btSoftBody::tLinkArray& getLinks() { return mSoftBody->m_links; }
	//! Returns a reference to the face array. The face array represents the faces produced by a triangle of nodes.
	inline btSoftBody::tFaceArray& getFaces() { return mSoftBody->m_faces; }
	//! Returns a reference to the config structure used inside this softBody.
	inline btSoftBody::Config& getConfig() { return mSoftBody->m_cfg; }
	//! Returns a pointer to the material located at \a index inside the materials array. Note there may
	//! be more than one material for the softBody.
	inline btSoftBody::Material* getMaterial( int index ) { return mSoftBody->m_materials[index]; }
	//! Returns the collision group this softBody is using.
	inline int16_t getCollisionGroup() { return mCollGroup; }
	//! Returns the collision mask this softBody is using.
	inline int16_t getCollisionMask() { return mCollMask; }
	//! Note: DON'T USE YET. Sets the collision Group.
	void setCollisionGroup( int16_t collGroup );
	//! Note: DON'T USE YET. Sets the collision Mask.
	void setCollisionMask( int16_t collMask );
	
	
	inline void generateBendingConstraints( int distance, btSoftBody::Material *material = nullptr ) { mSoftBody->generateBendingConstraints( distance, material ); }
	
	
protected:
	SoftBody( const Format &format );
	SoftBody( const btSoftBodyRef softBody );
	
	btSoftBodyRef	mSoftBody;
	int16_t			mCollGroup, mCollMask;
};
	
SoftBodyRef SoftBody::createEllipsoid( btSoftBodyWorldInfo &worldInfo, const ci::vec3 &center, const ci::vec3 &radius, uint16_t resolution )
{
	btSoftBodyRef ref( btSoftBodyHelpers::CreateEllipsoid( worldInfo, toBullet( center ), toBullet( radius ), resolution ) );
	return SoftBodyRef( new SoftBody( ref ) );
}
	
SoftBodyRef SoftBody::createFromConvexHull( btSoftBodyWorldInfo &worldInfo, const ConvexHullShapeRef &convexHull )
{
	btSoftBodyRef ref( btSoftBodyHelpers::CreateFromConvexHull( worldInfo, convexHull->getPoints(), convexHull->getNumPoints() ) );
	return SoftBodyRef( new SoftBody( ref ) );
}
	
SoftBodyRef SoftBody::createRope( btSoftBodyWorldInfo &worldInfo, const ci::vec3 &from, const ci::vec3 &to, uint32_t resolution, int32_t fixeds )
{
	btSoftBodyRef ref( btSoftBodyHelpers::CreateRope( worldInfo, toBullet( from ), toBullet( to ), resolution, fixeds ) );
	return SoftBodyRef( new SoftBody( ref ) );
}
	
SoftBodyRef SoftBody::createPatch( btSoftBodyWorldInfo &worldInfo, const ci::vec3 &corner00, const ci::vec3 &corner10, const ci::vec3 &corner01, const ci::vec3 &corner11, uint32_t resx, uint32_t resy, int32_t fixeds, bool gendiags )
{
	btSoftBodyRef ref( btSoftBodyHelpers::CreatePatch( worldInfo, toBullet( corner00 ), toBullet( corner10 ), toBullet( corner01 ), toBullet( corner11 ), resx, resy, fixeds, gendiags ) );
	return SoftBodyRef( new SoftBody( ref ) );

}
	
SoftBodyRef SoftBody::createPatchUV(btSoftBodyWorldInfo& worldInfo, const ci::vec3& corner00, const ci::vec3& corner10, const ci::vec3& corner01, const ci::vec3& corner11, uint32_t resx, uint32_t resy, int fixeds, bool gendiags, std::vector<ci::vec2>& tex_coords )
{
	const int numTexCoords = ( ( resx - 1 ) * ( resy - 1 ) * 12 ) / 2; // Notation from Bullet / 2 because of vec2
	if( tex_coords.size() < numTexCoords ) {
		tex_coords.resize( numTexCoords );
	}
	btSoftBodyRef ref( btSoftBodyHelpers::CreatePatchUV( worldInfo, toBullet( corner00 ), toBullet( corner10 ), toBullet( corner01 ), toBullet( corner11 ), resx, resy, fixeds, gendiags, reinterpret_cast<float*>( tex_coords.data() ) ) );
	return SoftBodyRef( new SoftBody( ref ) );
}

}