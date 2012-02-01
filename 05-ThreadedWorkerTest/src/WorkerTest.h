/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "fieldkit/ConfigDict.h"

#include "ControlTask.h"
#include "PhysicsTask.h"
#include "InterpolateTask.h"

using namespace ci::app;

namespace field {
        
    //! Main class for the Petite Salle installation
    class WorkerTest : public AppBasic {
    public:
        WorkerTest() {};
        ~WorkerTest() {};
        
        void prepareSettings(Settings *settings);
        void setup();
        void shutdown();
        void update();
        void draw();
        
    private:
        ConfigDict config;
        
        InterpolateState state_;
        
        Worker<ControlTask, NullState, ControlState> controlWorker;
        Worker<PhysicsTask, ControlState, PhysicsState> physicsWorker;
        Worker<InterpolateTask, PhysicsState, InterpolateState> interpolateWorker;
        
        // events
        void keyDown(KeyEvent event);
        void mouseDown(MouseEvent event);
        
        // drawing
        float framerate;
        int samples;
        ci::gl::Fbo fbo;
        vector<Renderer*> renderers;

        bool showDebug;
        
        // recording
        bool isRecording;
        bool recordFrame;
        int frame;
        
        void drawScene();
        void drawDebug();
    };
	
}
