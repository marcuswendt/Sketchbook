/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#include "ControlTask.h"

namespace field {

#define SCRIPT_MAIN_NAME "Logic"
    
#define ENTER_SCOPE \
    if(!isEnabled_) return; \
    v8::Context::Scope contextScope(context_.getContext()); \
    v8::HandleScope handleScope; \
    try {
    
#define EXIT_SCOPE \
    } catch(std::exception& e) { \
        LOG_ERROR(e.what()); \
        isEnabled_ = false; \
        return; \
    }
    
    void ControlTask::Notify(Message& message)
    {
        int id = message.id();
        if(id==0) {
            v8::Locker locker;
            Init();
            Reload(message.Gets(0));
            v8::Unlocker unlocker;
        }
    }
    
    void ControlTask::Dispose()
    {
//        v8::Unlocker unlocker;
        v8::V8::Dispose();
    }
    
    void ControlTask::Init()
    {
//        v8::Locker locker;
        
        // attach script modules
        context_.add(new fk::script::Date());
        context_.add(new fk::script::Logger());
        context_.add(new fk::script::Random()); 
        context_.add(new fk::script::TypedArray());
        context_.add(new fk::script::Graphics2D());
        context_.add(new fk::script::Perlin());
    }
    
    void ControlTask::Update(NullState* input, ControlState* output) 
    {
//        printf("ControlTask#Update\n");   
        v8::Locker locker;
        
        // 'Live Coding'
        // periodically check for source file modifications and reload script if necessary    
        reloadTimer_++;
        if(reloadTimer_ > 30) {
            reloadTimer_ = 0;
            if(context_.filesModified()) {
                Reload();
                return;
            }
        }
        
        // call the Sketch JavaScript objects update method once per frame
        ENTER_SCOPE;
        context_.call(instance_, "update");

        // DEBUG
        output->frame_ = context_.call(instance_, "frame")->Int32Value();
        output->pos_.x = context_.call(instance_, "x")->NumberValue();
        output->pos_.y = context_.call(instance_, "y")->NumberValue();
        EXIT_SCOPE;
        
        v8::Unlocker unlocker;
    }
    
    void ControlTask::Reload(std::string script)
    {
        printf("ControlTask#Reload %s \n", script.c_str());

        setEnabled(true);
        reloadTimer_ = 0;
        
        // cleanup
        instance_.Dispose();
        
        // execute script
        try {
            context_.execute(script);
        } catch(std::exception& e) {
            LOG_ERROR(e.what());
            setEnabled(false);
            return;
        }
        
        // create new sketch instance
        ENTER_SCOPE;
        
        int argc = 0;
        v8::Handle<v8::Value> args[] = { 
//            Integer::New(ci::app::getWindowWidth()), 
//            Integer::New(ci::app::getWindowHeight())
        };
        
        v8::Handle<v8::Object> object = context_.newInstance(context_.getContext()->Global(), v8::String::New(SCRIPT_MAIN_NAME), argc, args);
        instance_ = v8::Persistent<v8::Object>::New(object);
        EXIT_SCOPE;
    }
}