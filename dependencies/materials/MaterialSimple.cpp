#include "MaterialSimple.hpp"

App::MaterialSimple::MaterialSimple() { // TODO MERGE 2 MATERIALS
    m_maximumNumberOfReflections = 3;
    m_currentNumberOfReflections = 0;
}

App::MaterialSimple::~MaterialSimple() {

}

Vector3d App::MaterialSimple::calculateColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                             const std::vector<std::shared_ptr<LightSource>> &lightList,
                                             const std::shared_ptr<ObjectGeneric> &currentObject,
                                             const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                             const App::Ray &cameraRay) {
    Vector3d materialColor={0.0,0.0,0.0};
    Vector3d reflectionColor={0.0,0.0,0.0};;
    Vector3d diffuseColor={0.0,0.0,0.0};;
    Vector3d specularColor={0.0,0.0,0.0};;

    if(m_hasTexture){
        diffuseColor = calculateDiffuseColor(objectList, lightList, currentObject, intersectionPoint, localNormal,
                                             m_textures.at(0)->getColor(currentObject->m_uvCoordinates));
    } else {
        diffuseColor = calculateDiffuseColor(objectList, lightList, currentObject, intersectionPoint, localNormal, m_color);
    }

    if (m_reflectivity > 0.0){
        reflectionColor = calculateReflectionColor(objectList, lightList, currentObject, intersectionPoint, localNormal, cameraRay);
    }

    materialColor = (reflectionColor * m_reflectivity) + (diffuseColor * (1 - m_reflectivity));

    if (m_shininess> 0.0){
        specularColor = calculateSpecularColor(objectList, lightList, intersectionPoint, localNormal, cameraRay);
    }
    materialColor += specularColor;
    return materialColor;
}

Vector3d App::MaterialSimple::calculateSpecularColor(const std::vector<std::shared_ptr<ObjectGeneric>> &objectList,
                                                     const std::vector<std::shared_ptr<LightSource>> &lightList,
                                                     const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                                     const App::Ray &cameraRay) {
    Vector3d rgb = {0.0,0.0,0.0};

    for(std::shared_ptr<LightSource> currentLight : lightList){
        double intensity = 0.0;
        Vector3d lightDirection = (currentLight->m_location - intersectionPoint).normalized();
        Vector3d startPoint = intersectionPoint + (lightDirection * 0.001);
        Ray ray = Ray(startPoint, startPoint + lightDirection);

        Vector3d pointOfIntersection = {0.0,0.0,0.0};
        Vector3d pointOfIntersectionNormal = {0.0,0.0,0.0};
        Vector3d pointOfIntersectionColor = {0.0,0.0,0.0};
        bool isValidIntersection = false;
        for(std::shared_ptr<ObjectGeneric> anotherObject : objectList){
            isValidIntersection = anotherObject->isIntersecting(ray, pointOfIntersection, pointOfIntersectionNormal, pointOfIntersectionColor);
            if (isValidIntersection){
                break;
            }
        }
        // if no obstruction
        if (!isValidIntersection){
            Vector3d d = ray.m_orientation;
            Vector3d r = d - (2 * d.dot(localNormal) * localNormal);
            r.normalize();

            Vector3d v = cameraRay.m_orientation;
            v.normalize();
            double dp = r.dot(v);

            if (dp > 0.0){
                intensity = m_reflectivity * pow(dp, m_shininess);
            }
        }

        rgb += (currentLight->m_color)*intensity;
    }
    Vector3d specularColor = rgb;
    return specularColor;
}
