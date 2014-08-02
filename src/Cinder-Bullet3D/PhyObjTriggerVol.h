//
//  PTriggerVolume.h
//  BulletChapter1
//
//  Created by Ryan Bartley on 1/24/14.
//
//

#pragma once

#include "Physics/Common.h"

namespace bullet {
	
typedef std::shared_ptr<class PTriggerVolume> PTriggerVolumeRef;

class PhyObjTriggerVol {
public:
	PhyObjTriggerVol( btCollisionShape *collShape, const ci::Vec3f &position );
	~PhyObjTriggerVol();
	
	void initObject( btCollisionShape *collShape, const ci::Vec3f &position );
	
	operator btCollisionObject*() { return mTrigger; }
	btCollisionObject* getCollisionObject() { return mTrigger; }
	
private:
	btCollisionObject* mTrigger;
};

}