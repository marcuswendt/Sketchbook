/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Created by Marcus Wendt on 31/01/2012.
 */

#include "PhysicsTask.h"

#include "cinder/BSpline.h"

namespace field 
{    
    using namespace fk::physics;
    
    void PhysicsTask::Notify(Message& message)
    {
        int id = message.id();
        if(id==0) Init();
    }

    void PhysicsTask::Update(ControlState* input, PhysicsState* output)
    {
        // input
        timer_.stop();
        double dt = timer_.getSeconds();
        timer_.start();

//        printf("PhysicsTask#Update - frame %i dt %f \n", input->frame_, dt);

        attractor_->setPosition(Vec3f(input->pos_.x, input->pos_.y, 0));
        
        
        // update
        physics_->update(dt);

        
        // output
        output->frame_ = input->frame_;
        
        std::vector<Spring*>::iterator sit = physics_->springs.begin();
        for(; sit != physics_->springs.end(); sit++) {
            Spring* s = (*sit);
            output->lines_.push_back(s->a->position);
            output->lines_.push_back(s->b->position);
        }
    }
    
    void PhysicsTask::Init()
    {        
//        Space* space = new BasicSpace(Vec3f(0,0,0), Vec3f(1280, 768, 768));
        Space* space = new SpatialHash(Vec3f(0,0,0), Vec3f(1280, 768, 768), 5.0f);
        
        physics_ = new Physics(space);
        
//        FixedRadiusNeighbourUpdate* nbUpdate = new FixedRadiusNeighbourUpdate();
//        nbUpdate->setRadius(25.0f);
//        physics_->setNeighbourUpdate(nbUpdate);
        
        
        // Behaviours
        physics_->addBehaviour(new Gravity());
        
        attractor_ = new AttractorPoint(space);
        attractor_->setRange(0.25); // relative to space dimensions
        attractor_->setWeight(1.0);
        physics_->addBehaviour(attractor_);
        
        FlockAttract* attr = new FlockAttract(space);
        attr->setRange(0.1);
        attr->setWeight(0.01);
//        physics_->addBehaviour(attr);    

        FlockAlign* align = new FlockAlign(space);
        align->setRange(0.025);
        align->setWeight(0.1);
//        physics_->addBehaviour(align);    

        FlockRepel* repel = new FlockRepel(space);
        repel->setRange(0.01);
        repel->setWeight(1.0);
//        physics_->addBehaviour(repel);  
        
        physics_->addConstraint(new PlaneConstraint(AXIS_Z, 0.0));
        
        // ----------------------------------------------------------------
        // create particles
        float width = 100;
        int res = 20 * 1000;
        
        float sw = physics_->space->getWidth();
        float sh = physics_->space->getHeight();
        
        std::vector<Vec3f> points;
        points.push_back(Vec3f(sw*0.2, sh*0.5, 0.0));
        points.push_back(Vec3f(sw*0.8, sh*0.5, 0.0));
        ci::BSpline3f spline(points, 1, false, false);
        
        for(int i=0; i<res; i++) {
            Vec3f pos = spline.getPosition( (float)i / (float)res );
            
            Particle* a = new Particle();
            a->init(pos + Vec3f(0,-width*0.5,0));
            a->lifeTime = Particle::LIFETIME_PERPETUAL;
            a->size = 5;
            physics_->addParticle(a);
            
            Particle* b = new Particle();
            b->init(pos + Vec3f(0,width*0.5,0));
            b->lifeTime = Particle::LIFETIME_PERPETUAL;
            b->size = 5;
            physics_->addParticle(b);
        }
        
#define CREATE_SPRING(p1, p2, weight) \
physics_->addSpring(new Spring(p1, p2, p1->position.distance(p2->position), weight)); 
        
#define CREATE_SPRING_STRETCHED(p1, p2, weight, scale) \
physics_->addSpring(new Spring(p1, p2, p1->position.distance(p2->position) * scale, weight)); 
        
        // connect particles with springs
        for(int i=0; i<res; i++) {
            // vertical
            Particle* a = physics_->particles[i*2];
            Particle* b = physics_->particles[i*2+1];
            
            CREATE_SPRING(a, b, 0.1);
            
            if(i < res-1) {
                Particle* a2 = physics_->particles[(i+1)*2];
                Particle* b2 = physics_->particles[(i+1)*2+1];
                
                // horizontal
                CREATE_SPRING(a, a2, 0.01);
                CREATE_SPRING(b, b2, 0.01);
                
                // diagonal
                CREATE_SPRING(a, b2, 0.01);
                CREATE_SPRING(b, a2, 0.01);
            }
        }
        
        // add springs between multiple segments
        //    int step = 5;
        //    for(int i=0; i<res-1; i += step) {
        int i = 0;
        int step = res-1;
        
        Particle* a1 = physics_->particles[i*2];
        Particle* b1 = physics_->particles[i*2+1];
        
        Particle* a2 = physics_->particles[(i+step)*2];
        Particle* b2 = physics_->particles[(i+step)*2+1];
        
        CREATE_SPRING_STRETCHED(a1, b2, 0.25, 1.15);
        CREATE_SPRING_STRETCHED(b1, a2, 0.25, 1.15);
        //    }
        
        
        setEnabled(true);
    }
}