
#include "fcl/narrowphase/collision.h"
#include "fcl/broadphase/broadphase_collision_manager.h"
#include "fcl/broadphase/broadphase_dynamic_AABB_tree.h"
#include "fcl/narrowphase/detail/gjk_solver_indep.h"
#include "fcl/narrowphase/detail/gjk_solver_libccd.h"
#include "fcl/narrowphase/detail/traversal/collision_node.h"
#include "fcl/narrowphase/continuous_collision.h"
#include <vector>
#include <iostream>
#include <chrono>
using namespace fcl;

int main(){
    auto start = std::chrono::steady_clock::now();
    detail::GJKSolver_libccd<float> solver; //do not use this
    using CollisionGeometryPtr_t = std::shared_ptr<fcl::CollisionGeometry<float>>;
    CollisionGeometryPtr_t b1(new Box<float>(1,1,1));
    CollisionGeometryPtr_t b2(new Box<float>(1,1,1));
    Transform3<float> tf1(fcl::Translation3<float>(fcl::Vector3<float> (3., 0, 0)));
    Transform3<float> tf2(fcl::Translation3<float>(fcl::Vector3<float> (3., 0, 0)));
    CollisionObject<float> box1 (b1, tf1);
    CollisionObject<float> box2 (b2, tf2);
    CollisionRequest<float> request;
    request.gjk_solver_type = fcl::GST_LIBCCD;
    CollisionResult<float> result;
    collide<float>(&box1, &box2,request, result);
    std::cout << result.isCollision() <<std::endl;
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
}


//request config-- 
//rigid position -- forward kinematics 

