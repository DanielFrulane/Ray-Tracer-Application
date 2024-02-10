#include "Ray.hpp"

App::Ray::Ray() {
    m_point1 << 0.0,0.0,0.0;
    m_point2 << 0.0,0.0,1.0;
    m_orientation = m_point2 - m_point1;
}

App::Ray::Ray(const Vector3d &point1, const Vector3d &point2) {
    m_point1 = point1;
    m_point2 = point2;
    m_orientation = m_point2 - m_point1;
}

Vector3d App::Ray::getPoint1() const {
    return m_point1;
}

Vector3d App::Ray::getPoint2() const {
    return m_point2;
}

void App::Ray::updateOrientation() {
    m_orientation = m_point2 - m_point1;
}

App::Ray::Ray(const App::Ray &copyRay) {
    this->m_point1 = copyRay.m_point1;
    this->m_point2 = copyRay.m_point2;
    this->m_orientation = copyRay.m_orientation;
}
