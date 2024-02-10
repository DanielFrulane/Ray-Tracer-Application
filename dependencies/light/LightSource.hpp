#ifndef RAY_TRACER_APPLICATION_LIGHTSOURCE_HPP
#define RAY_TRACER_APPLICATION_LIGHTSOURCE_HPP

#include <memory>
#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"
#include "../objects/ObjectGeneric.hpp"

using namespace Eigen;

namespace App {
    class LightSource {
    public:
        Vector3d m_color;
        Vector3d m_location;
        double m_intensity;

        LightSource();
        virtual ~LightSource();

        virtual bool calculateIllumination(const Vector3d &intersectionPoint, const Vector3d &localNormal,
                                           const std::vector<std::shared_ptr<App::ObjectGeneric>> &objectsInScene,
                                           const std::shared_ptr<App::ObjectGeneric> &object,
                                           Vector3d &color, double &intensity);
    };
}


#endif //RAY_TRACER_APPLICATION_LIGHTSOURCE_HPP
