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
#include "fieldkit/script/ScriptKit.h"

namespace field {
    
    class NullState : public State {
    public:
        void swap(NullState const& state) {}
    };
    
    class ControlState : public State {
    public:
        ControlState() : frame_(0), pos_(Vec2f::zero()) {}
        
        int frame_;
        Vec2f pos_;
        
        void swap(ControlState const& state) 
        {
            frame_ = state.frame_;
            pos_ = state.pos_;
        }
    };
    
    class ControlTask : public Task<NullState, ControlState> {
    public:
        ControlTask() 
        { 
            setName("ControlTask");
        }
        
        void Notify(Message& message);
        void Dispose();
        void Update(NullState* input, ControlState* output);
        
    private:
        int reloadTimer_;

	    fk::script::ScriptContext context_;
	    v8::Persistent<v8::Object> instance_;
        
        void Init();
	    void Reload(std::string script="");
    };
}
