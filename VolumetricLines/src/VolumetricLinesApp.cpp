
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "Resources.h"
#include "ShaderSet.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct LinePath {
public:
    LinePath() {}
    ~LinePath() {}
    
    void loadShader(DataSourcePathRef file);
    int length() { return vertices.size(); }  
    void insert(Vec3f point);    
    void draw();
    
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<float> coords;
    
    gl::GlslProg shader;
};

void LinePath::loadShader(DataSourcePathRef file)
{    
    try {
        shader = fieldkit::gl::loadShaderSet(file);   
        
//        // init geometry shader
//        GLuint handle = shader.getHandle();
//        glProgramParameteriEXT(handle, GL_GEOMETRY_INPUT_TYPE_EXT, GL_LINES_ADJACENCY_EXT);
//        glProgramParameteriEXT(handle, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
//        glProgramParameteriEXT(handle, GL_GEOMETRY_VERTICES_OUT_EXT, 24);
        
        // set basic uniforms
        shader.uniform("Radius", 10.0f);
        shader.uniform("Modelview", gl::getModelView());
        shader.uniform("Projection", gl::getProjection());
        shader.uniform("ModelviewProjection", gl::getModelView());
        
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

void LinePath::draw()
{
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
    
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, stride, positionOffset);
    
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, stride, normalOffset);

    glEnableVertexAttribArray(pathCoord);
    glVertexAttribPointer(pathCoord, 1, GL_FLOAT, GL_FALSE, stride, pathCoordOffset);
 
    shader.bind();
    glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, length());
    shader.unbind();
    
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(normal);
    glDisableVertexAttribArray(pathCoord);
    
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
};


void VolumetricLinesApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(1024, 768);
	settings->setWindowPosition(0, 0);
}

void VolumetricLinesApp::setup()
{
    path.loadShader(loadResource(RES_VOLUMETRIC_LINE));
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
    
    gl::color(Color(1,1,1));
    glLineWidth(10.0);

    path.draw();
}


CINDER_APP_BASIC( VolumetricLinesApp, RendererGl )
