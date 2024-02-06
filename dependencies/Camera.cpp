#include "Camera.hpp"
#include "Ray.hpp"
#include <math.h>

using namespace Eigen;

App::Camera::Camera() {
    m_position << 0.0,-10.0,0.0;
    m_lookAt << 0.0,0.0,0.0;
    m_up << 0.0,0.0,1.0;
    m_length = 1.0;
    m_horizontalSize = 1.0;
    m_aspectRatio = 1.0;
}

void App::Camera::setPosition(const Vector3d &newPosition){
    m_position = newPosition;
}
void App::Camera::setLookAt(const Vector3d &newLookAt){
    m_lookAt = newLookAt;
}
void App::Camera::setUp(const Vector3d &newUp){
    m_up = newUp;
}
void App::Camera::setLength(double newLength){
    m_length = newLength;
}
void App::Camera::setHorizontalSize(double newHorizontalSize){
    m_horizontalSize = newHorizontalSize;
}
void App::Camera::setAspectRatio(double newAspectRatio){
    m_aspectRatio = newAspectRatio;
}
Vector3d App::Camera::getPosition() {
    return m_position;
}
Vector3d App::Camera::getLookAt(){
    return m_lookAt;
}
Vector3d App::Camera::getUp(){
    return m_up;
}
Vector3d App::Camera::getU(){
    return m_u;
}
Vector3d App::Camera::getV(){
    return m_v;
}
Vector3d App::Camera::getScreenCenter(){
    return m_screenCenter;
}
double App::Camera::getLength(){
    return m_length;
}
double App::Camera::getHorizontalSize(){
    return m_horizontalSize;
}
double App::Camera::getAspectRatio(){
    return m_aspectRatio;
}

void App::Camera::updateStatus() {
    // see the documentation for the visual explanation of each vector
    m_alignmentVector = m_lookAt - m_position;
    m_alignmentVector.normalize();

    m_u = m_alignmentVector.cross(m_up);
    m_u.normalize();
    m_v = m_u.cross(m_alignmentVector);
    m_v.normalize();

    m_screenCenter = m_position + (m_length * m_alignmentVector);
    m_u = m_u * m_horizontalSize; // proportional to aspect ratio
    m_v = m_v * (m_horizontalSize / m_aspectRatio);
}

bool App::Camera::generateRay(double projectionScreenX, double projectionScreenY, App::Ray &parameterRay) {
    Vector3d screenWorldUCoordinate = m_screenCenter + (m_u * projectionScreenX);
    Vector3d screenWorldCompleteCoordinate = screenWorldUCoordinate + (m_v * projectionScreenY);

    parameterRay.m_point1 = m_position;
    parameterRay.m_point2 = screenWorldCompleteCoordinate;
    parameterRay.m_orientation = screenWorldCompleteCoordinate - m_position;

    return true;
}