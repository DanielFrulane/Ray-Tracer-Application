#include "MaterialGeneric.hpp"

#define MINIMAL_DISTANCE 1e6

App::MaterialGeneric::MaterialGeneric() {
    m_maximumNumberOfReflections = 3;
    m_currentNumberOfReflections = 0;
}

App::MaterialGeneric::~MaterialGeneric() {

}

Vector3d App::MaterialGeneric::calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                              const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                              const std::shared_ptr<ObjectGeneric> &currentObject,
                                              const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                              const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                              const App::Ray &cameraRay) {
    return {0.0,0.0,0.0};
}

Vector3d App::MaterialGeneric::calculateDiffuseColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                     const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                     const std::shared_ptr<ObjectGeneric> &currentObject,
                                                     const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                     const Vector3d &localColor) {
    Vector3d diffuseColor = {0.0,0.0,0.0};
    double intensity;
    Vector3d color = {0.0,0.0,0.0};
    Vector3d rgb = {0.0,0.0,0.0};
    bool illuminationFound = false;
    for (const std::shared_ptr<LightGeneric>& currentLight : lightList) {
        bool isIlluminated = currentLight->calculateIllumination(intersectionPoint, localNormal, objectList, currentObject, color, intensity);
        if (isIlluminated){
            illuminationFound = true;
            rgb += color*intensity;
        }
    }
    if (illuminationFound){
        diffuseColor(0) = rgb(0) * localColor(0);
        diffuseColor(1) = rgb(1) * localColor(1);
        diffuseColor(2) = rgb(2) * localColor(2);
    } else {
        diffuseColor = {0.0,0.0,0.0};
    }
    return diffuseColor;
}

Vector3d App::MaterialGeneric::calculateReflectionColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                        const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                        const std::shared_ptr<ObjectGeneric> &currentObject,
                                                        const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                        const App::Ray &incidentRay) {
    Vector3d reflectionColor;
    Vector3d d = incidentRay.m_orientation;
    Vector3d reflection = d - (2 * d.dot(localNormal) * localNormal);

    Ray reflectionRay = Ray(intersectionPoint, intersectionPoint + reflection);
    std::shared_ptr<ObjectGeneric> closestObject;
    //Vector3d closestIntersectionPoint, closestLocalNormal, closestLocalColor;
    HitInformation closestHitInformation;
    bool isIntersectionFound = castRay(reflectionRay, objectList, currentObject, closestObject, closestHitInformation);

    Vector3d materialColor = {0.0,0.0,0.0};
    if (isIntersectionFound && (m_currentNumberOfReflections < m_maximumNumberOfReflections)){
        m_currentNumberOfReflections++;

        //if (closestHitInformation.hitObject -> m_hasMaterial){
            materialColor = closestHitInformation.hitObject -> m_material -> calculateColor(objectList, lightList, closestHitInformation.hitObject, closestHitInformation.pointOfIntersection, closestHitInformation.normal, closestHitInformation.localPointOfIntersection, closestHitInformation.uvCoordinates, reflectionRay); ///// TODO outros param
        //} else {
            //materialColor = calculateDiffuseColor(objectList, lightList, closestHitInformation.hitObject, closestHitInformation.pointOfIntersection, closestHitInformation.normal, closestObject->m_color);
        //}
    }
    else
    {
        ; // no change to color
    }

    reflectionColor = materialColor;
    return reflectionColor;
}

bool App::MaterialGeneric::castRay(const App::Ray &castRay, const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                              const std::shared_ptr<ObjectGeneric> &thisObject,
                              std::shared_ptr<ObjectGeneric> &closestObject, HitInformation &closestHitInformation) {
    //Vector3d intersectionPoint, localNormal, localColor; // hit data
    HitInformation hitInformation;
    double minDist = MINIMAL_DISTANCE;
    bool intersectionFound = false;
    for (std::shared_ptr<ObjectGeneric> currentObject : objectList){
        if (currentObject != thisObject){
            bool isValidIntersection = currentObject -> isIntersecting(castRay, hitInformation);
            if (isValidIntersection) {
                intersectionFound = true;
                double dist = (hitInformation.pointOfIntersection - castRay.m_point1).norm();

                if (dist < minDist){
                    minDist = dist;
                    closestObject = currentObject;
                    closestHitInformation = hitInformation;
                }
            }
        }
    }
    return intersectionFound;
}

void App::MaterialGeneric::setTexture(const std::shared_ptr<Textures::TextureGeneric> &newTexture) {
    if (m_texture == nullptr){
        m_texture = newTexture;
    }
}
