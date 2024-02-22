#include "Application.hpp"
#include "include/SDL2/SDL.h"
#include "include/Eigen/Eigen"
#include <iostream>

#undef main

using namespace Eigen;

int main(int argc, char* argv[]){
    std::cout<<"Running"<<std::endl;
    Application app;
    return app.inExecution();
}