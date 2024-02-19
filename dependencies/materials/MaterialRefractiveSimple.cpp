#include "MaterialRefractiveSimple.hpp"

App::MaterialRefractiveSimple::MaterialRefractiveSimple() {

}

App::MaterialRefractiveSimple::~MaterialRefractiveSimple() {

}

Vector3d App::MaterialRefractiveSimple::calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                       const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                       const std::shared_ptr<ObjectGeneric> &currentObject,
                                                       const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                       const Vector3d &localPointOfIntersection, const Vector2d &uvCoordinates,
                                                       const App::Ray &cameraRay) {
    Vector3d materialColor = {0.0,0.0,0.0};
    Vector3d reflectiveColor = {0.0,0.0,0.0};
    Vector3d diffuseColor = {0.0,0.0,0.0};
    Vector3d specularColor = {0.0,0.0,0.0};
    Vector3d translucentColor = {0.0,0.0,0.0};

    if (m_hasTexture){
        diffuseColor = calculateDiffuseColor(objectList, lightList, currentObject, intersectionPoint, localNormal, m_textures.at(0)->getColor(uvCoordinates)); ////////// TODO UV COORDS
    } else {
        diffuseColor = calculateDiffuseColor(objectList, lightList, currentObject, intersectionPoint, localNormal, m_color);
    }

    if (m_reflectivity > 0.0){
        reflectiveColor = calculateReflectionColor(objectList, lightList, currentObject, intersectionPoint, localNormal, cameraRay);
    }

    materialColor = (reflectiveColor * m_reflectivity) + (diffuseColor * (1.0 - m_reflectivity));

    if (m_translucency > 0.0){
        translucentColor = calculateTranslucency(objectList, lightList, currentObject, intersectionPoint, localNormal, cameraRay);
    }

    // And combine with the current color.
    materialColor = (translucentColor * m_translucency) + (materialColor * (1.0 - m_translucency));

    // And compute the specular component.
    if (m_shininess > 0.0){
        specularColor = calculateSpecularColor(objectList, lightList, intersectionPoint, localNormal, cameraRay);
    }

    // Finally, add the specular component.
    materialColor = materialColor + specularColor;

    return materialColor;
}

Vector3d App::MaterialRefractiveSimple::calculateSpecularColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                              const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                              const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                              const App::Ray &cameraRay) {
    Vector3d specularColor;
    Vector3d  rgb = {0.0,0.0,0.0};

    // Loop through all of the lights in the scene.
    for (std::shared_ptr<LightGeneric> currentLight : lightList){
        /* Check for intersections with all objects in the scene. */
        double intensity = 0.0;

        // Construct a vector pointing from the intersection point to the light.
        Vector3d lightDir = (currentLight->m_location - intersectionPoint).normalized();

        // Compute a start point.
        Vector3d startPoint = intersectionPoint + (lightDir * 0.001);

        // Construct a ray from the point of intersection to the light.
        Ray lightRay (startPoint, startPoint + lightDir);

        /* Loop through all objects in the scene to check if any
            obstruct light from this source. */
        //Vector3d pointOfIntersection;
        //Vector3d pointOfIntersectionNormal;
        //Vector3d pointOfIntersectionColor;
        HitInformation hitInformation;
        bool validInt = false;
        for (std::shared_ptr<ObjectGeneric> anotherObject : objectList){
            validInt = anotherObject -> isIntersecting(lightRay, hitInformation);
            if (validInt){
                break;
            }
        }

        /* If no intersections were found, then proceed with
            computing the specular component. */
        if (!validInt){
            // Compute the reflection vector.
            Vector3d d = lightRay.m_orientation;
            Vector3d r = d - (2 * d.dot(localNormal) * localNormal);
            r.normalize();

            // Compute the dot product.
            Vector3d v = cameraRay.m_orientation;
            v.normalize();
            double dotProduct = r.dot(v);

            // Only proceed if the dot product is positive.
            if (dotProduct > 0.0){
                intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
            }
        }

        rgb += currentLight->m_color * intensity;
    }

    specularColor = rgb;
    return specularColor;
}

Vector3d App::MaterialRefractiveSimple::calculateTranslucency(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                             const std::vector<std::shared_ptr<LightGeneric>> &lightList,
                                                             const std::shared_ptr<ObjectGeneric> &currentObject,
                                                             const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                             const App::Ray &cameraRay) {
    Vector3d translucentColor;

    // Compute the refracted vector.
    Vector3d p = cameraRay.m_orientation;
    p.normalize();
    Vector3d tempNormal = localNormal;
    double r = 1.0 / m_indexOfRefraction;
    double c = -tempNormal.dot(p);
    if (c < 0.0){
        tempNormal = tempNormal * -1.0;
        c = -tempNormal.dot(p);
    }

    Vector3d refractedVector = r*p + (r*c - sqrt(1.0-pow(r,2.0) * (1.0-pow(c,2.0)))) * tempNormal;

    // Construct the refracted ray.
    Ray refractedRay (intersectionPoint + (refractedVector * 0.01), intersectionPoint + refractedVector);

    // Test for secondary intersection with this object.
    std::shared_ptr<ObjectGeneric> closestObject;
    //Vector3d closestIntPoint;
    //Vector3d closestLocalNormal;
    //Vector3d closestLocalColor;
    //Vector3d newIntPoint;
    //Vector3d newLocalNormal;
    //Vector3d newLocalColor;
    HitInformation closestHitInformation;
    HitInformation hitInformation;
    bool test = currentObject -> isIntersecting(refractedRay, hitInformation);
    bool intersectionFound = false;
    Ray finalRay;
    if (test){
        // Compute the refracted vector.
        Vector3d p2 = refractedRay.m_orientation;
        p2.normalize();
        Vector3d tempNormal2 = hitInformation.normal;
        double r2 = m_indexOfRefraction;
        double c2 = -tempNormal2.dot(p2);
        if (c2 < 0.0){
            tempNormal2 = tempNormal2 * -1.0;
            c2 = -tempNormal2.dot(p2);
        }
        Vector3d refractedVector2 = r2*p2 + (r2*c2 - sqrt(1.0-pow(r2,2.0) * (1.0-pow(c2,2.0)))) * tempNormal2;

        // Compute the refracted ray.
        Ray refractedRay2 (hitInformation.pointOfIntersection + (refractedVector2 * 0.01), hitInformation.pointOfIntersection + refractedVector2);

        // Cast this ray into the scene.
        intersectionFound = castRay(refractedRay2, objectList, currentObject, closestObject, closestHitInformation);
        finalRay = refractedRay2;
    }else{
        /* No secondary intersections were found, so continue the original refracted ray. */
        intersectionFound = castRay(refractedRay, objectList, currentObject, closestObject, closestHitInformation);
        finalRay = refractedRay;
    }

    // Compute the color for closest object.
    Vector3d matColor = {0.0,0.0,0.0};
    if (intersectionFound){
        // Check if a material has been assigned.
        if (closestObject -> m_hasMaterial){
            matColor = closestHitInformation.hitObject -> m_material -> calculateColor(objectList, lightList, closestHitInformation.hitObject, closestHitInformation.pointOfIntersection, closestHitInformation.normal, closestHitInformation.localPointOfIntersection, closestHitInformation.uvCoordinates, finalRay);
        }else{
            matColor = MaterialGeneric::calculateDiffuseColor(objectList, lightList, closestHitInformation.hitObject, closestHitInformation.pointOfIntersection, closestHitInformation.normal, closestObject->m_color);
        }
    }else{
        // no change to color
    }

    translucentColor = matColor;
    return translucentColor;
}
