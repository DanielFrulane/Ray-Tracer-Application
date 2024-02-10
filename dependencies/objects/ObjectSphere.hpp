#ifndef RAY_TRACER_APPLICATION_OBJECTSPHERE_HPP
#define RAY_TRACER_APPLICATION_OBJECTSPHERE_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectSphere : public App::ObjectGeneric{
    public:
        ObjectSphere();
        ~ObjectSphere() override;

        bool isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) override;
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTSPHERE_HPP
