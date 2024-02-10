#ifndef RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
#define RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP

#include "GeometricalTransform.hpp"
#include <memory>

namespace App {
    // forward-declaring a class to be overwritten in order to avoid redundant import errors
    class MaterialGeneric;

    class ObjectGeneric {
    public:
        GeometricalTransform m_transformation;

        ObjectGeneric();

        explicit ObjectGeneric(const Vector3d &mColor);

        virtual ~ObjectGeneric();

        void setTransformation(const GeometricalTransform &transformation);

        virtual bool isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor);
        bool isWithinProximityPrecision(const double float1, const double float2);

        void setColor(const Vector3d &color);
        const Vector3d &getColor() const;

        bool setMaterial(const std::shared_ptr<MaterialGeneric> &material);

        Vector3d m_color;
        std::shared_ptr<MaterialGeneric> m_material;
        bool m_hasMaterial = false;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
