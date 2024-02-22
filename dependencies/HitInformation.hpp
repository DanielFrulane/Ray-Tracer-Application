#ifndef RAY_TRACER_APPLICATION_HITINFORMATION_HPP
#define RAY_TRACER_APPLICATION_HITINFORMATION_HPP

#include <memory>
#include "../include/Eigen/Eigen"

// general information of an intersection between a ray and an object

using namespace Eigen;

namespace App {
    class ObjectGeneric;

    struct HitInformation {
        Vector3d color;
        Vector3d normal; // vector normal to the intersection
        Vector2d uvCoordinates;
        Vector3d pointOfIntersection;
        Vector3d localPointOfIntersection; // for calculating UV spaces
        std::shared_ptr<ObjectGeneric> hitObject; // what object was hit
    };

}

#endif //RAY_TRACER_APPLICATION_HITINFORMATION_HPP
