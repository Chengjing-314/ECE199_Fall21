cmake_minimum_required(VERSION 3.10)
project(General_collision_test)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/usr/local/include)

add_executable(collision broad_phase_collision.cpp)

target_link_libraries(collision  /usr/local/lib/libccd.a
                        /usr/local/lib/libfcl.so)



