#ifndef RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
#define RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP

#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"
#include "GeometricalTransform.hpp"

using namespace Eigen;

namespace App {
    class ObjectGeneric {
    public:
        GeometricalTransform m_transformation;

        ObjectGeneric();

        explicit ObjectGeneric(const Vector3d &mColor);

        virtual ~ObjectGeneric();

        void setTransformation(const GeometricalTransform &transformation);

        virtual bool isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor);
        bool proximityPrecision(const double float1, const double float2);

        void setColor(const Vector3d &color);
        const Vector3d &getColor() const;

        Vector3d m_color;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
