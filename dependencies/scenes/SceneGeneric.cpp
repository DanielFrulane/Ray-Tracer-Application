#include "SceneGeneric.hpp"
#include <iostream>

# define ARBITRARY_HIGH_NUMBER 1e6

using namespace Eigen;

App::SceneGeneric::SceneGeneric() = default;

App::SceneGeneric::~SceneGeneric() = default;

void App::SceneGeneric::generateSceneObjects() {}

// checks if it is possible to properly render a scene given the current configuration
void App::SceneGeneric::checkIfHasAllNecessaryComponents() const {
    if (!m_hasCamera){
        throw std::invalid_argument("camera not configured");
    }
    if (m_lightsInScene.empty()){
        throw std::invalid_argument("no lights in scene");
    }
    if (m_objectsInScene.empty()){
        throw std::invalid_argument("no objects in scene");
    }
}