#include "ObjectCuboid.hpp"
#include <cmath>

#define ARBITRARY_HIGH_NUMBER 100e6

App::ObjectCuboid::ObjectCuboid() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
}

App::ObjectCuboid::~ObjectCuboid() {

}

bool App::ObjectCuboid::isIntersecting(const App::Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal,
                                       Vector3d &localColor) {
    if (!m_isVisible){
        return false;
    }

    // Copy the ray and apply the backwards transform.
    Ray backwardsRay = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    // Moved these here from the header file.
    std::array<double, 6> t{};
    std::array<double, 6> u{};
    std::array<double, 6> v{};

    // Extract values of a.
    double ax = backwardsRay.m_point1(0);
    double ay = backwardsRay.m_point1(1);
    double az = backwardsRay.m_point1(2);

    // Extract the value of k.
    Vector3d k = backwardsRay.m_orientation;
    k.normalize();
    double kx = k(0);
    double ky = k(1);
    double kz = k(2);

    // Test for intersections with each plane (side of the box).
    // Top and bottom.
    if (!isWithinProximityPrecision(kz, 0.0)){
        t[0] = (az - 1.0) / -kz;
        t[1] = (az + 1.0) / -kz;
        u[0] = ax + kx * t[0];
        v[0] = ay + ky * t[0];
        u[1] = ax + kx * t[1];
        v[1] = ay + ky * t[1];
    }else{
        t[0] = ARBITRARY_HIGH_NUMBER;
        t[1] = ARBITRARY_HIGH_NUMBER;
        u[0] = 0.0;
        v[0] = 0.0;
        u[1] = 0.0;
        v[1] = 0.0;
    }

    // Left and right.
    if (!isWithinProximityPrecision(kx, 0.0)){
        t[2] = (ax + 1.0) / -kx;
        t[3] = (ax - 1.0) / -kx;
        u[2] = az + kz * t[2];
        v[2] = ay + ky * t[2];
        u[3] = az + kz * t[3];
        v[3] = ay + ky * t[3];
    }else{
        t[2] = ARBITRARY_HIGH_NUMBER;
        t[3] = ARBITRARY_HIGH_NUMBER;
        u[2] = 0.0;
        v[2] = 0.0;
        u[3] = 0.0;
        v[3] = 0.0;
    }

    // Front and back.
    if (!isWithinProximityPrecision(ky, 0.0)){
        t[4] = (ay + 1.0) / -ky;
        t[5] = (ay - 1.0) / -ky;
        u[4] = ax + kx * t[4];
        v[4] = az + kz * t[4];
        u[5] = ax + kx * t[5];
        v[5] = az + kz * t[5];
    }else{
        t[4] = ARBITRARY_HIGH_NUMBER;
        t[5] = ARBITRARY_HIGH_NUMBER;
        u[4] = 0.0;
        v[4] = 0.0;
        u[5] = 0.0;
        v[5] = 0.0;
    }

    // Find the index of the smallest non-negative value of t.
    double finalU = 0.0;
    double finalV = 0.0;
    double finalT = ARBITRARY_HIGH_NUMBER;
    int finalIndex = 0;
    bool validIntersection = false;
    for (int i=0; i<6; ++i){
        if ((t[i] < finalT) && (t[i] > 0.0) && (abs(u[i]) <= 1.0) && (abs(v[i]) <= 1.0)){
            finalT = t[i];
            finalIndex = i;
            finalU = u[i];
            finalV = v[i];
            validIntersection = true;
        }
    }

    if (validIntersection){
        // Compute the point of intersection.
        Vector3d pointOfIntersection = backwardsRay.m_point1 + finalT * k;

        // Compute the normal vector
        Vector3d normalVector;
        switch (finalIndex){
            case 0:
                normalVector = {0.0, 0.0, 1.0}; // Down.
                break;

            case 1:
                normalVector = {0.0, 0.0, -1.0}; // Up.
                break;

            case 2:
                normalVector = {-1.0, 0.0, 0.0}; // Left.
                break;

            case 3:
                normalVector = {1.0, 0.0, 0.0}; // Right.
                break;

            case 4:
                normalVector = {0.0, -1.0, 0.0}; // Backwards (towards the camera).
                break;

            case 5:
                normalVector = {0.0, 1.0, 0.0}; // Forwards (away from the camera).
                break;

        }

        // Transform the intersection point back into world coordinates.
        intersectionPoint = m_transformation.applyTransformation(pointOfIntersection, FORWARD_TRANSFORMATION);

        // Transform the normal vector.
        //Vector3d originOfObject(0.0,0.0,0.0);
        //Vector3d  newOriginOfObject = m_transformation.applyTransformation(originOfObject, FORWARD_TRANSFORMATION);
        //localNormal = intersectionPoint - newOriginOfObject;
        localNormal = m_transformation.applyNorm(normalVector);

        localNormal.normalize();

        // Return the base color.
        localColor = m_color;

        m_uvCoordinates = {finalU, finalV};

        return true;
    }
    else
    {
        return false;
    }
}

bool App::ObjectCuboid::isIntersecting(const App::Ray &castRay){
    if (!m_isVisible){
        return false;
    }

    // Copy the ray and apply the backwards transform.
    Ray bckRay = m_transformation.applyTransformation(castRay, BACKWARD_TRANSFORMATION);

    std::array<double, 6> t {100e6, 100e6, 100e6, 100e6, 100e6, 100e6};
    std::array<double, 6> u {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double, 6> v {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Extract values of a.
    double ax = bckRay.m_point1(0);
    double ay = bckRay.m_point1(1);
    double az = bckRay.m_point1(2);

    // Extract the value of k.
    Vector3d k = bckRay.m_orientation;
    double kx = k(0);
    double ky = k(1);
    double kz = k(2);

    // Test for intersections with each plane (side of the box).
    // Top and bottom.
    if (!isWithinProximityPrecision(kz, 0.0)){
        t[0] = (az - 1.0) / -kz;
        t[1] = (az + 1.0) / -kz;
        u[0] = ax + kx * t[0];
        v[0] = ay + ky * t[0];
        u[1] = ax + kx * t[1];
        v[1] = ay + ky * t[1];
    }

    // Left and right.
    if (!isWithinProximityPrecision(kx, 0.0)){
        t[2] = (ax + 1.0) / -kx;
        t[3] = (ax - 1.0) / -kx;
        u[2] = az + kz * t[2];
        v[2] = ay + ky * t[2];
        u[3] = az + kz * t[3];
        v[3] = ay + ky * t[3];
    }

    // Front and back.
    if (!isWithinProximityPrecision(ky, 0.0)){
        t[4] = (ay + 1.0) / -ky;
        t[5] = (ay - 1.0) / -ky;
        u[4] = ax + kx * t[4];
        v[4] = az + kz * t[4];
        u[5] = ax + kx * t[5];
        v[5] = az + kz * t[5];
    }

    // Find the index of the smallest non-negative value of t.
    /* Note that in the case of a bounding box, we are only interested
        in whether or not there was a valid intersection, we don't need
        to know which face of the box was actually involved. */
    bool validIntersection = false;
    int i = 0;
    while ((i < 6) && (!validIntersection))
    {
        if ((t[i] < 100e6) && (t[i] > 0.0) && (abs(u[i]) <= 1.0) && (abs(v[i]) <= 1.0))
        {
            validIntersection = true;
        }

        i++;
    }

    return validIntersection;
}