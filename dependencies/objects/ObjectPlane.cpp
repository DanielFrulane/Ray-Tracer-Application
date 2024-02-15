#include "ObjectPlane.hpp"
#include <cmath>

App::ObjectPlane::ObjectPlane() {
    m_hasMaterial = false;
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 0.01 });
}

App::ObjectPlane::~ObjectPlane() {

}

bool App::ObjectPlane::isIntersecting(const App::Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal,
                                      Vector3d &localColor) {
    Ray backward = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    Vector3d orientation = backward.m_orientation;
    orientation.normalize();

    if (!isWithinProximityPrecision(orientation(2), 0.0)){
        // there is an intersection
        double t = -(backward.m_point1(2) / orientation(2));
        if (t>0.0){ // excluding behind the camera points
            double u = backward.m_point1(0) + (orientation(0) * t);
            double v = backward.m_point1(1) + (orientation(1) * t);
            if (abs(u) < 1.0 && abs(v) < 1.0){
                // in the plane
                Vector3d intersectionLocal = backward.m_point1 + (t*orientation);
                intersectionPoint = m_transformation.applyTransformation(intersectionLocal, FORWARD_TRANSFORMATION);

                /*Vector3d localOrigin = {0.0, 0.0, 0.0};
                Vector3d normalVector = {0.0, 0.0, -1.0};
                Vector3d globalOrigin =m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
                localNormal = m_transformation.applyTransformation(normalVector, FORWARD_TRANSFORMATION) - globalOrigin;
                localNormal.normalize();*/

                Vector3d normalVector = {0.0,0.0,-1.0}; // constant
                localNormal = m_transformation.applyNorm(normalVector);
                localNormal.normalize();

                localColor = m_color;

                m_uvCoordinates = {u, v};
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
    return false;
}
