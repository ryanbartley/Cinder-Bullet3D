//
//  ConstraintPoint2Point.h
//  RagDoll
//
//  Created by Ryan Bartley on 8/3/14.
//
//

#pragma once

#include "ConstraintBase.h"

namespace bullet {
	

	
class ConstraintPoint2Point : public ConstraintBase {
public:
	
	struct Format : public ConstraintBase::Format {
		
		
		
	private:
		
	};
	
	ConstraintPoint2PointRef create( const Format &format );
	
	~ConstraintPoint2Point() {}
	
private:
	ConstraintPoint2Point( const Format &format );
};
	
}
