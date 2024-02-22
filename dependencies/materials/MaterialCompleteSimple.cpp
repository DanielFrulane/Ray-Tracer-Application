#include "MaterialCompleteSimple.hpp"

App::MaterialCompleteSimple::MaterialCompleteSimple() {
    m_reflectivity = 0.0;
    m_shininess = 0.0;
    m_translucency = 0.0;
    m_indexOfRefraction = 0.0;
}

App::MaterialCompleteSimple::~MaterialCompleteSimple() = default;

// will call other color calculation functions
Vector3d App::MaterialCompleteSimple::calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                     const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                     const std::shared_ptr<ObjectGeneric> &currentObject,
                                                     const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                     const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                                     const App::Ray &cameraRay) {
    Vector3d materialColor = {0.0,0.0,0.0};
    Vector3d reflectiveColor = {0.0,0.0,0.0};
    Vector3d specularColor = {0.0,0.0,0.0};
    Vector3d translucentColor = {0.0,0.0,0.0};

    // considers diffuse color
    materialColor = calculateDiffuseColor(objectList, lightList, currentObject, intersectionPoint, localNormal, m_texture->getColor(uvCoordinates));

    // considers reflection color
    if (m_reflectivity > 0.0){
        reflectiveColor = calculateReflectionColor(objectList, lightList, currentObject, intersectionPoint, localNormal, cameraRay);
    }
    materialColor = (reflectiveColor * m_reflectivity) + (materialColor * (1.0 - m_reflectivity));

    // considers translucent color
    if (m_translucency > 0.0){
        translucentColor = calculateTranslucency(objectList, lightList, currentObject, intersectionPoint, localNormal, cameraRay);
    }
    materialColor = (translucentColor * m_translucency) + (materialColor * (1.0 - m_translucency));

    // considers specular color
    if (m_shininess > 0.0){
        specularColor = calculateSpecularColor(objectList, lightList, intersectionPoint, localNormal, cameraRay);
    }
    materialColor = materialColor + specularColor;

    return materialColor;
}

// calculates specular color
Vector3d App::MaterialCompleteSimple::calculateSpecularColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                             const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                             const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                             const App::Ray &cameraRay) const {
    Vector3d specularColor;
    Vector3d  rgb = {0.0,0.0,0.0};

    // for all lights in scene
    for (const std::shared_ptr<LightGeneric>& currentLight : lightList){
        Vector3d lightDirection = (currentLight->m_position - intersectionPoint).normalized();
        Vector3d startPoint = intersectionPoint + (lightDirection * 0.001);

        Ray lightRay (startPoint, startPoint + lightDirection);

        // checks for obstruction of light by objects
        HitInformation hitInformation;
        bool isLightObstructedByObject = false;
        for (const std::shared_ptr<ObjectGeneric>& anotherObject : objectList){ // for all object in scene
            isLightObstructedByObject = anotherObject -> isIntersecting(lightRay, hitInformation);
            if (isLightObstructedByObject){
                break;
            }
        }

        double intensity = 0.0;
        if (!isLightObstructedByObject){ // if no obstruction found
            // gets the reflection vector
            Vector3d direction = lightRay.m_orientation;
            Vector3d reflection = direction - (2 * direction.dot(localNormal) * localNormal);
            reflection.normalize();

            // gets the dot product
            Vector3d v = cameraRay.m_orientation;
            v.normalize();
            double dotProduct = reflection.dot(v);

            // only proceed if the dot product is positive
            if (dotProduct > 0.0){
                intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
            }
        }
        // else: intensity of light is 0

        rgb += currentLight->m_color * intensity;
    }

    specularColor = rgb;
    return specularColor;
}

// calculates translucent color
Vector3d App::MaterialCompleteSimple::calculateTranslucency(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                            const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                            const std::shared_ptr<ObjectGeneric> &currentObject,
                                                            const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                            const App::Ray &cameraRay) const {
    Vector3d translucentColor;

    // refracts ray given the index of refraction of the object
    Vector3d cameraDirection = cameraRay.m_orientation;
    cameraDirection.normalize();
    Vector3d temporaryNormal = localNormal;
    double ratio = 1.0 / m_indexOfRefraction;
    double dotProduct = -temporaryNormal.dot(cameraDirection);
    if (dotProduct < 0.0){
        // inverts values
        temporaryNormal = -1.0*temporaryNormal;
        dotProduct = -temporaryNormal.dot(cameraDirection);
    }

    // gets the refracted ray
    Vector3d refractedVector = ratio * cameraDirection + (ratio * dotProduct - sqrt(1.0 - pow(ratio, 2.0) * (1.0 - pow(dotProduct, 2.0)))) * temporaryNormal;
    Ray refractedRay (intersectionPoint + (refractedVector * 0.01), intersectionPoint + refractedVector);

    // test for secondary intersection with the same object (second refraction)
    std::shared_ptr<ObjectGeneric> closestObject;
    HitInformation closestHitInformation;
    HitInformation hitInformation;
    bool hasSecondIntersection = currentObject -> isIntersecting(refractedRay, hitInformation);
    bool intersectionFound;
    Ray finalRay;
    if (hasSecondIntersection){
        // gets the second refracted vector
        Vector3d direction2 = refractedRay.m_orientation;
        direction2.normalize();
        Vector3d temporaryNormal2 = hitInformation.normal;
        double ratio2 = m_indexOfRefraction;
        double dotProduct2 = -temporaryNormal2.dot(direction2);
        if (dotProduct2 < 0.0){
            temporaryNormal2 = temporaryNormal2 * -1.0;
            dotProduct2 = -temporaryNormal2.dot(direction2);
        }

        // gets the refracted ray
        Vector3d refractedVector2 = ratio2 * direction2 + (ratio2 * dotProduct2 - sqrt(1.0 - pow(ratio2, 2.0) * (1.0 - pow(dotProduct2, 2.0)))) * temporaryNormal2;
        Ray refractedRay2 (hitInformation.pointOfIntersection + (refractedVector2 * 0.01), hitInformation.pointOfIntersection + refractedVector2);

        // cast this ray into the scene
        intersectionFound = castRay(refractedRay2, objectList, currentObject, closestObject, closestHitInformation);
        finalRay = refractedRay2;
    }else{
        // no intersection found, calculate refracted ray
        intersectionFound = castRay(refractedRay, objectList, currentObject, closestObject, closestHitInformation);
        finalRay = refractedRay;
    }

    // gets the color for closest object
    Vector3d matColor = {0.0,0.0,0.0};
    if (intersectionFound){
        // gets color from material
        matColor = closestHitInformation.hitObject -> m_material -> calculateColor(objectList, lightList, closestHitInformation.hitObject, closestHitInformation.pointOfIntersection, closestHitInformation.normal, closestHitInformation.localPointOfIntersection, closestHitInformation.uvCoordinates, finalRay);
    }
    // else: no change to color

    translucentColor = matColor;
    return translucentColor;
}
