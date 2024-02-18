#ifndef RAY_TRACER_APPLICATION_HITINFORMATION_HPP
#define RAY_TRACER_APPLICATION_HITINFORMATION_HPP

#include <memory>
#include "../include/Eigen/Eigen"

using namespace Eigen;

namespace App {
    class ObjectGeneric;

    struct HitInformation { // TODO inicializar ou n
        Vector3d pointOfIntersection = {0.0,0.0,0.0};
        Vector3d normal = {0.0,0.0,0.0};
        Vector3d color = {0.0,0.0,0.0};
        Vector3d localPointOfIntersection = {0.0,0.0,0.0};
        Vector2d uvCoordinates = {0.0,0.0};
        std::shared_ptr<ObjectGeneric> hitObject;
    };

}

#endif //RAY_TRACER_APPLICATION_HITINFORMATION_HPP
