#include "ObjectCuboid.hpp"
#include <cmath>

#define ARBITRARY_HIGH_NUMBER 100e6

App::ObjectCuboid::ObjectCuboid() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvCUBOID;
}

App::ObjectCuboid::~ObjectCuboid() = default;

// tests intersection for the object
bool App::ObjectCuboid::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    Ray backwardsRay = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    std::array<double, 6> t{};
    std::array<double, 6> u{};
    std::array<double, 6> v{};

    // extract values used to compare the intersections with the cuboid
    double ax = backwardsRay.m_point1(0);
    double ay = backwardsRay.m_point1(1);
    double az = backwardsRay.m_point1(2);

    Vector3d k = backwardsRay.m_orientation;
    k.normalize();
    double kx = k(0);
    double ky = k(1);
    double kz = k(2);

    // tests intersection with all the planes that define the cuboid
    if (!isWithinProximityPrecision(kz, 0.0)){ // top
        t[0] = (az - 1.0) / -kz;
        t[1] = (az + 1.0) / -kz;
        u[0] = ax + kx * t[0];
        v[0] = ay + ky * t[0];
        u[1] = ax + kx * t[1];
        v[1] = ay + ky * t[1];
    }else{ // bottom
        t[0] = ARBITRARY_HIGH_NUMBER;
        t[1] = ARBITRARY_HIGH_NUMBER;
        u[0] = 0.0;
        v[0] = 0.0;
        u[1] = 0.0;
        v[1] = 0.0;
    }

    if (!isWithinProximityPrecision(kx, 0.0)){ // left
        t[2] = (ax + 1.0) / -kx;
        t[3] = (ax - 1.0) / -kx;
        u[2] = az + kz * t[2];
        v[2] = ay + ky * t[2];
        u[3] = az + kz * t[3];
        v[3] = ay + ky * t[3];
    }else{ // right
        t[2] = ARBITRARY_HIGH_NUMBER;
        t[3] = ARBITRARY_HIGH_NUMBER;
        u[2] = 0.0;
        v[2] = 0.0;
        u[3] = 0.0;
        v[3] = 0.0;
    }

    if (!isWithinProximityPrecision(ky, 0.0)){ // front
        t[4] = (ay + 1.0) / -ky;
        t[5] = (ay - 1.0) / -ky;
        u[4] = ax + kx * t[4];
        v[4] = az + kz * t[4];
        u[5] = ax + kx * t[5];
        v[5] = az + kz * t[5];
    }else{ // back
        t[4] = ARBITRARY_HIGH_NUMBER;
        t[5] = ARBITRARY_HIGH_NUMBER;
        u[4] = 0.0;
        v[4] = 0.0;
        u[5] = 0.0;
        v[5] = 0.0;
    }

    // gets the index of the smallest non-negative value of t
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
        // configures hit information
        Vector3d pointOfIntersection = backwardsRay.m_point1 + finalT * k;

        Vector3d normalVector;
        switch (finalIndex){ // defines what plane is concerned
            case 0:
                normalVector = {0.0, 0.0, 1.0}; // down.
                break;
            case 1:
                normalVector = {0.0, 0.0, -1.0}; // up.
                break;
            case 2:
                normalVector = {-1.0, 0.0, 0.0}; // left.
                break;
            case 3:
                normalVector = {1.0, 0.0, 0.0}; // right.
                break;
            case 4:
                normalVector = {0.0, -1.0, 0.0}; // backwards (towards the camera)
                break;
            case 5:
                normalVector = {0.0, 1.0, 0.0}; // forwards (away from the camera)
                break;
        }

        hitInformation.pointOfIntersection = m_transformation.applyTransformation(pointOfIntersection, FORWARD_TRANSFORMATION);
        hitInformation.normal = m_transformation.applyNorm(normalVector);
        hitInformation.normal.normalize();
        hitInformation.hitObject = this->shared_from_this();
        hitInformation.uvCoordinates = {finalU, finalV};
        hitInformation.localPointOfIntersection = pointOfIntersection;

        return true;
    }else{
        return false;
    }
}

// tests intersection for the object for the case that it is considered as abounding box.
// calculations are simplified in order do make processing reduced
bool App::ObjectCuboid::isIntersecting(const App::Ray &castRay){
    Ray bckRay = m_transformation.applyTransformation(castRay, BACKWARD_TRANSFORMATION);

    std::array<double, 6> t {ARBITRARY_HIGH_NUMBER, ARBITRARY_HIGH_NUMBER, ARBITRARY_HIGH_NUMBER, ARBITRARY_HIGH_NUMBER, ARBITRARY_HIGH_NUMBER, ARBITRARY_HIGH_NUMBER};
    std::array<double, 6> u {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double, 6> v {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // extract values used to compare the intersections with the cuboid
    double ax = bckRay.m_point1(0);
    double ay = bckRay.m_point1(1);
    double az = bckRay.m_point1(2);

    Vector3d k = bckRay.m_orientation;
    double kx = k(0);
    double ky = k(1);
    double kz = k(2);

    // tests intersection with all the planes that define the cuboid
    if (!isWithinProximityPrecision(kz, 0.0)){ // top and bottom
        t[0] = (az - 1.0) / -kz;
        t[1] = (az + 1.0) / -kz;
        u[0] = ax + kx * t[0];
        v[0] = ay + ky * t[0];
        u[1] = ax + kx * t[1];
        v[1] = ay + ky * t[1];
    }

    if (!isWithinProximityPrecision(kx, 0.0)){ // left and right
        t[2] = (ax + 1.0) / -kx;
        t[3] = (ax - 1.0) / -kx;
        u[2] = az + kz * t[2];
        v[2] = ay + ky * t[2];
        u[3] = az + kz * t[3];
        v[3] = ay + ky * t[3];
    }

    if (!isWithinProximityPrecision(ky, 0.0)){ // front and back
        t[4] = (ay + 1.0) / -ky;
        t[5] = (ay - 1.0) / -ky;
        u[4] = ax + kx * t[4];
        v[4] = az + kz * t[4];
        u[5] = ax + kx * t[5];
        v[5] = az + kz * t[5];
    }

    // find the index of the smallest non-negative value of t
    bool validIntersection = false;
    int i = 0;
    while ((i < 6) && (!validIntersection)){
        if ((t[i] < 100e6) && (t[i] > 0.0) && (abs(u[i]) <= 1.0) && (abs(v[i]) <= 1.0)){
            validIntersection = true;
        }
        i++;
    }

    return validIntersection;
}