/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#include "WorkerTest.h"

#include <algorithm>

#include <boost/algorithm/string/replace.hpp>
 #include "cinder/Camera.h"
 #include "cinder/Utilities.h"
// #include "cinder/Rand.h"

#include "Resources.h"
#include "utilities/DrawingUtilities.h"

using namespace ci;
using namespace ci::gl;
using namespace field;


//CINDER_APP_BASIC(WorkerTest, RendererGl)
CINDER_APP_BASIC(WorkerTest, RendererGl(0)); // avoids grey screen bug during fullscreen mode on osx + ati 5870 gpu

void WorkerTest::prepareSettings(Settings *settings)
{
    LOG_INFO("WorkerTest::prepareSettings");
    
    // load config
    try {
        config.loadXML(loadResource(RES_CONFIG));
    } catch(exception& e) {
        LOG_ERROR("Couldnt read configuration file");
        _exit(0);
    }
    
    config.overrideWith(getArgs());
    config.print();
    
    framerate = config.getf("framerate", 60.0f);
    
    // setup screen
    settings->setDisplay( Display::getDisplays()[config.geti("display", 0)] );
    settings->setFrameRate(framerate);
    settings->setFullScreen(config.getb("fullscreen"));
    settings->setChromeless(config.getb("chromeless"));
    
    Vec2i windowSize = config.get2i("window", Vec2i(640,480));
    settings->setWindowSize(windowSize.x, windowSize.y);

    Vec2i windowPos = config.get2i("position", Vec2i(-1,-1));
    settings->setWindowPosition(windowPos.x, windowPos.y);
}


void WorkerTest::setup() 
{
    LOG_INFO("Initialising components:");
    
    // init members
    showDebug = config.getb("debug", false);
    isRecording = false;
    recordFrame = false;
    frame = 0;
    
    // init renderers
    LOG_INFO("- Graphics")
    
    ci::gl::Fbo::Format format;
    format.setSamples( config.geti("aasamples", 0) );
    format.enableColorBuffer(true, 1);
    
    samples = config.geti("samples", 0);
    if(samples > 0) {
        LOG_INFO("rendering into antialiased & supersampled fbo");
        fbo = ci::gl::Fbo(getWindowWidth() * samples, getWindowHeight() * samples, format);
    }
    
        
    // init simulation + network thread
    LOG_INFO("- Workers");
    
    // start threads
    controlWorker.Start();
    physicsWorker.Start();
    interpolateWorker.Start();
        
    // init control script
    std::string script = config.gets("script", "$APP/Contents/Resources/Logic.js");
    boost::algorithm::replace_all(script, "$APP", cinder::app::getAppPath().c_str());
    
    Message init(0);
    init.Add("script", script);
    controlWorker.Notify(init);
    controlWorker.SetWaitForInput(false);
    
    // init physics
    physicsWorker.Notify(Message(0));
    
    // init interpolate 
    interpolateWorker.Notify(Message(0));
}


void WorkerTest::shutdown()
{
    LOG_INFO("WorkerTest::shutdown");
}


void WorkerTest::update() 
{        
    NullState nullState;
    ControlState controlState;
    PhysicsState physicsState;
    
    controlWorker.Next(nullState, controlState);
    physicsWorker.Next(controlState, physicsState);
    interpolateWorker.Next(physicsState, state_);
}


void WorkerTest::keyDown(KeyEvent event)
{
//         char c = event.getChar();
//         int code = event.getCode();
    
// //    LOG_INFO("key "<< c <<" code "<< code);
    
//         if(c == 'd') {
//                 showDebug = !showDebug;

//     } else if(c == 'g') {
//         showStaticGrid = !showStaticGrid;
        
//     } else if(c == 'f') {
//         showFlexibleGrid = !showFlexibleGrid;
        
//     } else if(c == 'w') {
//         showState = !showState;

//     } else if(c == 'r') {
//         recordFrame = true;
        
//     } else if(c == 'R') {
//         isRecording = !isRecording;
        
//         } else if(c == '-') {
//         setGridSize(grid.size + Vec2i(1,1));
        
//     } else if(c == '+') {
//         setGridSize(grid.size - Vec2i(1,1));
//     }
    
//     // in showstate mode left + right keys switch between displayed state
//     if(showState) {
//         // left
//         if(code == 276) {
//             stateProperty--;
//             if(stateProperty < 0)
//                 stateProperty = CellState::NumProperties - 1;
            
//         // right
//         } else if(code == 275) {
//             stateProperty++;
//             if(stateProperty == CellState::NumProperties)
//                 stateProperty = 0;
//         }
//     }
    
//     // pass on key events to simulation
//     runner->KeyPressed(event);
}

void WorkerTest::mouseDown(MouseEvent event)
{
}


// -- Drawing ------------------------------------------------------------------
void WorkerTest::draw() 
{        
    hideCursor();
    enableAlphaBlending();
    
    // clear buffers
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    drawScene();
    
    if(showDebug)
        drawDebug();
    
    if(isRecording || recordFrame) {
        std::stringstream ss;
        ss << getHomeDirectory() << "Desktop/WorkerTest"<< getPathSeparator() << "petite_" << frame <<".png";
        std::string path = ss.str();
        
//        LOG_INFO("Writing "<< path);
//        ci::writeImage(path, ci::app::copyWindowSurface());   
        frame++;
        recordFrame = false;
    }
}

void WorkerTest::drawScene()
{
    ci::gl::color(Color(0.0,0.1,0.9));
    glLineWidth(2.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &state_.lines_[0].x);
    glDrawArrays(GL_LINES, 0, state_.lines_.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
//    std::vector<Vec3f>::iterator it = physicsState.lines_.begin();
//    for(; it != physicsState.lines_.end(); ) {
//        Vec3f a = *it;
//        it++;
//        Vec3f b = *it;
//        it++;
//        ci::gl::drawLine(a, b);
//	}   
    
    /*
    // draw scene into fbo
    if(fbo) {
        fbo.bindFramebuffer();
        setViewport(fbo.getBounds());
        pushMatrices();
        
        // setup the viewport to match the dimensions of the fbo
        CameraOrtho cam(0.0f, fbo.getWidth(), 0.0f, fbo.getHeight(), -0.1f, 1000.0f);
        setMatrices(cam);

        glScalef(samples, samples, samples);
        
        // clear fbo
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw creatures using assigned renderers
        // BOOST_FOREACH(Renderer* r, assignedRenderers) {
        //     if(r->size() > 0)
        //         r->draw(&grid);
        // }

        // clean up
        popMatrices();  
        fbo.unbindFramebuffer();
        setViewport(Area(0, 0, getWindowWidth(), getWindowHeight()));
        
        // draw fbo to screen
        glEnable(GL_TEXTURE_2D);

        fbo.bindTexture();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        color(ColorA(1, 1, 1, 1));
        ci::gl::drawSolidRect(Rectf(0,0,getWindowWidth(), getWindowHeight()));
        
        fbo.unbindTexture();
        glDisable(GL_TEXTURE_2D);
        
    } else {
        // basic fbo-less rendering mode
        // BOOST_FOREACH(Renderer* r, assignedRenderers) {
        //     if(r->size() > 0)
        //         r->draw(&grid);
        // }
    }
     */
}

void WorkerTest::drawDebug()
{    
    // draw info overlay
    float h = getWindowHeight() * 0.5;
    drawText("FIELD | Worker Test", 15, h);
    
    std::stringstream frame;
    frame << "FRAME "<< state_.frame_;
    drawText(frame.str(), 15, h + 15);
    
    std::stringstream fps;
    fps << "FPS "<< getAverageFps() <<" PARTICLES "<< state_.lines_.size();
    drawText(fps.str(), 15, h + 30);
}
