#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/GlslProg.h"

#include "Cinder-Bullet3D/BulletContext.h"
#include "Cinder-Bullet3D/Common.h"
#include "Cinder-Bullet3D/SoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "Cinder-Bullet3D/PhysicsDebugRenderable.h"

#include "cinder/Rand.h"

#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SoftBodyApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	
	bt::SoftBodyRef		mSoftBody;
	bullet::ContextRef	mContext;
	gl::GlslProgRef		mPhongShader;
	CameraPersp			mCam;
	bullet::RigidBodyRef	mPhyPlane;
	bt::SoftBodyRef			mSb;
	btSoftBodyWorldInfo mSoftBodyWorldInfo;
	float				mKVC, kLST;
	gl::VboMeshRef		mSoftBodyMesh;
};



void SoftBodyApp::setup()
{
	// Create the shader that will represent all of the stuff
//	mPhongShader = gl::GlslProg::create( gl::GlslProg::Format()
//#if ! defined( CINDER_GL_ES )
//										.vertex( loadAsset( "Phong.vert" ) )
//										.fragment( loadAsset( "Phong.frag" ) )
//#else
//										.vertex( loadAsset( "Phong_ios.vert" ) )
//										.fragment( loadAsset( "Phong_ios.frag" ) )
//#endif
//										);
	
	// Create a context. This Context stores all of the bullet world.
	// Take a look at the Context::Format to find out just what can be
	// controlled. Specifically here, we're creating a format that allows
	// debug Drawing
	mContext = bullet::Context::create( bullet::Context::Format().drawDebug( true ).createDebugRenderer( true ).createSoftRigidWorld( true ) );
	
	const btScalar	s=2;
	const btScalar	h=10;
	const int		segments=6;
	const int		count=50;
	for(int i=0;i<count;++i)
	{
		btSoftBody*		psb=btSoftBodyHelpers::CreatePatch( mSoftBodyWorldInfo,btVector3(-s,h,-s),
														   btVector3(+s,h,-s),
														   btVector3(-s,h,+s),
														   btVector3(+s,h,+s),
														   segments,segments,
														   0,true);
		btSoftBody::Material*	pm=psb->appendMaterial();
//		pm->m_flags				-=	btSoftBody::fMaterial::DebugDraw;
		psb->generateBendingConstraints(2,pm);
		psb->m_cfg.kLF			=	0.004;
		psb->m_cfg.kDG			=	0.0003;
		psb->m_cfg.aeromodel	=	btSoftBody::eAeroModel::V_TwoSided;
		btTransform		trs;
		btQuaternion	rot;
		btVector3		ra= bt::toBullet( randVec3() * 0.1f );
		btVector3		rp= bt::toBullet( randVec3() * 15.0f + vec3( 0, 5, 0 ) );
		rot.setEuler(SIMD_PI/8+ra.x(),-SIMD_PI/7+ra.y(),ra.z());
		trs.setIdentity();
		trs.setOrigin(rp);
		trs.setRotation(rot);
		psb->transform(trs);
		psb->setTotalMass(0.1);
		psb->addForce(btVector3(0,2,0),0);
		mContext->addSoftBody( psb );
		
	}

	
	bt::SoftBody::Format format;
	format.worldInfo( mContext->getSoftBodyWorldInfo() );
	
	mSoftBodyWorldInfo.m_broadphase = mContext->getBroadPhase();
	mSoftBodyWorldInfo.m_dispatcher = mContext->getCollisionDispatcher();
	mSoftBodyWorldInfo.m_sparsesdf.Initialize();
	
//	mSb = bt::SoftBody::createFromConvexHull( mSoftBodyWorldInfo, bt::createConvexHull( TriMesh::create( geom::Capsule() ) ) );
//	mSb = bt::SoftBody::createPatch( mSoftBodyWorldInfo, vec3( 0 ), vec3( 0, 1, 0 ), vec3( 1, 0, 0 ), vec3( 1, 1, 0 ), 3, 3, 0, true );
	mSb = bt::SoftBody::createEllipsoid( mSoftBodyWorldInfo, vec3( 0 ), vec3( 1, 1, 1 ), 128 );
	mSb->getConfig().kVC = mKVC = 1.0f;
	mSb->getMaterial( 0 )->m_kLST = kLST = 1.0;
	mSb->setTotalMass( 1.0 );
	mSb->setPose( true, false );
	mSb->translate( vec3( 0, 10, 0 ) );
	mContext->addSoftBody( mSb );
	
	mPhyPlane = bt::RigidBody::create( bt::RigidBody::Format().collisionShape( bt::createBoxShape( vec3( 1, 1, 1 ) ) ).initialPosition( vec3( 0, 0, 0 ) ).initialScale( vec3( 20, 1, 20 ) ).addToWorld( true ) );
	
	mContext->addRigidBody( mPhyPlane );
	
	mSoftBodyMesh = bt::drawableHelpers::getDrawableSoftBody( mSb );
	
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), .01, 1000.0f );
	mCam.lookAt( vec3( 0, 10, 10 ), vec3( 0, 0, 0 ) );
	
}

void SoftBodyApp::mouseDown( MouseEvent event )
{
}

void SoftBodyApp::update()
{
	mSb->getConfig().kVC = mKVC;
	mSb->getMaterial( 0 )->m_kLST = kLST;
	mContext->update();
	bt::drawableHelpers::updateVboMesh( mSoftBodyMesh, mSb );
}

void SoftBodyApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::setMatrices( mCam );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	bt::drawableHelpers::drawSoftBody( mSb, bt::drawableHelpers::SoftBodyDrawType::LINES );
//	gl::draw( mSoftBodyMesh );
	mContext->debugDraw();
}

CINDER_APP( SoftBodyApp, RendererGl )
