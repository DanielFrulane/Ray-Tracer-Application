#include "SceneGeneric.hpp"
#include <iostream>

# define ARBITRARY_HIGH_NUMBER 1e6

using namespace Eigen;

App::SceneGeneric::SceneGeneric() = default;

App::SceneGeneric::~SceneGeneric() = default;

void App::SceneGeneric::generateSceneObjects() {}

// checks if it is possible to properly render a scene given the current configuration
void App::SceneGeneric::checkIfHasAllNecessaryComponents() {
    if (!m_hasCamera){
        throw std::invalid_argument("camera not configured");
    }
    if (m_lightsInScene.empty()){
        throw std::invalid_argument("no lights in scene");
    }
    if (m_objectsInScene.empty()){
        throw std::invalid_argument("no objects in scene");
    }
    for (const std::shared_ptr<ObjectGeneric>& currentObject : m_objectsInScene){
        if (currentObject->m_material == nullptr){
            throw std::invalid_argument("no material in object");
        } else {
            if (currentObject->m_material->m_texture == nullptr){
                throw std::invalid_argument("no texture in material");
            }
        }
    }
}

int App::SceneGeneric::convertCoordinatesToLinearIndex(int x, int y, int width, int height) {
    if ((x<width) && (x>= 0) && (y<height) && (y >= 0)){
        return (y*width) + x;
    }
    return -1;
}

// tests if a ray cast within the scene intersects with an object
bool App::SceneGeneric::castRay(App::Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject,
                                HitInformation &closestHitInformation) {
    HitInformation hitInformation;
    double localMinimalDistance = ARBITRARY_HIGH_NUMBER;
    bool intersectionFound = false;

    for (std::shared_ptr<ObjectGeneric> &currentObject: m_objectsInScene) { // for all objects
        bool isValidIntersection = currentObject->isIntersecting(castedRay, hitInformation);
        if (isValidIntersection){
            intersectionFound = true;

            double distanceFromCameraToIntersection = (hitInformation.pointOfIntersection - castedRay.m_point1).norm();
            if (distanceFromCameraToIntersection < localMinimalDistance){
                localMinimalDistance = distanceFromCameraToIntersection;
                closestObject = currentObject;
                closestHitInformation = hitInformation;
            }
        }
    }
    return intersectionFound;
}

// renders a tile from the image
void App::SceneGeneric::renderTile(App::TileInformation *tile) {
    Vector3d pixelColor;
    for (int y = 0; y < tile->height; ++y) {
        for (int x = 0; x < tile->width; ++x) {
            pixelColor = renderPixel(tile->x+x,tile->y+y,m_width,m_height);
            tile->rgb.at(convertCoordinatesToLinearIndex(x,y,tile->width,tile->height)) = pixelColor;
        }
    }
}

// renders a pixel from the image by casting rays
Vector3d App::SceneGeneric::renderPixel(int x, int y, int width, int height) {
    std::shared_ptr<ObjectGeneric> closestObject;
    App::Ray cameraRay;
    double xFactor = 1.0 / (static_cast<double>(width)/2.0);
    double yFactor = 1.0 / (static_cast<double>(height)/2.0);
    double normX = (static_cast<double>(x) * xFactor) - 1.0;
    double normY = (static_cast<double>(y) * yFactor) - 1.0;

    m_camera.generateRay(normX, normY, cameraRay);

    HitInformation hitInformation;
    bool intersectionFound = castRay(cameraRay, closestObject, hitInformation);
    Vector3d color;
    if (intersectionFound){
        MaterialGeneric::m_currentNumberOfReflections = 0; // resets reflection count
        // gets color from material
        color = hitInformation.hitObject->m_material->calculateColor(m_objectsInScene, m_lightsInScene, hitInformation.hitObject, hitInformation.pointOfIntersection, hitInformation.normal, hitInformation.localPointOfIntersection, hitInformation.uvCoordinates, cameraRay);
    }else{
        color = {0.0,0.0,0.0}; // dark
    }
    return color;
}
