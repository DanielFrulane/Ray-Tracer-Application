#include "GeometricalTransform.hpp"
#include <iostream>

void App::GeometricalTransform::printMatrix(const Matrix4d &matrix) {
    std::cout<<"printing matrix: "<<std::endl<<matrix<<std::endl;
}

void App::GeometricalTransform::printMatrix(bool isForwardTransformation) {
    if (isForwardTransformation){
        printMatrix(m_forwardTransformation);
    } else {
        printMatrix(m_backwardTransformation);
    }
}

App::GeometricalTransform::GeometricalTransform() {
    m_forwardTransformation.setIdentity();
    m_backwardTransformation.setIdentity();
}

App::GeometricalTransform::~GeometricalTransform() {

}

App::GeometricalTransform::GeometricalTransform(const Matrix4d &forward, const Matrix4d &backward) {
    // correct dimensions check
    if(forward.rows() != 4 || forward.cols() != 4 || backward.rows() != 4 || backward.cols() != 4){
        std::string message = "Transform dimensions must be 4x4, check:" +
                              std::string("\nForward Transformation) Rows: ") + std::to_string(forward.rows()) +
                              std::string(", Columns: ") + std::to_string(forward.cols()) +
                              std::string("\nBackward Transformation) Rows: ") + std::to_string( backward.rows()) +
                              std::string(", Columns: ") + std::to_string(backward.cols());
        throw std::invalid_argument(message);
    }
    m_forwardTransformation = forward;
    m_backwardTransformation = backward;
}

void App::GeometricalTransform::setTransformation(const Vector3d &translation, const Vector3d &rotation, const Vector3d &scale) {
    /*// correct dimensions check
    if(translation.rows() != 3 || translation.cols() != 1 || rotation.rows() != 3 || rotation.cols() != 1 || scale.rows() != 3 || scale.cols() != 1){
        std::string message = "Transform dimensions must be 3x1, check:" +
                              std::string("\nTranslation Transformation) Rows: ") + std::to_string(translation.rows()) +
                              std::string(", Columns: ") + std::to_string(translation.cols()) +
                              std::string("\nRotation Transformation) Rows: ") + std::to_string(rotation.rows()) +
                              std::string(", Columns: ") + std::to_string(rotation.cols()) +
                              std::string("\nScale Transformation) Rows: ") + std::to_string( scale.rows()) +
                              std::string(", Columns: ") + std::to_string(scale.cols());
        throw std::invalid_argument(message);
    }*/
    Matrix4d translationFinal = Matrix4d::Identity();
    Matrix4d rotationXFinal = Matrix4d::Identity();
    Matrix4d rotationYFinal = Matrix4d::Identity();
    Matrix4d rotationZFinal = Matrix4d::Identity();
    Matrix4d scaleFinal = Matrix4d::Identity();

    translationFinal(0,3) = translation(0);
    translationFinal(1,3) = translation(1);
    translationFinal(2,3) = translation(2);

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

    scaleFinal(0,0) = rotation(0);
    scaleFinal(1,1) = rotation(1);
    scaleFinal(2,2) = rotation(2);

    m_forwardTransformation << translationFinal * scaleFinal * rotationXFinal * rotationYFinal * rotationZFinal;
    m_backwardTransformation = m_forwardTransformation;
    m_backwardTransformation.inverse();
}

Matrix4d App::GeometricalTransform::getForward() {
    return m_forwardTransformation;
}

Matrix4d App::GeometricalTransform::getBackward() {
    return m_backwardTransformation;
}

App::Ray App::GeometricalTransform::applyTransformation(const Ray &rayInput, bool isForwardTransformation) {
    App::Ray rayOutput;
    if (isForwardTransformation){
        rayOutput.m_point1 = this->applyTransformation(rayInput.m_point1, App::FORWARD_TRANSFORMATION);
        rayOutput.m_point2 = this->applyTransformation(rayInput.m_point2, App::FORWARD_TRANSFORMATION);
        rayOutput.updateOrientation();
    } else {
        rayOutput.m_point1 = this->applyTransformation(rayInput.m_point1, App::BACKWARD_TRANSFORMATION);
        rayOutput.m_point2 = this->applyTransformation(rayInput.m_point2, App::BACKWARD_TRANSFORMATION);
        rayOutput.updateOrientation();
    }
    return rayOutput;
}

Vector3d App::GeometricalTransform::applyTransformation(const Vector3d &vectorInput, bool isForwardTransformation) {
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

namespace App{
    App::GeometricalTransform operator* (const GeometricalTransform& leftHandSide, const GeometricalTransform& rightHandSide){
        Matrix4d forward = leftHandSide.m_forwardTransformation * rightHandSide.m_forwardTransformation;

        Matrix4d& backward = forward;
        backward.inverse();

        App::GeometricalTransform finalResult (forward, backward);

        return finalResult;
    }
}

App::GeometricalTransform App::GeometricalTransform::operator= (const App::GeometricalTransform &rightHandSide){
    if (this != &rightHandSide){
        m_forwardTransformation = rightHandSide.m_forwardTransformation;
        m_backwardTransformation = rightHandSide.m_backwardTransformation;
    }
    return *this;
}