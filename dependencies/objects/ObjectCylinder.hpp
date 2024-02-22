#ifndef RAY_TRACER_APPLICATION_OBJECTCYLINDER_HPP
#define RAY_TRACER_APPLICATION_OBJECTCYLINDER_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectCylinder : public ObjectGeneric, public std::enable_shared_from_this<ObjectCylinder> {
    public:
        ObjectCylinder();
        ~ObjectCylinder() override;

        bool isIntersecting(const Ray &rayCasted, HitInformation &hitInformation) override;
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTCYLINDER_HPP