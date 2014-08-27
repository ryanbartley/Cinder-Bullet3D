#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ConstraintsApp : public AppNative {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void ConstraintsApp::setup()
{
}

void ConstraintsApp::mouseDown( MouseEvent event )
{
}

void ConstraintsApp::update()
{
}

void ConstraintsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( ConstraintsApp, RendererGl )
