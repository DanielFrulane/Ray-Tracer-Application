#ifndef RAY_TRACER_APPLICATION_CAMERA_HPP
#define RAY_TRACER_APPLICATION_CAMERA_HPP

#include "../include/Eigen/Eigen"
#include "Ray.hpp"

using namespace Eigen;

namespace App {
    class Camera {
    private:
        Vector3d m_position;
        Vector3d m_lookAt;
        Vector3d m_up; // what is perceived as "up" in the scene
        Vector3d m_alignmentVector; // orientation
        Vector3d m_screenCenter;
        Vector3d m_u, m_v;
        // defining the display resolution
        double m_length, m_horizontalSize, m_aspectRatio;

    public:
        Camera();

        bool generateRay(double projectionScreenX, double projectionScreenY, Ray &parameterRay);
        void updateStatus();

        void setPosition(const Vector3d &newPosition);
        void setLookAt(const Vector3d &newLookAt);
        void setUp(const Vector3d &newUp);
        void setLength(double newLength);
        void setHorizontalSize(double newHorizontalSize);
        void setAspectRatio(double newAspectRatio);

        Vector3d getPosition();
        Vector3d getLookAt();
        Vector3d getUp();
        Vector3d getU();
        Vector3d getV();
        Vector3d getScreenCenter();
        double getLength() const;
        double getHorizontalSize() const;
        double getAspectRatio() const;
    };
}

#endif //RAY_TRACER_APPLICATION_CAMERA_HPP
