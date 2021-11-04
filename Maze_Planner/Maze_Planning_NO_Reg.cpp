#include "fcl/narrowphase/collision.h"
#include "fcl/broadphase/broadphase_collision_manager.h"
#include "fcl/broadphase/broadphase_dynamic_AABB_tree.h"
#include "fcl/broadphase/default_broadphase_callbacks.h"
#include <ompl/geometric/planners/rrt/RRTstar.h>
#include <ompl/geometric/planners/rrt/SORRTstar.h>
#include <ompl/base/SpaceInformation.h>
#include <ompl/base/objectives/PathLengthOptimizationObjective.h>
#include <ompl/base/objectives/StateCostIntegralObjective.h>
#include <ompl/base/objectives/MaximizeMinClearanceObjective.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include "./json.hpp"
#include <cstdio>
#include <string>

using namespace fcl;
namespace ob = ompl::base;
namespace og = ompl::geometric;

using CollisionGeometryPtr_t = std::shared_ptr<fcl::CollisionGeometry<float>>;

void build_maze(std::string filename,float maze_length, float maze_width,BroadPhaseCollisionManagerf * manager);

void build_walls(int length, int width, BroadPhaseCollisionManagerf * manager);

CollisionObject<float> * initialize_bot(float x, float y, float radius, BroadPhaseCollisionManagerf * manager);

ob::OptimizationObjectivePtr getPathLengthObjective(const ob::SpaceInformationPtr& si);

class ValidityChecker : public ob::StateValidityChecker
{
public:
    BroadPhaseCollisionManagerf* maze_manager;
    BroadPhaseCollisionManagerf* bot_manager;
    mutable CollisionObject<float> * bot;
    float radius;
    ValidityChecker(const ob::SpaceInformationPtr& si):ob::StateValidityChecker(si) {
    
    maze_manager = new DynamicAABBTreeCollisionManagerf();
    bot_manager = new DynamicAABBTreeCollisionManagerf();
    build_maze("cl_obj.json",100,100,maze_manager);
    float posx = 3;
    float posy = 85;
    radius = 1;
    bot = initialize_bot(posx,posy,radius,bot_manager);
    }


     bool isValid(const ob::State* state) const
    {   

        double pt = this->clearance(state);
        std:: cout << "Collision Points:" << pt << std::endl;
        return pt  == 0.0;
    }

    double clearance(const ob::State* state) const
    {
        const ob::RealVectorStateSpace::StateType* state2D =
            state->as<ob::RealVectorStateSpace::StateType>();
 
        float x = static_cast<float>(state2D->values[0]);
        float y = static_cast<float>(state2D->values[1]);
        bot->setTranslation(Vector3<float> (x, y, 0));
        bot->computeAABB();
        bot_manager->update();
        // std:: cout << "bot_size:" <<  bot_manager ->size() << std::endl;
        DefaultCollisionData<float> collision_data;
        bot_manager->collide(maze_manager,&collision_data,DefaultCollisionFunction<float>);
        
        double min_d = collision_data.result.numContacts();
        return min_d;
    }
};



int main(){
    auto start_time = std::chrono::steady_clock::now();

    ob::StateSpacePtr space(new ob::RealVectorStateSpace(2));
 
    // Set the bounds of space to be in [0,100].
    space->as<ob::RealVectorStateSpace>()->setBounds(0.0, 100.0);

    ob::SpaceInformationPtr si(new ob::SpaceInformation(space));


    si->setStateValidityCheckingResolution(0.001);


    si->setStateValidityChecker(ob::StateValidityCheckerPtr(new ValidityChecker(si)));
 
    si->setup();

    // Set our robot's starting state to be the bottom-left corner of
    // the environment, or (0,0).
    ob::ScopedState<> start(space);
    start->as<ob::RealVectorStateSpace::StateType>()->values[0] = 3.0;
    start->as<ob::RealVectorStateSpace::StateType>()->values[1] = 85.0;
    
    // Set our robot's goal state to be the top-right corner of the
    // environment, or (1,1).
    ob::ScopedState<> goal(space);
    goal->as<ob::RealVectorStateSpace::StateType>()->values[0] = 90.0;
    goal->as<ob::RealVectorStateSpace::StateType>()->values[1] = 45.0;

    
    // Create a problem instance
    ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));
    
    // Set the start and goal states
    pdef->setStartAndGoalStates(start, goal);

    // Construct our optimizing planner using the RRTstar algorithm.
    ob::PlannerPtr optimizingPlanner(new og::RRTstar(si));
    
    // Set the problem instance for our planner to solve
    optimizingPlanner->setProblemDefinition(pdef);
    optimizingPlanner->setup();
    
    // attempt to solve the planning problem within one second of
    // planning time
    ob::PlannerStatus solved = optimizingPlanner->solve(5);

    std::cout <<"Solve Status:"<< bool(solved) << std::endl;

    if (solved){

    std::ostream &out = std::cout;


    pdef-> getSolutionPath()->print(out);

    std::string outputFile = "out.txt";

    std::ofstream outFile(outputFile.c_str());
    std::static_pointer_cast<og::PathGeometric>(pdef->getSolutionPath())->
            printAsMatrix(outFile);
    outFile.close();

    }
    else{

    std::cout << "failed" << std::endl;
    }
    auto end_time = std::chrono::steady_clock::now();
    auto diff = end_time - start_time;
    std::cout << "\n";
    std::cout <<"Runtime: "<< std::chrono::duration <double, std::milli> (diff).count() << " ms\n" << std::endl;
}


void build_walls(float length, float width, BroadPhaseCollisionManagerf * manager){
    const float BOX_HEIGHT = 100; //ignore 3rd dimision
    const float ZERO = 0; //0 
   
    std::cout << "\nInitialize Enviroment...\n\n Boundary Length: " << length << '\n' <<" Boundary Height: " << width << '\n' << std::endl;
    
    try{
    //left wall
    CollisionGeometryPtr_t lw(new Box<float>(1,width,BOX_HEIGHT));
    Transform3<float> tlw(Translation3<float>(Vector3<float>(-0.5,width * 0.5,ZERO)));
    CollisionObject<float> *blw = new CollisionObject<float>(lw,tlw);
    manager->registerObject(blw);

    //right wall
    CollisionGeometryPtr_t rw(new Box<float>(1,width,BOX_HEIGHT));
    Transform3<float> trw(Translation3<float>(Vector3<float>(width + 0.5,width * 0.5,ZERO)));
    CollisionObject<float> *brw = new CollisionObject<float>(rw,trw);
    manager->registerObject(brw);

    //top wall
    CollisionGeometryPtr_t bw(new Box<float>(length,1,BOX_HEIGHT));
    Transform3<float> tbw(Translation3<float>(Vector3<float>(length * 0.5,-0.5,ZERO)));
    CollisionObject<float> *bbw = new CollisionObject<float>(bw,tbw);
    manager->registerObject(bbw);

    //bottom wall
    CollisionGeometryPtr_t tw(new Box<float>(length,1,BOX_HEIGHT));
    Transform3<float> ttw(Translation3<float>(Vector3<float>(length * 0.5,width + 0.5,ZERO)));
    CollisionObject<float> *btw = new CollisionObject<float>(tw,ttw);
    manager->registerObject(btw);

    std::cout << "Initialize Success!\n"<< std::endl;
    }catch(...){
        std::cout << "Initialization Failed"<<std::endl;
        exit(1);
    }
}


void build_maze(std::string filename,float maze_length, float maze_width, BroadPhaseCollisionManagerf* manager){

    const float BOX_HEIGHT = 100; //ignore 3rd dimision
    const float ZERO = 0; //0 

   
    //Build Walls Around
    build_walls(maze_length,maze_width,manager);

    //READ JSON File
    std::cout << "\nReading JSON file...." <<std::endl;
    std::ifstream i(filename);
    nlohmann::json j;
    i >> j;
    
        
    //Build Maze
    int ctr = 1;
    for (auto c:j["col_obj"]){
        try{
        //Retrive data for object creation
        float length, width, x , y;
        length = static_cast<float>(c["length"]);
        width = static_cast<float>( c["width"]);
        x = static_cast<float>(c["x"]);
        y = static_cast<float>(c["y"]);
        //Buidling object;
        std:: cout << "\nCreating NO." << ctr << " rectangle:" << std::endl;
        std::printf("Centered at (%.2f,%.2f) with length: %.2f, width: %.2f\n",x,y,length,width);
        CollisionGeometryPtr_t b(new Box<float>(length,width,BOX_HEIGHT));
        Transform3<float> tf(Translation3<float>(Vector3<float>(x,y,ZERO)));
        CollisionObject<float> *box = new CollisionObject<float> (b,tf);
        manager->registerObject(box);
        ctr++;}catch(...){
            continue;
        }
    }
}


CollisionObject<float> * initialize_bot(float x, float y,float radius, BroadPhaseCollisionManagerf * manager){
        const float CYLINDER_HEIGHT = 100;
        const float ZERO = 0;
        std:: cout << "\nInitializing bot at (" << x << ","  << y << ") " << "with radius: " << radius  << "\n" <<std::endl;
        CollisionGeometryPtr_t c(new Cylinder<float>(radius,CYLINDER_HEIGHT));
        Transform3<float> tf(Translation3<float>(Vector3<float>(x,y,ZERO)));
        CollisionObject<float> *bot = new CollisionObject<float> (c,tf);
        manager->registerObject(bot);
        return bot;
}

