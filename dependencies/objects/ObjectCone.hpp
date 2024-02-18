#ifndef RAY_TRACER_APPLICATION_OBJECTCONE_HPP
#define RAY_TRACER_APPLICATION_OBJECTCONE_HPP

#include "ObjectGeneric.hpp"
#include "./GeometricalTransformation.hpp"

namespace App {
    class ObjectCone : public ObjectGeneric, public std::enable_shared_from_this<ObjectCone> {
    public:
        ObjectCone();
        virtual ~ObjectCone() override;

        virtual bool isIntersecting(const Ray &castedRay, HitInformation &hitInformation) override;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTCONE_HPP
