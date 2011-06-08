#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"

#include "fieldkit/FieldKit.h"
#include "fieldkit/Logger.h"
#include "ShaderSet.h"

#define DIM 100

using namespace ci;
using namespace ci::app;

class ShaderTestApp : public AppCocoaTouch {
  public:
	virtual void	setup();
	virtual void	resize( ResizeEvent event );
	virtual void	update();
	virtual void	draw();
	virtual void	mouseDown( MouseEvent event );
		
    gl::GlslProg gradient_;
//	gl::Texture mTex;
	CameraPersp	cam_;
};

void ShaderTestApp::setup()
{
    LOG_INFO("setup");
    
    try {
        gradient_ = fieldkit::gl::loadShaderSet(loadResource("gradient.glsl"));
        
    } catch(std::exception& e) {
        LOG_ERROR(e.what());
        _exit(-1);
    }
}

void ShaderTestApp::resize( ResizeEvent event )
{
    LOG_INFO("resize");
    
	cam_.lookAt(Vec3f(0, 0, -DIM), Vec3f::zero() );
	cam_.setPerspective( 60, event.getAspectRatio(), 1, 1000 );
}

void ShaderTestApp::mouseDown( MouseEvent event )
{
	console() << "Mouse down @ " << event.getPos() << std::endl;
}

void ShaderTestApp::update()
{
//	mCubeRotation.rotate( Vec3f( 1, 1, 1 ), 0.03f );
}

void ShaderTestApp::draw()
{
	gl::clear( Color(0.0, 0.0, 0.0) );
    

    float x = 0;
    float y = 0;    
    float s = DIM * 0.25;
    
    std::vector<Vec3f> vertices;
    vertices.push_back(Vec3f( x, y+s, 0));
    vertices.push_back(Vec3f( x+s, y-s, 0));
    vertices.push_back(Vec3f( x-s, y-s, 0));
    
    std::vector<ColorA> colors;
    colors.push_back(ColorA(1,0,0,1));
    colors.push_back(ColorA(0,1,0,1));
    colors.push_back(ColorA(0,0,1,1));
    
    GLsizei stride = sizeof(float) * 0;
    const GLvoid* positionOffset = static_cast<GLvoid*>(&vertices[0]);
    const GLvoid* colorOffset = static_cast<GLvoid*>(&colors[0]);
    
    gradient_.bind();
    gradient_.uniform("modelView", cam_.getModelViewMatrix());
    gradient_.uniform("projection", cam_.getProjectionMatrix());

    int position = gradient_.getAttribLocation("position");
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, positionOffset);
    glEnableVertexAttribArray(position);

    int color = gradient_.getAttribLocation("color");
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, stride, colorOffset);
    
//    LOG_INFO("pos "<< position <<" color "<< color <<"p1: "<< vertices[0] <<" p2: "<< vertices[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(color);
    gradient_.unbind();
}

CINDER_APP_COCOA_TOUCH( ShaderTestApp, RendererGl )
