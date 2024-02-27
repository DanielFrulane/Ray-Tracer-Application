#include "RTMotorSimple.hpp"

App::RTMotorSimple::RTMotorSimple(){
    m_currentTileIndex = 0;
}

App::RTMotorSimple::~RTMotorSimple() = default;

// tests if a ray cast within the scene intersects with an object
bool App::RTMotorSimple::castRay(App::Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject,
                                   HitInformation &closestHitInformation) {

    HitInformation hitInformation;
    double localMinimalDistance = ARBITRARY_HIGH_NUMBER;
    bool intersectionFound = false;

    for (std::shared_ptr<ObjectGeneric> &currentObject: m_scene.m_objectsInScene) { // for all objects
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

// base function to progressively generate image in tiles
void App::RTMotorSimple::iterateTiles(std::vector<App::TileInformation>& listOfTiles,
                                        std::vector<std::atomic<int> *>& listOfTileFlags) {
    if (m_currentTileIndex < listOfTiles.size() && listOfTileFlags.at(m_currentTileIndex)->load(std::memory_order_acquire) == TILE_NOT_RENDERED){
        renderTileFromIteration(&listOfTiles.at(m_currentTileIndex), listOfTileFlags.at(m_currentTileIndex));
        m_currentTileIndex++;
    }
}

// editing flags
void App::RTMotorSimple::renderTileFromIteration(App::TileInformation *tile, std::atomic<int> *tileFlag) {
    tileFlag->store(TILE_RENDERING,std::memory_order_release);
    renderTile(tile);
    tileFlag->store(TILE_RENDERED,std::memory_order_release);
}

// renders a tile from the image
void App::RTMotorSimple::renderTile(App::TileInformation *tile) {
    Vector3d pixelColor = {1.,0.5,0.2};
    for (int y = 0; y < tile->height; ++y) {
        for (int x = 0; x < tile->width; ++x) {
            pixelColor = renderPixel(tile->x+x,tile->y+y,m_scene.m_width,m_scene.m_height);
            tile->rgb.at(convertCoordinatesToLinearIndex(x,y,tile->width,tile->height)) = pixelColor;
        }
    }
}

// renders a pixel from the image by casting rays
Vector3d App::RTMotorSimple::renderPixel(int x, int y, int width, int height) {
    std::shared_ptr<ObjectGeneric> closestObject;
    App::Ray cameraRay;
    double xFactor = 1.0 / (static_cast<double>(width)/2.0);
    double yFactor = 1.0 / (static_cast<double>(height)/2.0);
    double normX = (static_cast<double>(x) * xFactor) - 1.0;
    double normY = (static_cast<double>(y) * yFactor) - 1.0;
    m_scene.m_camera.generateRay(normX, normY, cameraRay);
    HitInformation hitInformation;
    bool intersectionFound = castRay(cameraRay, closestObject, hitInformation);
    Vector3d color;
    if (intersectionFound){
        MaterialGeneric::m_currentNumberOfReflections = 0; // resets reflection count
        // gets color from material
        color = hitInformation.hitObject->m_material->calculateColor(m_scene.m_objectsInScene, m_scene.m_lightsInScene, hitInformation.hitObject, hitInformation.pointOfIntersection, hitInformation.normal, hitInformation.localPointOfIntersection, hitInformation.uvCoordinates, cameraRay);
    }else{
        color = {0.0,0.0,0.0}; // dark
    }
    return color;
}