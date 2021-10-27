#include <iostream>
#include <fstream>
#include "./json.hpp"

int main(){
    std::ifstream i("cl_obj.json");
    nlohmann::json j;
    i >> j;
    std::cout << j["col_obj"] << std::endl;

    for (auto p: j["col_obj"]){
        //std::cout << p["width"] << std::endl;
        float i = p["length"];
        std::cout << i << std::endl;
    }
}