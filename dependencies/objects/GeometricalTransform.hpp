#ifndef RAY_TRACER_APPLICATION_GEOMETRICALTRANSFORM_HPP
#define RAY_TRACER_APPLICATION_GEOMETRICALTRANSFORM_HPP

#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"
#include <string>

namespace App {
    // flags
    constexpr bool FORWARD_TRANSFORMATION = true;
    constexpr bool BACKWARD_TRANSFORMATION = false;

    class GeometricalTransform {
    private:
        Matrix4d  m_forwardTransformation;
        Matrix4d  m_backwardTransformation;

    public:
        GeometricalTransform();
        ~GeometricalTransform();

        GeometricalTransform(const Matrix4d &forward, const Matrix4d &backward);

        void setTransformation(const Vector3d &translation, const Vector3d &rotation, const Vector3d &scale);

        Matrix4d getForward() const;
        Matrix4d getBackward() const;

        Ray applyTransformation(const Ray &rayInput, bool isForwardTransformation);
        Vector3d applyTransformation(const Vector3d &vectorInput, bool isForwardTransformation);

        friend GeometricalTransform operator* (const GeometricalTransform& leftHandSide, const GeometricalTransform& rightHandSide);

        GeometricalTransform operator= (const GeometricalTransform &rightHandSide);

        void printMatrix(bool isForwardTransformation);
    };
}

#endif //RAY_TRACER_APPLICATION_GEOMETRICALTRANSFORM_HPP