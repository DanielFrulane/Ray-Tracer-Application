#include "MaterialGeneric.hpp"

#define MAXIMUM_NUMBER_OF_REFLECTIONS 3;
#define ARBITRARY_HIGH_NUMBER 1e6

App::MaterialGeneric::MaterialGeneric() {
    m_maximumNumberOfReflections = MAXIMUM_NUMBER_OF_REFLECTIONS;
    m_currentNumberOfReflections = 0;
}

App::MaterialGeneric::~MaterialGeneric() = default;

// texture setter
void App::MaterialGeneric::setTexture(const std::shared_ptr<Textures::TextureGeneric> &newTexture) {
    if (m_texture == nullptr){
        m_texture = newTexture;
    } else {
        throw std::invalid_argument("object already has texture");
    }
}

// casts a ray into an object to see how it affects the color
bool App::MaterialGeneric::castRay(const App::Ray &castRay, const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                   const std::shared_ptr<ObjectGeneric> &thisObject,
                                   std::shared_ptr<ObjectGeneric> &closestObject, HitInformation &closestHitInformation) {
    HitInformation hitInformation;
    double minimalDistance = ARBITRARY_HIGH_NUMBER;
    bool isIntersectionFound = false;
    // will iterate all objects in scene to find the closest
    for (const std::shared_ptr<ObjectGeneric>& currentObject : objectList){
        if (currentObject != thisObject){
            bool isValidIntersection = currentObject -> isIntersecting(castRay, hitInformation);
            if (isValidIntersection) {
                isIntersectionFound = true;
                double distance = (hitInformation.pointOfIntersection - castRay.m_point1).norm();
                // confirms the closest
                if (distance < minimalDistance){
                    minimalDistance = distance;
                    closestObject = currentObject;
                    closestHitInformation = hitInformation;
                }
            }
        }
    }
    return isIntersectionFound;
}

// will call other color calculation functions
Vector3d App::MaterialGeneric::calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                              const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                              const std::shared_ptr<ObjectGeneric> &currentObject,
                                              const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                              const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                              const App::Ray &cameraRay) {
    return {0.0,0.0,0.0};
}

// calculates diffuse color
Vector3d App::MaterialGeneric::calculateDiffuseColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                     const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                     const std::shared_ptr<ObjectGeneric> &currentObject,
                                                     const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                     const Vector3d &localColor) {
    double intensity;
    Vector3d diffuseColor = {0.0,0.0,0.0};
    Vector3d color = {0.0,0.0,0.0};
    Vector3d rgb = {0.0,0.0,0.0};
    bool isIlluminationFound = false;
    // for all lights in scene
    for (const std::shared_ptr<LightGeneric>& currentLight : lightList) {
        bool isIlluminated = currentLight->calculateIllumination(intersectionPoint, localNormal, objectList, currentObject, color, intensity);
        if (isIlluminated){
            isIlluminationFound = true;
            rgb += color*intensity; // considers intensity of light
        }
    }
    if (isIlluminationFound){
        diffuseColor(0) = rgb(0) * localColor(0);
        diffuseColor(1) = rgb(1) * localColor(1);
        diffuseColor(2) = rgb(2) * localColor(2);
    } else {
        diffuseColor = {0.0,0.0,0.0};
    }
    return diffuseColor;
}

// calculates reflection color
Vector3d App::MaterialGeneric::calculateReflectionColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                        const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                        const std::shared_ptr<ObjectGeneric> &currentObject,
                                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                        const App::Ray &incidentRay) {
    Vector3d materialColor = {0.0,0.0,0.0};
    Vector3d reflectionColor;
    Vector3d direction = incidentRay.m_orientation;
    Vector3d reflection = direction - (2 * direction.dot(localNormal) * localNormal); // reflected ray orientation

    Ray reflectionRay = Ray(intersectionPoint, intersectionPoint + reflection);
    std::shared_ptr<ObjectGeneric> closestObject;
    HitInformation closestHitInformation;

    bool isIntersectionFound = castRay(reflectionRay, objectList, currentObject, closestObject, closestHitInformation);
    if (isIntersectionFound && (m_currentNumberOfReflections < m_maximumNumberOfReflections)){
        m_currentNumberOfReflections++;
        // gets the color from the texture class
        materialColor = closestHitInformation.hitObject->m_material->calculateColor(objectList, lightList, closestHitInformation.hitObject, closestHitInformation.pointOfIntersection, closestHitInformation.normal, closestHitInformation.localPointOfIntersection, closestHitInformation.uvCoordinates, reflectionRay);
    }
    // else: no change to color

    reflectionColor = materialColor;
    return reflectionColor;
}