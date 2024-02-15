#ifndef RAY_TRACER_APPLICATION_OBJECTCYLINDER_HPP
#define RAY_TRACER_APPLICATION_OBJECTCYLINDER_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectCylinder : public ObjectGeneric {
    public:
        ObjectCylinder();
        virtual ~ObjectCylinder();

        virtual bool isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) override;
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTCYLINDER_HPP