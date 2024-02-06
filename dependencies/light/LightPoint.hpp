#ifndef RAY_TRACER_APPLICATION_LIGHTPOINT_HPP
#define RAY_TRACER_APPLICATION_LIGHTPOINT_HPP

#include "LightSource.hpp"
#include "../objects/ObjectGeneric.hpp"

namespace App {
    class LightPoint : public App::LightSource {
    public:
        LightPoint();
        virtual ~LightPoint() override;

        virtual bool CalculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                           const std::vector<std::shared_ptr<ObjectGeneric>> &objectsInScene,
                                           const std::shared_ptr<ObjectGeneric> &object,
                                           Vector3d &color, double &intensity);
    };
}


#endif //RAY_TRACER_APPLICATION_LIGHTPOINT_HPP
