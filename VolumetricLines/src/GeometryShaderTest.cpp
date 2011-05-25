//
//  GeometryShaderTest.cpp
//  VolumetricLines
//
//  Created by Marcus Wendt on 24/05/2011.
//  Copyright 2011 Marcus Wendt. All rights reserved.
//

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "Resources.h"
#include "ShaderSet.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GeometryShaderTest : public AppBasic {
public:
    void prepareSettings(Settings *settings);
	void setup();
    void mouseDrag(MouseEvent event);
	void draw();
    
private:
    gl::GlslProg shader;
    Vec2f mouse;
};


void GeometryShaderTest::prepareSettings(Settings *settings)
{
	settings->setWindowSize(1024, 768);
	settings->setWindowPosition(0, 0);
}

void GeometryShaderTest::setup()
{
    try {
        shader = fieldkit::gl::loadShaderSet(loadResource("geoshader_test.glsl"), GL_LINES, GL_LINE_STRIP, 100);
    } catch(exception& e) {
        printf("Error: %s", e.what());
    }
    
    mouse = getWindowSize() * 0.5;
}

void GeometryShaderTest::mouseDrag( MouseEvent event )
{
    mouse.x = event.getX();
    mouse.y = event.getY();
}

void GeometryShaderTest::draw()
{
	gl::clear(Color(0,0,0));

    gl::color(Color(1,1,1));
    glLineWidth(4.0);
//    glPointSize(15.0);
    shader.bind();
    glBegin(GL_LINES);
        glVertex2f(mouse.x, mouse.y);
        glVertex2f(mouse.x + 100, mouse.y);
    glEnd();
    shader.unbind();
}


CINDER_APP_BASIC( GeometryShaderTest, RendererGl )
