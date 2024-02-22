#include "LightPoint.hpp"

App::LightPoint::LightPoint() {
    m_color << 1.0,1.0,1.0;
    m_intensity = 1.0;
}

App::LightPoint::~LightPoint() = default;

// calculates how the light impacts the color of a certain object
bool App::LightPoint::calculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                            const std::vector<std::shared_ptr<App::ObjectGeneric>> &objectsInScene,
                                            const std::shared_ptr<App::ObjectGeneric> &object, Vector3d &color,
                                            double &intensity) {
    // from intersection to source of light
    Vector3d lightDirection = (m_position - intersectionPoint).normalized();
    double lightDistance = (m_position - intersectionPoint).norm();
    Ray lightRay (intersectionPoint, intersectionPoint + lightDirection);

    HitInformation hitInformation;
    bool isValidIntersection = false;
    // testing objects in scene until finding an intersection
    for(const std::shared_ptr<ObjectGeneric>& objectOther: objectsInScene){
        if (object != objectOther){
            isValidIntersection = objectOther ->isIntersecting(lightRay, hitInformation);
            if(isValidIntersection){
                // checks if the light is between the two objects (then, no shadow needed)
                double distance = (hitInformation.pointOfIntersection - intersectionPoint).norm();
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
        if (angleToLight > (M_PI/2)){
            color = m_color; // dark
            intensity = 0.0;
            return false;
        } else {
            color = m_color; // colored
            intensity = m_intensity * (1.0 - (angleToLight / (M_PI/2)));
            return true;
        }
    }
    else {
        color = m_color;
        intensity = 0.0; // dark
        return false;
    }
}
