#ifndef RAY_TRACER_APPLICATION_GEOMETRICALTRANSFORMATION_HPP
#define RAY_TRACER_APPLICATION_GEOMETRICALTRANSFORMATION_HPP

#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"
#include <string>

// transformation 3D -> 3D to be applied in objects for translation, rotation and scale

namespace App {
    // flags
    constexpr bool FORWARD_TRANSFORMATION = true;
    constexpr bool BACKWARD_TRANSFORMATION = false;

    class GeometricalTransformation {
    private:
        Matrix4d  m_forwardTransformation;
        Matrix4d  m_backwardTransformation;
        Matrix3d  m_linearTransformation;
        void extractLinearTransform();

    public:
        GeometricalTransformation();
        ~GeometricalTransformation();

        GeometricalTransformation(const Matrix4d &forward, const Matrix4d &backward);
        GeometricalTransformation(const Vector3d &translation, const Vector3d &rotation, const Vector3d &scale);

        void setTransformation(const Vector3d &translation, const Vector3d &rotationInPercentage, const Vector3d &scale);

        Matrix4d getForward() const;
        Matrix4d getBackward() const;

        Ray applyTransformation(const Ray &rayInput, bool isForwardTransformation);
        Vector3d applyTransformation(const Vector3d &vectorInput, bool isForwardTransformation);
        Vector3d applyNorm(const Vector3d &inputVector);

        // operator overriding
        friend GeometricalTransformation operator* (const GeometricalTransformation& leftSide, const GeometricalTransformation& rightSide);
        GeometricalTransformation operator= (const GeometricalTransformation &rightSide);
    };
}

#endif //RAY_TRACER_APPLICATION_GEOMETRICALTRANSFORMATION_HPP