#include "fcl/narrowphase/collision.h"
#include "fcl/broadphase/broadphase_collision_manager.h"
#include "fcl/broadphase/broadphase_dynamic_AABB_tree.h"
#include "fcl/narrowphase/detail/gjk_solver_indep.h"
#include "fcl/narrowphase/detail/gjk_solver_libccd.h"
#include "fcl/narrowphase/detail/traversal/collision_node.h"
#include "fcl/narrowphase/continuous_collision.h"
#include "fcl/broadphase/default_broadphase_callbacks.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include "./json.hpp"
#include <cstdio>
#include <string>
using namespace fcl;
using CollisionGeometryPtr_t = std::shared_ptr<fcl::CollisionGeometry<float>>;


void build_maze(std::string filename,float maze_length, float maze_width,BroadPhaseCollisionManagerf * manager);

void build_walls(int length, int width, BroadPhaseCollisionManagerf * manager);

int main(){
    auto start = std::chrono::steady_clock::now();

    BroadPhaseCollisionManagerf* maze_manager = new DynamicAABBTreeCollisionManagerf();

    build_maze("cl_obj.json",500,600,maze_manager);

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
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



