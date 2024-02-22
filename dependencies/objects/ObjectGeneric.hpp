#ifndef RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
#define RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP

#include "GeometricalTransformation.hpp"
#include "../HitInformation.hpp"
#include <iostream>
#include <memory>

namespace App {
    // forward-declaring a class to be overwritten in order to avoid redundant import errors
    class MaterialGeneric;
    
    // constants for UV mapping
    constexpr int uvSPHERE = 0;
    constexpr int uvPLANE = 1;
    constexpr int uvCYLINDER = 2;
    constexpr int uvCUBOID = 3;

    class ObjectGeneric {
    public:
        std::shared_ptr<MaterialGeneric> m_material = nullptr;
        Vector2d m_uvCoordinates;
        int m_uvMapType = uvSPHERE; // default

        GeometricalTransformation m_boundingBoxTransformation;
        GeometricalTransformation m_transformation;

        ObjectGeneric();
        virtual ~ObjectGeneric();

        void setTransformation(const GeometricalTransformation &transformation);
        void setMaterial(const std::shared_ptr<MaterialGeneric> &material);

        virtual bool isIntersecting(const Ray &rayCasted, HitInformation &hitInformation);
        static bool isWithinProximityPrecision(double float1, double float2);
        void calculateUVSpace(const Vector3d &localPointOfIntersection, Vector2d &uvCoordinates) const;

        virtual void getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim);
        static std::vector<Vector3d> getCuboid(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) ;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTGENERIC_HPP
