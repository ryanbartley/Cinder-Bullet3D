//
//  ConstraintHinge.h
//  BulletSourceTest
//
//  Created by Ryan Bartley on 4/7/14.
//
//

#pragma once

#include "Physics/Common.h"
#include "Physics/PhyObjBase.h"
#include "Physics/ConstraintBase.h"

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
