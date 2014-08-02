#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VisualPhysicsApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void VisualPhysicsApp::setup()
{
}

void VisualPhysicsApp::mouseDown( MouseEvent event )
{
}

void VisualPhysicsApp::update()
{
}

void VisualPhysicsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( VisualPhysicsApp, RendererGl )
