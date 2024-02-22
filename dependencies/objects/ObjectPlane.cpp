#include "ObjectPlane.hpp"
#include <cmath>

App::ObjectPlane::ObjectPlane() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 0.01 });
    m_uvMapType = uvPLANE;
}

App::ObjectPlane::~ObjectPlane() = default;

// tests intersection for the object
bool App::ObjectPlane::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    Ray backward = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);
    Vector3d orientation = backward.m_orientation;

    if (!isWithinProximityPrecision(orientation(2), 0.0)){
        // there is an intersection
        double t = -(backward.m_point1(2) / orientation(2));
        if (t>0.0){ // excluding behind the camera points
            double u = backward.m_point1(0) + (orientation(0) * t);
            double v = backward.m_point1(1) + (orientation(1) * t);
            if (abs(u) < 1.0 && abs(v) < 1.0){ // in the plane
                // configures hit information
                Vector3d intersectionLocal = backward.m_point1 + (t*orientation);
                hitInformation.pointOfIntersection = m_transformation.applyTransformation(intersectionLocal, FORWARD_TRANSFORMATION);

                Vector3d normalVector = {0.0,0.0,-1.0}; // constant for the plane
                hitInformation.normal = m_transformation.applyNorm(normalVector);
                hitInformation.normal.normalize();

                hitInformation.hitObject = this -> shared_from_this();
                hitInformation.localPointOfIntersection = intersectionLocal;
                hitInformation.uvCoordinates = {u,v};
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}
