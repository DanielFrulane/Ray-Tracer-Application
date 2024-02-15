#ifndef RAY_TRACER_APPLICATION_OBJECTCUBOID_HPP
#define RAY_TRACER_APPLICATION_OBJECTCUBOID_HPP

#include "ObjectGeneric.hpp"

namespace App {
    class ObjectCuboid : public ObjectGeneric {
    public:
        /* The default constructor.
            Note that this will define a unit sphere at the origin. */
        ObjectCuboid();

        // Override the destructor.
        virtual ~ObjectCuboid() override;

        // Override the function to test for intersections.
        virtual bool isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) override;
        bool isIntersecting(const Ray &castRay);
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTCUBOID_HPP
