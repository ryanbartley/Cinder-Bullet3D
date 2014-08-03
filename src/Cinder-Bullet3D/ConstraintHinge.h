//
//  ConstraintHinge.h
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/RigidBody.h"
#include "Cinder-Bullet3D/ConstraintBase.h"

namespace bullet {
	
typedef std::shared_ptr<class ConstraintHinge> ConstraintHingeRef;
	
class ConstraintHinge : public ConstraintBase {
public:
	
	struct Format : ConstraintBase::Format {
		
	};
	
	static ConstraintHingeRef create( const Format &format );
	
	~ConstraintHinge();
	
private:
	ConstraintHinge( const Format &format );
};
	
}
