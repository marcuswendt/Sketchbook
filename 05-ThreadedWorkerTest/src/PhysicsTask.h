/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#pragma once

#include "Worker.h"
#include "ControlTask.h"

#include "cinder/Timer.h"
#include "fieldkit/physics/PhysicsKit.h"

namespace field {

//    struct Line {
//        Line(Vec3f a, Vec3f b) : a_(a), b_(b) {}
//        Vec3f a_;
//        Vec3f b_;
//    };
    
    class PhysicsState : public State {
    public:
        int frame_;
        std::vector<Vec3f> lines_;
        
        void swap(PhysicsState const& state) 
        {
            frame_ = state.frame_;
            lines_ = state.lines_;
        }
    };
    
    class PhysicsTask : public Task<ControlState, PhysicsState> {
    public:
        PhysicsTask() 
        {
            setName("PhysicsTask");
        }
        
        void Notify(Message& message);
        void Update(ControlState* input, PhysicsState* output);
        
    private:        
        ci::Timer timer_;
        fk::physics::Physics* physics_;
        fk::physics::AttractorPoint* attractor_;
        
        void Init();
    };
}
