#ifndef RAY_TRACER_APPLICATION_OBJECTSPHERE_HPP
#define RAY_TRACER_APPLICATION_OBJECTSPHERE_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectSphere : public App::ObjectGeneric, public std::enable_shared_from_this<ObjectSphere>{
    public:
        ObjectSphere();
        ~ObjectSphere() override;

        bool isIntersecting(const Ray &rayCasted, HitInformation &hitInformation) override;
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTSPHERE_HPP
