//
//  RagDoll.h
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/ConstraintBase.h"
#include "Cinder-Bullet3D/BulletContext.h"

using RagDollRef = std::shared_ptr<class RagDoll>;

class RagDoll {
public:
	static RagDollRef create( const bullet::ContextRef &context, const ci::Vec3f &positionOffset );
	
	virtual ~RagDoll();
	
	enum {
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,
		
		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,
		
		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,
		
		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,
		
		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,
		
		BODYPART_COUNT
	};
	
	enum {
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,
		
		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,
		
		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,
		
		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,
		
		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,
		
		JOINT_COUNT
	};

private:
	RagDoll( const bullet::ContextRef &context, const ci::Vec3f &positionOffset );
	
	bullet::ContextRef				mOwner;
	std::vector<bullet::btCollisionShapeRef>	mShapes;
	std::vector<bullet::RigidBodyRef>			mBodies;
	std::vector<bullet::ConstraintBaseRef>		mConstraints;
};
