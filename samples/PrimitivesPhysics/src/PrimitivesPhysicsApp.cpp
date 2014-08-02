#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PrimitivesPhysicsApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void PrimitivesPhysicsApp::setup()
{
}

void PrimitivesPhysicsApp::mouseDown( MouseEvent event )
{
}

void PrimitivesPhysicsApp::update()
{
}

void PrimitivesPhysicsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( PrimitivesPhysicsApp, RendererGl )
