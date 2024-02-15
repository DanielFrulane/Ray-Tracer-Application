#ifndef RAY_TRACER_APPLICATION_OBJECTCONE_HPP
#define RAY_TRACER_APPLICATION_OBJECTCONE_HPP

#include "ObjectGeneric.hpp"
#include "./GeometricalTransformation.hpp"

namespace App {
    class ObjectCone : public ObjectGeneric {
    public:
        ObjectCone();
        virtual ~ObjectCone() override;

        virtual bool isIntersecting(const Ray &castedRay, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) override;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTCONE_HPP
