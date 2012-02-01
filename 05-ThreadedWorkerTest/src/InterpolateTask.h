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
#include "PhysicsTask.h"

namespace field {
    
        
    class InterpolateState : public State {
    public:
        InterpolateState() : frame_(0) {}
        
        int frame_;
        std::vector<Vec3f> lines_;
        
        void swap(InterpolateState const& state) 
        {
            frame_ = state.frame_;
            lines_ = state.lines_;
        }
    };
    
    
    class InterpolateTask : public Task<PhysicsState, InterpolateState> {
    public:
        InterpolateTask()
        {
            setName("InterpolateTask");
        }
        
        void Notify(Message& message);
        void Update(PhysicsState* input, InterpolateState* output);
        
    private:
        void Init();
    };
}
