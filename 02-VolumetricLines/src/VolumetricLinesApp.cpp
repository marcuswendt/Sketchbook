
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"

#include "Resources.h"
#include "ShaderSet.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct LinePath {
public:
    LinePath() {}
    ~LinePath() {}
    
    void initShader(DataSourcePathRef file);
    int length() { return vertices.size(); }  
    void insert(Vec3f point);    
    void draw(Camera const& cam);
    
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<float> coords;
    
    gl::GlslProg shader;
};

void LinePath::initShader(DataSourcePathRef file)
{    
    try {
        // works
//        shader = fieldkit::gl::loadShaderSet(file, 
//                                             "test.vs", "test.fs",
//                                             "test.gs", 
//                                             GL_LINES_ADJACENCY_EXT, GL_TRIANGLE_STRIP, 24);
        
        shader = fieldkit::gl::loadShaderSet(file, 
                                             "VolumetricLine.vs", "test.fs",
                                             "test2.gs", 
                                             GL_LINES_ADJACENCY_EXT, GL_TRIANGLE_STRIP, 24);
        
    } catch(exception& e) {
        printf("Error: %s", e.what());
    }
}

void LinePath::insert(Vec3f point)
{
    Vec3f normal = Vec3f::zero();
    
    if(vertices.size() > 0) {
        Vec3f prev = vertices[vertices.size()-1];
        normal = (point - prev).normalized();
    }
    vertices.push_back(point);
    normals.push_back(normal);
    
    coords.clear();
    int numPoints = vertices.size();
    for(int i=0; i<numPoints; i++) {
        coords.push_back( i/ (float)numPoints );
    }
}

void LinePath::draw(Camera const& cam)
{
    // Update camera
    Matrix44f proj = cam.getProjectionMatrix();
    Matrix44f modelView = gl::getModelView();
    Matrix44f modelViewProj = cam.getProjectionMatrix() * modelView;
    
    shader.uniform("Radius", 10.0f);
    shader.uniform("ModelView", modelView);
    shader.uniform("Projection", proj);
    shader.uniform("ModelViewProjection", modelViewProj);
    
    
    // OpenGL 4.0 style drawing
    int position = 0;
    int normal = 1;
    int pathCoord = 2;
    
    GLsizei stride = sizeof(float) * 0;
//    const GLvoid* positionOffset = (GLvoid*) (0);
//    const GLvoid* normalOffset = (GLvoid*) (sizeof(float) * 3);
    const GLvoid* positionOffset = static_cast<GLvoid*>(&vertices[0]);
    const GLvoid* normalOffset = static_cast<GLvoid*>(&normals[0]);
    const GLvoid* pathCoordOffset = static_cast<GLvoid*>(&coords[0]);
 
    shader.bind();
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, positionOffset);
    
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, stride, normalOffset);

    glEnableVertexAttribArray(pathCoord);
    glVertexAttribPointer(pathCoord, 1, GL_FLOAT, GL_FALSE, stride, pathCoordOffset);
 
//    glDrawArrays(GL_LINES_ADJACENCY_EXT, 0, length());    
    glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, length());
    
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(pathCoord);
    shader.unbind();
    
    // OpenGL 2.0 style drawing
//    shader.bind();
//    glEnableClientState(GL_VERTEX_ARRAY); 
//    glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);    
//    glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, length());
//    glDisableClientState(GL_VERTEX_ARRAY);
//    shader.unbind();
}



// -----------------------------------------------------------------------------
class VolumetricLinesApp : public AppBasic {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void update();
    void mouseDrag(MouseEvent event);
	void draw();
    
private:
    LinePath path;
    CameraPersp cam;
};


void VolumetricLinesApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(1024, 768);
	settings->setWindowPosition(0, 0);
}

void VolumetricLinesApp::setup()
{
    Vec2f center = getWindowSize() * 0.5f;
    
    cam.lookAt( Vec3f( center.x, center.y, 650), Vec3f(center.x, center.y, 0) );
	cam.setPerspective( 60, getWindowAspectRatio(), 1, 5000 );
    
    path.initShader(loadResource(RES_VOLUMETRIC_LINE));
}

void VolumetricLinesApp::update()
{
}

void VolumetricLinesApp::mouseDrag( MouseEvent event )
{
    path.insert(Vec3f(event.getX(), event.getY(), 0));
}

void VolumetricLinesApp::draw()
{
	gl::clear(Color(0,0,0));
    
    gl::setMatrices(cam);
    
    gl::color(Color(1,1,1));

    Vec2f center = getWindowSize() * 0.5f;
    float w = 25;
    float h = 25;
    gl::drawSolidRect(Rectf(center.x-w, center.y-h, center.x+w, center.y+h));
    
    glLineWidth(10.0);
    path.draw(cam);
}


CINDER_APP_BASIC( VolumetricLinesApp, RendererGl )
