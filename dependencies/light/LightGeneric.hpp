#ifndef RAY_TRACER_APPLICATION_LIGHTGENERIC_HPP
#define RAY_TRACER_APPLICATION_LIGHTGENERIC_HPP

#include <memory>
#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"
#include "../objects/ObjectGeneric.hpp"

using namespace Eigen;

namespace App {
    class LightGeneric {
    public:
        Vector3d m_color;
        Vector3d m_position;
        double m_intensity;

        LightGeneric();
        virtual ~LightGeneric();

        virtual bool calculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                           const std::vector<std::shared_ptr<App::ObjectGeneric>> &objectsInScene,
                                           const std::shared_ptr<App::ObjectGeneric> &object,
                                           Vector3d &color, double &intensity);
    };
}


#endif //RAY_TRACER_APPLICATION_LIGHTGENERIC_HPP
