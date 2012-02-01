/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#include "InterpolateTask.h"

#include "cinder/BSpline.h"

namespace field 
{    
    void InterpolateTask::Notify(Message& message)
    {
        int id = message.id();
        if(id==0) Init();
    }

    void InterpolateTask::Init()
    {
        setEnabled(true);
    }
    
    void InterpolateTask::Update(PhysicsState* input, InterpolateState* output)
    {
        output->frame_ = input->frame_;
        
        std::vector<Vec3f>::iterator it = input->lines_.begin();
        for(; it != input->lines_.end(); it++) {
            output->lines_.push_back(*it);
        } 
    }
}