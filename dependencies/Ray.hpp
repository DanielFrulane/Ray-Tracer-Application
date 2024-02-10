#ifndef RAY_TRACER_APPLICATION_RAY_HPP
#define RAY_TRACER_APPLICATION_RAY_HPP

#include <vector>
#include "../include/Eigen/Eigen"

using namespace Eigen;

namespace App {
    class Ray {
    public:
        Vector3d m_point1;
        Vector3d m_point2;
        Vector3d m_orientation;

    public:
        Ray();
        Ray(const Vector3d &mPoint1, const Vector3d &mPoint2, const Vector3d &mOrientation);
        Ray(const Vector3d &point1, const Vector3d &point2);
        Ray(const Ray &copyRay);

        Vector3d getPoint1() const;
        Vector3d getPoint2() const;
        void updateOrientation();
    };
}


#endif //RAY_TRACER_APPLICATION_RAY_HPP
