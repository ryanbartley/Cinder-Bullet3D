//
//  BulletContext.cpp
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/25/13.
//
//

#include "BulletContext.h"
#include "PhysicsDebugRenderable.h"

namespace bullet {

static class Context* sBulletContext = nullptr;
static bool sBulletContextInitialized = false;
	
	
Context::Format::Format()
: mWorld( nullptr ), mCollisionDispatcher( nullptr ),
	mBroadphase( nullptr ), mSolver( nullptr ),
	mConfiguration( nullptr ), mCreateDebugRenderer( false ),
	mDrawDebug( false ), mStepVal( 1.0f / 60.0f), mGravity( ci::Vec3f( 0.0f, -9.8f, 0.0f ) ),
	mDebugMode(  btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits )
{
	
}

ContextRef Context::create( const Format &format )
{
	static ContextRef factoryBulletContext;
	if( ! sBulletContextInitialized ) {
		factoryBulletContext = ContextRef( new Context( format ) );
	}
	
	return factoryBulletContext;
}
	
Context::Context( const Format &format )
: mStepVal( format.mStepVal ), mDrawDebug( format.mDrawDebug )
{
	init( format );
}
	
void Context::init( const Format &format )
{
	mCollisionConfiguration = format.mConfiguration ? format.mConfiguration : new btDefaultCollisionConfiguration();
	mCollisionDispatcher = format.mCollisionDispatcher ? format.mCollisionDispatcher : new btCollisionDispatcher(mCollisionConfiguration);
	mBroadPhase = format.mBroadphase ? format.mBroadphase : new btDbvtBroadphase();
	mSolver = format.mSolver ? format.mSolver : new btSequentialImpulseConstraintSolver();
	mWorld = format.mWorld ? format.mWorld : new btDiscreteDynamicsWorld( mCollisionDispatcher, mBroadPhase, mSolver, mCollisionConfiguration );
	
	// If we want to setup the debug renderer
	if( format.mCreateDebugRenderer ) {
		setupDebugRenderer( format.mDebugMode );
	}
	
	// This initializes the static pointer to the bullet context
	sBulletContext = this;
	sBulletContextInitialized = true;
	
	mWorld->setWorldUserInfo( this );
	mWorld->setGravity( toBullet( format.mGravity ) );
	
}
	
Context::~Context()
{
	sBulletContext = nullptr;
	sBulletContextInitialized = false;
	delete mWorld;
	delete mSolver;
	delete mBroadPhase;
	delete mCollisionDispatcher;
	delete mCollisionConfiguration;
}
	
class Context* Context::getCurrent()
{
	if( ! sBulletContextInitialized ) {
		return nullptr;
	}
	return sBulletContext;
}
	
void Context::setupDebugRenderer( int mode )
{
	if( mDebugRenderer )
		return;

	mDebugRenderer.reset( new PhysicsDebugRenderable() );
	
	if( mWorld ) {
		mWorld->setDebugDrawer( mDebugRenderer.get() );
		mDebugRenderer->setDebugMode( mode );

	}
}
	
void Context::addRigidBody( const PhyObjBaseRef &phyObj )
{
	addRigidBody( phyObj->getRigidBody().get(), phyObj->mCollGroup, phyObj->mCollMask );
	phyObj->setIsAddedToWorld( true );
}
	
void Context::removeRigidBody( const PhyObjBaseRef &phyObj )
{
	removeRigidBody( phyObj->getRigidBody().get() );
}
	
void Context::update()
{
	mWorld->stepSimulation(mStepVal);
	checkForCollisions();
	if( mDrawDebug ) {
		mDebugRenderer->update();
		mWorld->debugDrawWorld();
	}
}
	
bool Context::closestRayCast( const ci::Vec3f &startPosition, const ci::Vec3f &direction, RayResult &result )
{
	if( ! world() )
		return false;
	
	btVector3 rayTo = toBullet( direction * 1000 );
	btVector3 rayFrom = toBullet( startPosition );
	
	btCollisionWorld::ClosestRayResultCallback rayCallback( rayFrom, rayTo );
	
	
	world()->rayTest( rayFrom, rayTo, rayCallback );
	
	if( rayCallback.hasHit() ) {
		
		btRigidBody* pBody = (btRigidBody*) btRigidBody::upcast( rayCallback.m_collisionObject );
		
		if(!pBody)
			return false;
		
		result.pBody = pBody;
		result.hitPoint = fromBullet( rayCallback.m_hitPointWorld );
		result.hitNormal = fromBullet( rayCallback.m_hitNormalWorld );
		
		return true;
	}
	
	return false;
}
	
void Context::debugDraw()
{
	if( ! ( mDrawDebug && mDebugRenderer ) )
		return;
	
	mDebugRenderer->draw();
}
	
void Context::checkForCollisions()
{
	// keep a list of the collision pairs we
	// found during the current update
	CollisionPairs pairsThisUpdate;
	
	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < mCollisionDispatcher->getNumManifolds(); ++i) {
		
		// get the manifold
		btPersistentManifold* pManifold = mCollisionDispatcher->getManifoldByIndexInternal(i);
		
		// ignore manifolds that have
		// no contact points.
		if (pManifold->getNumContacts() > 0) {
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());
			
			// if either of these objects are static we don't care about the pair
			// if( pBody0->isStaticObject() || pBody1->isStaticObject() ) continue;
			
			// always create the pair in a predictable order
			// (use the pointer value..)
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;
			
			// create the pair
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);
			
			// insert the pair into the current list
			pairsThisUpdate.insert(thisPair);
			
			// if this pair doesn't exist in the list
			// from the previous update, it is a new
			// pair and we must send a collision event
			if ( mPairs.find(thisPair) == mPairs.end() ) {
				collisionBegin( const_cast<btRigidBody*>(pBody0), const_cast<btRigidBody*>(pBody1) );
			}
		}
	}
	
	// create another list for pairs that
	// were removed this update
	CollisionPairs removedPairs;
	
	// this handy function gets the difference beween
	// two sets. It takes the difference between
	// collision pairs from the last update, and this
	// update and pushes them into the removed pairs list
	std::set_difference( mPairs.begin(), mPairs.end(),
						pairsThisUpdate.begin(), pairsThisUpdate.end(),
						std::inserter(removedPairs, removedPairs.begin()));
	
	// iterate through all of the removed pairs
	// sending separation events for them
	CollisionPairs::const_iterator iter = removedPairs.begin();
	CollisionPairs::const_iterator end = removedPairs.end();
	while( iter != end ) {
		collisionEnd( const_cast<btRigidBody*>(iter->first), const_cast<btRigidBody*>(iter->second) );
		++iter;
	}
	
	// in the next iteration we'll want to
	// compare against the pairs we found
	// in this iteration
	mPairs = pairsThisUpdate;
}
	
}