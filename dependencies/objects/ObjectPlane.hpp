#ifndef RAY_TRACER_APPLICATION_OBJECTPLANE_HPP
#define RAY_TRACER_APPLICATION_OBJECTPLANE_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectPlane : public ObjectGeneric {
    public:
        ObjectPlane();
        virtual ~ObjectPlane() override;
        virtual bool isIntersecting(const App::Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) override;

    };
}


#endif //RAY_TRACER_APPLICATION_OBJECTPLANE_HPP
