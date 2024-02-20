#include "LightPoint.hpp"

App::LightPoint::LightPoint() {
    m_color << 1.0,1.0,1.0;
    m_intensity = 1.0;
}

App::LightPoint::~LightPoint() {

}

bool App::LightPoint::calculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                            const std::vector<std::shared_ptr<App::ObjectGeneric>> &objectsInScene,
                                            const std::shared_ptr<App::ObjectGeneric> &object, Vector3d &color,
                                            double &intensity) {
    // from intersection to source of light
    Vector3d lightDirection = (m_position - intersectionPoint).normalized();
    double lightDistance = (m_position - intersectionPoint).norm();
    Vector3d sourceFromIntersection = intersectionPoint;
    Ray lightRay (sourceFromIntersection, sourceFromIntersection + lightDirection);

    //Vector3d pointOfIntersection;
    //Vector3d pointOfIntersectionNormal;
    //Vector3d pointOfIntersectionColor;
    HitInformation hitInformation;
    bool isValidIntersection = false;
    for(const std::shared_ptr<ObjectGeneric>& objectOther: objectsInScene){
        if (object != objectOther){
            isValidIntersection = objectOther ->isIntersecting(lightRay, hitInformation);
            if(isValidIntersection){
                // checks if the light is between the two objects (then, no shadow needed)
                double distance = (hitInformation.pointOfIntersection - sourceFromIntersection).norm();
                if (distance > lightDistance){
                    isValidIntersection = false;
                }
            }
        }
        if(isValidIntersection){
            // already found what we were looking for
            break;
        }
    }

    if (!isValidIntersection){
        // no shadow (no intersection with light ray)
        double angleToLight = acos(localNormal.dot(lightDirection));
        if (angleToLight > 1.5708){  // pi/2
            color = m_color; // dark
            intensity = 0.0;
            return false;
        } else {
            color = m_color; // colored
            intensity = m_intensity * (1.0 - (angleToLight / 1.5708));
            return true;
        }
    }
    else {
        color = m_color;
        intensity = 0.0;
        return false;
    }
    return true;
}
