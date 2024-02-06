#include "LightSource.hpp"

App::LightSource::LightSource() {

}

App::LightSource::~LightSource() {

}

bool App::LightSource::CalculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                             const std::vector<std::shared_ptr<App::ObjectGeneric>> &objectsInScene,
                                             const std::shared_ptr<App::ObjectGeneric> &object, Vector3d &color,
                                             double &intensity) {
    return false;
}
