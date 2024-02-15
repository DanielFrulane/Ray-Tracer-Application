#ifndef RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
#define RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP

#include "GeometricalTransformation.hpp"
#include <memory>

namespace App {
    // forward-declaring a class to be overwritten in order to avoid redundant import errors
    class MaterialGeneric;

    class ObjectGeneric {
    public:
        GeometricalTransformation m_boundingBoxTransformation;
        double m_boundingBoxPadding = 0.0;

        GeometricalTransformation m_transformation;

        ObjectGeneric();

        explicit ObjectGeneric(const Vector3d &mColor);

        virtual ~ObjectGeneric();

        void setTransformation(const GeometricalTransformation &transformation);

        virtual bool isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor);
        bool isWithinProximityPrecision(const double float1, const double float2);

        virtual void getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim);
        virtual void getExtents(const GeometricalTransformation &parentTransformationMatrix, Vector3d &xLim, Vector3d &yLim, Vector3d &zLim);
        std::vector<Vector3d> getCuboid(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);

        void setColor(const Vector3d &color);
        const Vector3d &getColor() const;

        bool setMaterial(const std::shared_ptr<MaterialGeneric> &material);

        Vector3d m_color;
        std::shared_ptr<MaterialGeneric> m_material;
        bool m_hasMaterial = false;
        Vector2d m_uvCoordinates;
        bool m_isVisible = true;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
