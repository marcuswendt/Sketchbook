/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#pragma once

#include "cinder/Timer.h"

#include <vector>
#include <queue>
#include <boost/thread/thread.hpp>


namespace field {
    
    class IParam {
    public:
    };
    
    template <class T>
    class Param : public IParam {
    public:
        Param(std::string key, T value) : key_(key), value_(value) {}
        
        std::string key_; // currently unused
        T value_;
    };
    
    class Message {
    public:
        Message(int id=0) : id_(id) {}
        ~Message() 
        {
            // TODO leaks?
//            for(int i=0; i<params.size(); i++) {
//                delete &params[i];
//            }
            params.clear();
        }

        template <typename T>
        void Add(std::string key, T value)
        {
            params.push_back(new Param<T>(key, value));
        }
        
        template <typename T>
        T Get(int index)
        {
            Param<T>* p = static_cast<Param<T>*>(params[index]);
            return p->value_;
        }
        
        std::string Gets(int index) { return Get<std::string>(index); }
        
        int id() { return id_; }
        int size() { return params.size(); }

    private:
        int id_;
        std::vector<IParam*> params;
    };
    
    
    
    // base class for all state types
    class State {
    public: 
        State() {}
        ~State() {}
//        virtual void swap(State const& state) {}
    };
    
    
    
    // base class for all Task types
    template <class I, class O>
    class Task {
    public: 
        Task() : name_("Task"), isEnabled_(false) {}
        ~Task() {}
        
        virtual void Dispose() {}
        virtual void Notify(Message& message) {}
        
        void Process(I* input, O* output) 
        {
            if(!isEnabled_) return;
            
            timer_.stop();
            double dt = timer_.getSeconds();
            timer_.start();
            
            printf("%s FPS: %f \n", name_.c_str(), (1.0 / dt));
            Update(input, output);
        }
        
        std::string getName() { return name_; }
        
    protected:
        bool isEnabled_;
        std::string name_;
        ci::Timer timer_;
        
        void setName(std::string value) { name_ = value; }
        void setEnabled(bool value) { isEnabled_ = value; }
        virtual void Update(I* input, O* output) = 0;
    };
    
    
    template <class T, class I, class O>
    class Worker {
    public:
        Worker() : quit_(true), waitForInput_(true) {}
        ~Worker() 
        {
            Stop();
        }
        
        //! Starts the internal thread.
        void Start()
        {
            if (thread_.get_id() != boost::thread::id()) abort();
            
            for (size_t i = 0, il = outputs_in_.size(); i < il; ++i)
                outputs_in_.pop();
            
            for (size_t i = 0; i < 5; ++i)
                outputs_in_.push(O());
            
            quit_ = false;
            
            boost::thread thread(boost::thread(&Worker::ThreadMainThunk, &*this));
            thread_.swap(thread);
        }
        
        //! Stops the internal thread.
        void Stop()
        {
            if (thread_.get_id() == boost::thread::id()) abort();
            lock_.lock(); 
            quit_ = true; 
            lock_.unlock();
            
            thread_.join();
            
            boost::thread not_a_thread;
            thread_.swap(not_a_thread);
        }
        
        //! Ask for state for the next state step.  Returns true if it was available.
        bool Next(I& input, O& output)
        {           
            if(thread_.get_id() == boost::thread::id()) abort();
            
            lock_.lock();
            if(outputs_out_.size() == 0) {
                lock_.unlock();
                printf("Thread is behind\n");
                return false;
            }
            
            // Store the next input state in the queue.
//            inputs_in_.push(input);
            input_.swap(input);
            
            // Swap the output state back into the queue.
            outputs_in_.push(O());
            
            // Swap out the next output state and remove the empty replacement state.
            output.swap(outputs_out_.front());        
            outputs_out_.pop_front();
            
            lock_.unlock();
            return true;
        }
        
        void Notify(Message message)
        {
            printf("Worker#Notify\n");
            Pause();
            inbox_.push(message);
            Resume();
        }
        
        void SetWaitForInput(bool enabled)
        {
            Pause();
            waitForInput_ = enabled;
            Resume();
        }
        
    private:
        T* task_;
        
        boost::thread thread_;
        
        // Locks the variables below.
        boost::mutex lock_;
        bool quit_;
        bool waitForInput_;
        
        std::queue<Message> inbox_;
        
        I input_;
        // Input states not yet processed
//        std::queue<I> inputs_in_;
        
        // Output states to be recycled.
        std::queue<O> outputs_in_;
        
        // Output states that have been computed by the task.
        std::deque<O> outputs_out_;
        
        static void ThreadMainThunk(Worker<T, I, O>* worker)
        {
            worker->ThreadMain();
        }
        
        //! Internal use only - shouldnt be called directly
        void ThreadMain()
        {
            // create new task
            task_ = new T();
            
            while(!quit_) 
            {
                lock_.lock();
                
                if(quit_) {
                    printf("Background thread quitting.\n");
                    task_->Dispose();
                    lock_.unlock();
                    break;
                }
                
                // worker thread ahead of input.
                if (outputs_in_.size() == 0) {
                    // Wait until worker has new input
//                    if(waitForInput_) {
                        //                    printf("%s - Ahead of input\n", task_->getName().c_str());
                        lock_.unlock();
                        boost::this_thread::sleep(boost::posix_time::milliseconds(2));
                        continue;  // Try again

                    // Run as fast as you can
//                    } else {
//                        outputs_in_.push(O());
//                        outputs_out_.pop_front();
//                    }
                }
                
                // get input state
                I input = input_;
//                input.swap(inputs_in_.front());
//                inputs_in_.pop();
                
                // get output state to work on
                O output;
                output.swap(outputs_in_.front());
                outputs_in_.pop();
                
                // deliver messages
                if(inbox_.size() > 0) {
                    for (size_t i = 0, il = inbox_.size(); i < il; ++i) {
                        task_->Notify(inbox_.front());
                        inbox_.pop();
                    }
                }
                
                // update task
                task_->Process(&input, &output);
                                
                // push new output state onto queue
                outputs_out_.push_back(output);
                
                lock_.unlock();
            }
        }
        
        //! Pause the worker thread to access the task directly.
        void Pause()
        {
            if (thread_.get_id() == boost::thread::id()) abort();
            lock_.lock();
        }
        
        void Resume()
        {
            if (thread_.get_id() == boost::thread::id()) abort();
            lock_.unlock();
        }
    };
    
}