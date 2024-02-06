#include "LightPoint.hpp"

App::LightPoint::LightPoint() {
    m_color << 1.0,1.0,1.0;
    m_intensity = 1.0;
}

App::LightPoint::~LightPoint() {

}

bool App::LightPoint::CalculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                            const std::vector<std::shared_ptr<App::ObjectGeneric>> &objectsInScene,
                                            const std::shared_ptr<App::ObjectGeneric> &object, Vector3d &color,
                                            double &intensity) {

    // from intersection to source of light
    Vector3d lightDirection = (m_location - intersectionPoint).normalized();
    Vector3d sourceFromIntersection = intersectionPoint;
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
    return true;
}