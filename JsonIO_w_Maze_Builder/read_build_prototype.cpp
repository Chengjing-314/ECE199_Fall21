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
using namespace fcl;

const float BOX_HEIGHT = 100; //ignore 3rd dimision
const float ZERO = 0; //0 

int main(){
    auto start = std::chrono::steady_clock::now();

    //READ JSON FIle
    std::cout << "\nReading JSON file" <<std::endl;
    std::ifstream i("cl_obj.json");
    nlohmann::json j;
    i >> j;

    //Creating Collision Objectes
    using CollisionGeometryPtr_t = std::shared_ptr<fcl::CollisionGeometry<float>>;
    BroadPhaseCollisionManagerf* maze_manager = new DynamicAABBTreeCollisionManagerf();
    int ctr = 1;
    for (auto c:j["col_obj"]){
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
        maze_manager->registerObject(box);
        ctr++;
    }



    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "\n";
    std::cout <<"Runtime: "<< std::chrono::duration <double, std::milli> (diff).count() << " ms\n" << std::endl;
}
