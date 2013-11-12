#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/Font.h"
#include "cinder/Utilities.h"
#include "cinder/gl/TextureFont.h"
#include "TextureFont2.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class Cell {
public:
    float x, y, w, h;
    std::string text;
    Cell() {};
};

class LotsOftextApp : public AppNative {
  public:
    std::vector<Cell> cells;
    
    Font font;
//	gl::TextureFontRef	textureFont;
	gl::TextureFont2Ref	textureFont;
    
    void prepareSettings(Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void drawDebug();
};

void LotsOftextApp::prepareSettings(Settings *settings)
{
    settings->setWindowSize( 1280, 800 );
    settings->setFrameRate( 60.0f );
}

void LotsOftextApp::setup()
{
    int cols = 30 / 3;
    int rows = 90 / 3;
    float gutter = 2;
    float lineSpacing = 1;
    
    float w = getWindowWidth() / (float)cols;
    float h = getWindowHeight() / (float)rows;
    
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            Cell c;
            c.x = j * w;
            c.y = i * h;
            
            c.w = w - gutter;
            c.h = h - lineSpacing;
            
            c.text = "lorem ipsum";
            
            cells.push_back(c);
        }
    }
    
    // resources
    font = Font("Apercu Mono", 8);
    textureFont = gl::TextureFont2::create(font);
}

void LotsOftextApp::mouseDown( MouseEvent event )
{
}

void LotsOftextApp::update()
{
}

void LotsOftextApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::enableAlphaBlending();

    gl::color(ColorA(1, 1, 1, 1));
    
    textureFont->beginDraw();
    for(Cell c : cells) {
        textureFont->drawString(toString(c.text), Vec2f(c.x, c.y));
    }
    textureFont->endDraw();
    
    drawDebug();
}

void LotsOftextApp::drawDebug()
{
    gl::color(ColorA(1, 1, 1, 0.1f));

//    for(Cell c : cells) {
//        ci::gl::drawSolidRect(Rectf( c.x, c.y, c.x + c.w, c.y + c.h ));
//    }

    console() << "FPS " << floor(getAverageFps()) << std::endl;
    
//    gl::color(ColorA(1, 0, 0, 1));
//	textureFont->drawString(toString(floor(getAverageFps())) + " FPS",
//                            Vec2f(10, getWindowHeight() - textureFont->getDescent()));
}

CINDER_APP_NATIVE( LotsOftextApp, RendererGl )
