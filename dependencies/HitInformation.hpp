#ifndef RAY_TRACER_APPLICATION_HITINFORMATION_HPP
#define RAY_TRACER_APPLICATION_HITINFORMATION_HPP

#include <memory>
#include "../include/Eigen/Eigen"

using namespace Eigen;

namespace App {
    class ObjectGeneric;

    struct hitInformation {
        Vector3d pointOfIntersection;
        Vector3d normal;
        Vector3d color;
        Vector3d localPointOfIntersection;
        Vector3d uvCoordinates;
        std::shared_ptr<ObjectGeneric> hitObject;
    };

}

#endif //RAY_TRACER_APPLICATION_HITINFORMATION_HPP
