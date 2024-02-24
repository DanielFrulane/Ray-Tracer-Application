#include "GeometricalTransformation.hpp"

using namespace Eigen;

App::GeometricalTransformation::GeometricalTransformation() {
    m_forwardTransformation.setIdentity();
    m_backwardTransformation.setIdentity();
    extractLinearTransform();
}

App::GeometricalTransformation::~GeometricalTransformation() = default;

App::GeometricalTransformation::GeometricalTransformation(const Vector3d &translation, const Vector3d &rotation,
                                                          const Vector3d &scale) {
    setTransformation(translation, rotation, scale);
    extractLinearTransform();
}

// general configuration needed
void App::GeometricalTransformation::extractLinearTransform() {
    m_linearTransformation = m_forwardTransformation.block(0,0,3,3);
    m_linearTransformation = m_linearTransformation.inverse().eval();
    m_linearTransformation = m_linearTransformation.transpose().eval();
}

App::GeometricalTransformation::GeometricalTransformation(const Matrix4d &forward, const Matrix4d &backward) {
    m_forwardTransformation = forward;
    m_backwardTransformation = backward;
}

void App::GeometricalTransformation::setTransformation(const Vector3d &translation, const Vector3d &rotationInPercentage, const Vector3d &scale) {
    Matrix4d translationFinal = Matrix4d::Identity();
    Matrix4d rotationXFinal = Matrix4d::Identity();
    Matrix4d rotationYFinal = Matrix4d::Identity();
    Matrix4d rotationZFinal = Matrix4d::Identity();
    Matrix4d scaleFinal = Matrix4d::Identity();

    // configures translation
    translationFinal(0,3) = translation(0);
    translationFinal(1,3) = translation(1);
    translationFinal(2,3) = translation(2);

    // configures rotation
    Vector3d rotation = rotationInPercentage * 2 * M_PI;

    rotationXFinal(1,1) = cos(rotation(0));
    rotationXFinal(1,2) = -sin(rotation(0));
    rotationXFinal(2,1) = sin(rotation(0));
    rotationXFinal(2,2) = cos(rotation(0));

    rotationYFinal(0,0) = cos(rotation(1));
    rotationYFinal(0,2) = -sin(rotation(1));
    rotationYFinal(2,0) = sin(rotation(1));
    rotationYFinal(2,2) = cos(rotation(1));

    rotationZFinal(0,0) = cos(rotation(2));
    rotationZFinal(0,1) = -sin(rotation(2));
    rotationZFinal(1,0) = sin(rotation(2));
    rotationZFinal(1,1) = cos(rotation(2));

    // configures scale
    scaleFinal(0,0) = scale(0);
    scaleFinal(1,1) = scale(1);
    scaleFinal(2,2) = scale(2);

    // configures final matrices
    m_forwardTransformation = translationFinal*rotationXFinal*rotationYFinal*rotationZFinal*scaleFinal;
    m_backwardTransformation = m_forwardTransformation.inverse();
}

Matrix4d App::GeometricalTransformation::getForward() const {
    return m_forwardTransformation;
}

Matrix4d App::GeometricalTransformation::getBackward() const {
    return m_backwardTransformation;
}

// applies the norm to a vector
Vector3d App::GeometricalTransformation::applyNorm(const Vector3d &inputVector) {
    return m_linearTransformation * inputVector;
}

// applies the transformation to a ray
App::Ray App::GeometricalTransformation::applyTransformation(const Ray &rayInput, bool isForwardTransformation) {
    App::Ray rayOutput;
    if (isForwardTransformation){
        rayOutput.m_point1 = this->applyTransformation(rayInput.m_point1, App::FORWARD_TRANSFORMATION);
        rayOutput.m_point2 = this->applyTransformation(rayInput.m_point2, App::FORWARD_TRANSFORMATION);
        rayOutput.m_orientation = rayOutput.m_point2 - rayOutput.m_point1;
    } else {
        rayOutput.m_point1 = this->applyTransformation(rayInput.m_point1, App::BACKWARD_TRANSFORMATION);
        rayOutput.m_point2 = this->applyTransformation(rayInput.m_point2, App::BACKWARD_TRANSFORMATION);
        rayOutput.m_orientation = rayOutput.m_point2 - rayOutput.m_point1;
    }
    return rayOutput;
}

// applies the transformation to a vector
Vector3d App::GeometricalTransformation::applyTransformation(const Vector3d &vectorInput, bool isForwardTransformation) {
    Vector4d temp(vectorInput(0), vectorInput(1), vectorInput(2), 1.0);
    Vector4d result;

    if (isForwardTransformation) {
        result = m_forwardTransformation * temp;
    } else {
        result = m_backwardTransformation * temp;
    }

    Vector3d output(result(0),result(1),result(2));

    return output;
}

// defines operator *
namespace App{
    App::GeometricalTransformation operator* (const GeometricalTransformation& leftSide, const GeometricalTransformation& rightSide){
        Matrix4d forward = leftSide.getForward() * rightSide.getForward();

        Matrix4d backward = forward.inverse();

        App::GeometricalTransformation finalResult (forward, backward);

        return finalResult;
    }
}

// defines operator =
App::GeometricalTransformation App::GeometricalTransformation::operator= (const App::GeometricalTransformation &rightSide){
    if (this != &rightSide){
        m_forwardTransformation = rightSide.m_forwardTransformation;
        m_backwardTransformation = rightSide.m_backwardTransformation;
    }
    return *this;
}