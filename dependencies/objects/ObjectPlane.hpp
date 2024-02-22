#ifndef RAY_TRACER_APPLICATION_OBJECTPLANE_HPP
#define RAY_TRACER_APPLICATION_OBJECTPLANE_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectPlane : public ObjectGeneric, public std::enable_shared_from_this<ObjectPlane> {
    public:
        ObjectPlane();
        ~ObjectPlane() override;
        bool isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) override;
    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTPLANE_HPP
