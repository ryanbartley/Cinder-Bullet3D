//
//  PTriggerVolume.cpp
//  BulletChapter1
//
//  Created by Ryan Bartley on 1/24/14.
//
//

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/PhyObjTriggerVol.h"
#include "Cinder-Bullet3D/BulletContext.h"

namespace bullet {

PhyObjTriggerVol::PhyObjTriggerVol( btCollisionShape *collShape, const ci::vec3 &position )
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

void PhyObjTriggerVol::initObject( btCollisionShape *collShape, const ci::vec3 &position )
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