#include "ObjectGeneric.hpp"
#include <math.h>

#define PRECISION_CONSTANT 1e-21f;

App::ObjectGeneric::ObjectGeneric() {
    ;
}

App::ObjectGeneric::~ObjectGeneric(){
    ;
}

bool App::ObjectGeneric::isIntersecting(const App::Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) {
    return false;
}

bool App::ObjectGeneric::proximityPrecision(const double float1, const double float2) {
    return false;
}

const Vector3d &App::ObjectGeneric::getColor() const {
    return m_color;
}

void App::ObjectGeneric::setColor(const Vector3d &color) {
    m_color = color;
}

void App::ObjectGeneric::setTransformation(const App::GeometricalTransform &transformation) {
    m_transformation = transformation;
}
