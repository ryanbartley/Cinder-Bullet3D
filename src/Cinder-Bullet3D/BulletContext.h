//
//  BulletContext.h
//  BulletTestChapter1
//
//  Created by Ryan Bartley on 12/25/13.
//
//

#pragma once

#include <boost/signals2.hpp>

#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/RigidBody.h"

#include "Cinder-Bullet3D/ConstraintBase.h"

namespace bullet {
	
typedef std::shared_ptr<class Context>								ContextRef;
typedef std::shared_ptr<class PhysicsDebugRenderable>				DebugRendererRef;
typedef std::pair<const btRigidBody*, const btRigidBody*>			CollisionPair;
typedef std::set<CollisionPair>										CollisionPairs;
typedef boost::signals2::signal<void(btRigidBody*, btRigidBody*)>	CollisionSignal;
typedef std::function<void(btRigidBody*, btRigidBody*)>				CollisionFunc;
	
struct RayResult {
	btRigidBody* pBody;
	ci::Vec3f hitPoint;
	ci::Vec3f hitNormal;
};

class Context {
public:
	
	//! A small struct to further specialize the Bullet Context
	struct Format {
		//! Default constructs a Bullet Context Format.
		Format();
		
		//! Sets the collision configuration. (Only use this if you know what you're doing.)
		Format& collisionConfiguration( btCollisionConfiguration *collConfig ) { mConfiguration = collConfig; return *this; }
		//! Sets the collision dispatcher. (Only use this if you know what you're doing)
		Format& collisionDispatcher( btCollisionDispatcher *dispatcher ) { mCollisionDispatcher = dispatcher; return *this; }
		//! Sets the broad phase interface. (Only use this if you know what you're doing.)
		Format& broadphase( btBroadphaseInterface *broadphase ) { mBroadphase = broadphase; return *this; }
		//! Sets the constraint solver. (Only use this if you know what you're doing)
		Format& constraintSolver( btConstraintSolver *solver ) { mSolver = solver; return *this; }
		//! Sets the world. (Only use this if you know what you're doing.)
		Format& world( btDynamicsWorld *world ) { mWorld = world; return *this; }
		
		//! Sets the step simulation value. Defaults to 1.0f / 60.0f.
		Format& setStepVal( float stepVal ) { mStepVal = stepVal; return *this; }
		//! Sets whether or not to create the debug renderer. Defaults to false.
		Format& createDebugRenderer( bool createDebugRenderer ) { mCreateDebugRenderer = createDebugRenderer; return *this; }
		//! Sets whether BulletContext should draw debug. If you don't createDebugDraw this is a meaningless function. Defaults to false.
		Format& drawDebug( bool drawDebug ) { mDrawDebug = drawDebug; return *this; }
		//! Sets the gravity for the world.
		Format& gravity( const ci::Vec3f &gravity ) { mGravity = gravity; return *this; }
		//! Sets the render mode for the debug drawer. Default is DrawContactPoints, DrawWireframe, DrawConstraints, DrawConstraintLimits. Modes combined by bitwise or ( | ) operator. You can find the other modes in btIDebugDraw.h.
		Format& debugDrawMode( int mode ) { mDebugMode = mode; return *this; }
		
		bool getDrawDebug() const { return mDrawDebug; }
		
	private:
		btDynamicsWorld				*mWorld;
		btCollisionDispatcher		*mCollisionDispatcher;
		btBroadphaseInterface		*mBroadphase;
		btConstraintSolver			*mSolver;
		btCollisionConfiguration	*mConfiguration;
		ci::Vec3f					mGravity;
		float						mStepVal;
		int							mDebugMode;
		bool						mCreateDebugRenderer, mDrawDebug;
		
		friend class Context;
	};
	
	// Static singleton create method Which returns a constructed shared_ptr of BulletContext.
	static ContextRef create( const Format &format = Format() );
	
	~Context();
	
	//! Returns the Bullet world.
	btDynamicsWorld* world() { return mWorld; }
	
	//! Add a rigid body to the world.
	inline void addRigidBody( btRigidBody *body ) { addRigidBody( body, -1, -1 ); }
	//! Add a rigid body to the world with a Collision group and mask.
	inline void addRigidBody( btRigidBody *body, int16_t group, int16_t mask ) { mWorld->addRigidBody( body, group, mask ); }
	void addRigidBody( const RigidBodyRef &phyObj );
	//! Remove a rigid body from the world.
	inline void removeRigidBody( btRigidBody *body ){ mWorld->removeRigidBody( body ); }
	void removeRigidBody( const RigidBodyRef &phyObj );
	//! Add a collision Object to the world.
	inline void addCollisionObject( btCollisionObject *collObj ) { mWorld->addCollisionObject( collObj ); }
	//! Removes a Collision Object from the world.
	inline void removeCollisionObject( btCollisionObject *collObj ) { mWorld->removeCollisionObject( collObj ); }
	//! Adds an Action to the world.
	inline void addAction( btActionInterface *action ) { mWorld->addAction( action ); }
	//! Removes a constraint from the world.
	inline void removeAction( btActionInterface *action ){ mWorld->removeAction( action ); }
	//! Adds a constraint to the world.
	inline void addConstraint( const btTypedConstraintRef &constraint, bool disableCollisionsBetweenLinkedBodies = false ) { mWorld->addConstraint( constraint.get(), disableCollisionsBetweenLinkedBodies ); }
	inline void addConstraint( const ConstraintBaseRef &constraint, bool disableCollisionsBetweenLinkedBodies = false ) { mWorld->addConstraint( constraint->getTypedConstraint().get(), disableCollisionsBetweenLinkedBodies ); }
	//! Removes a constraint from the world.
	inline void removeConstraint( const btTypedConstraintRef &constraint ) { mWorld->removeConstraint( constraint.get() ); }
	inline void removeConstraint( const ConstraintBaseRef &constraint ) { mWorld->removeConstraint( constraint->getTypedConstraint().get() ); }
	
	//! Set the Gravity of the world. Default is Vec3f( 0.0f, -9.8f, 0.0f ).
	inline void setGravity( const ci::Vec3f &gravity ) { mWorld->setGravity( toBullet( gravity ) ); }
	//! Set the Time Increment Step Value of the world for each frame. Default is 1/60 for 60fps.
	inline void setStepVal( float stepVal ) { mStepVal = stepVal; }
	//! Returns the current stepVal.
	float getStepVal() { return mStepVal; }
	//! Set the internal Tick Call Back for the world
	void setInternalTickCallBack( btInternalTickCallback tickCallback, void* worldUserInfo = nullptr, bool preTick = false ) { mWorld->setInternalTickCallback( tickCallback, worldUserInfo, preTick ); }
	
	//! Updates the world. Used normally in the update loop. It also fires the Collision Checker and handles sending data to and updating the Debug Drawer, if Debug Drawer is enabled.
	void update();
	//! Fires the Debug Draw to send the updated data to the screen.
	void debugDraw();
	//! This turns debug draw on or off.
	void toggleDebugDraw() { mDrawDebug = !mDrawDebug; }
	
	//! Generic RayCast function that returns the closest object it hits in \a RayResult
	bool closestRayCast( const ci::Vec3f &startPosition, const ci::Vec3f &direction, RayResult &result );
	
	btCollisionObjectArray& getCollisionObjects() { return mWorld->getCollisionObjectArray(); }
	const btCollisionObjectArray& getCollisionObjects() const { return mWorld->getCollisionObjectArray(); }
	
	//! Returns the globally initialized BulletContext or a nullptr if not initialized.
	static Context* getCurrent();
	
	//! Returns a pointer to the current broadPhase Interface.
	btBroadphaseInterface*			getBroadPhase() { return mBroadPhase; }
	//! Returns a const pointer to the current broadPhase Interface.
	const btBroadphaseInterface*	getBroadPhase() const { return mBroadPhase; }
	
	//! Returns a pointer to the collision dispatcher.
	btCollisionDispatcher*			getCollisionDispatcher() { return mCollisionDispatcher; }
	//! Returns a const pointer to the collision dispatcher.
	const btCollisionDispatcher*	getCollisionDispatcher() const { return mCollisionDispatcher; }
	
	//! Returns a pointer to the collision configuration.
	btCollisionConfiguration*		getCollisionConfiguration() { return mCollisionConfiguration; }
	//! Returns a const pointer to the collision configuration.
	const btCollisionConfiguration*	getCollisionConfiguration() const { return mCollisionConfiguration; }
	
	//! Returns a pointer to the constraint solver.
	btConstraintSolver*				getConstraintSolver() { return mSolver; }
	//! Returns a const pointer to the constraint solver.
	const btConstraintSolver*		getConstraintSolver() const { return mSolver; }
	
	//! Returns the current world type.
	btDynamicsWorldType getWorldType() { return mWorld->getWorldType(); }
	
	//! Sets up the debug renderer. Returns if already setup.
	void setupDebugRenderer( int mode );
	
	//! Returns the current collision pairs. 
	CollisionPairs& getCurrentCollisionPairs() { return mPairs; }
	
private:
	//! Initializes the world and creates the Debug Renderer if \a debugRenderer is true. Also sets the static pointer to BulletContext for use with getCurrent().
	Context( const Format &format );
	
	//! Initializes the world.
	void init( const Format &format );
	
	//! Iterates through the Persistent manifolds and fires whether Collisions have begun or ended. This is
	void checkForCollisions();

	btBroadphaseInterface	   *mBroadPhase;
	btCollisionConfiguration   *mCollisionConfiguration;
	btCollisionDispatcher	   *mCollisionDispatcher;
	btConstraintSolver		   *mSolver;
	btDynamicsWorld			   *mWorld;
	
	CollisionPairs			mPairs;
	CollisionSignal			mCollisionBegin, mCollisionEnd;
	DebugRendererRef		mDebugRenderer;
	mutable float			mStepVal;
	bool					mDrawDebug;
	
	// Actual Collision Signal Dispatchers
	inline void collisionBegin( btRigidBody* pBody0, btRigidBody* pBody1 )
	{ mCollisionBegin( pBody0, pBody1 ); }
	inline void collisionEnd( btRigidBody* pBody0, btRigidBody* pBody1 )
	{ mCollisionEnd( pBody0, pBody1 ); }
	
public:
	//! Interface to add Collision Begin Event Signal
	inline boost::signals2::connection addCollisionBeginSignal( CollisionFunc collisionBeginFunc )
	{ return mCollisionBegin.connect( collisionBeginFunc ); }
	
	//! Interface to add Collision End Event Signal
	inline boost::signals2::connection addCollisionEndSignal( CollisionFunc collisionEndFunc )
	{ return mCollisionEnd.connect( collisionEndFunc ); }
};
	
}
