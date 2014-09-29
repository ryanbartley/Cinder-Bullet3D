//
//  PTriggerVolume.h
//  BulletChapter1
//
//  Created by Ryan Bartley on 1/24/14.
//
//

#pragma once

#include "Cinder-Bullet3D/Common.h"

namespace bullet {
	
typedef std::shared_ptr<class PTriggerVolume> PTriggerVolumeRef;

class PhyObjTriggerVol {
public:
	PhyObjTriggerVol( btCollisionShape *collShape, const ci::vec3 &position );
	~PhyObjTriggerVol();
	
	void initObject( btCollisionShape *collShape, const ci::vec3 &position );
	
	btCollisionObject* getCollisionObject() { return mTrigger; }
	
private:
	btCollisionObject* mTrigger;
};

}