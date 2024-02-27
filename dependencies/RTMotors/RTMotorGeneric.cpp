#include "RTMotorGeneric.hpp"

// this is the superclass responsible for the conversion from the scene to a coherent image

App::RTMotorGeneric::RTMotorGeneric() = default;

App::RTMotorGeneric::~RTMotorGeneric() = default;

void App::RTMotorGeneric::setScene(const App::SceneGeneric &scene) {
    m_scene = scene;
}

// to be overridden with the treatment of the scene
void App::RTMotorGeneric::iterateTiles(std::vector<App::TileInformation> &listOfTiles,
                                       std::vector<std::atomic<int> *> &listOfTileFlags) {}


// 2D to 1D conversion
int App::RTMotorGeneric::convertCoordinatesToLinearIndex(int x, int y, int width, int height) {
    if ((x<width) && (x>= 0) && (y<height) && (y >= 0)){
        return (y*width) + x;
    }
    return -1;
}