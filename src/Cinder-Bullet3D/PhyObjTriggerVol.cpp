//
//  PTriggerVolume.cpp
//  BulletChapter1
//
//  Created by Ryan Bartley on 1/24/14.
//
//

#include "Physics/Common.h"
#include "Physics/PhyObjTriggerVol.h"
#include "Physics/BulletContext.h"

namespace bullet {

PhyObjTriggerVol::PhyObjTriggerVol( btCollisionShape *collShape, const ci::Vec3f &position )
{
	initObject( collShape, position );
}
	
PhyObjTriggerVol::~PhyObjTriggerVol()
{
	Context()->world()->removeCollisionObject( mTrigger );
	auto shape = mTrigger->getCollisionShape();
	delete shape;
	delete mTrigger;
}

void PhyObjTriggerVol::initObject( btCollisionShape *collShape, const ci::Vec3f &position )
{
	mTrigger = new btCollisionObject();
	
	mTrigger->setCollisionShape( collShape );
	
	btTransform triggerTrans;
	triggerTrans.setIdentity();
	triggerTrans.setOrigin( toBullet( position ) );
	mTrigger->setWorldTransform( triggerTrans );
	
	mTrigger->setCollisionFlags( btCollisionObject::CF_NO_CONTACT_RESPONSE );
	
	Context()->world()->addCollisionObject( mTrigger );
}
	
}