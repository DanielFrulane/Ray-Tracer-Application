#include "ObjectSphere.hpp"
#include <cmath>

App::ObjectSphere::ObjectSphere() {

}

App::ObjectSphere::~ObjectSphere() {

}

bool App::ObjectSphere::isIntersecting(const App::Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) {
    //Ray backwardRay = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    // ref doc ///////////////
    // calculate values for a, b, c
    // Vector3d vOrientation = backwardRay.m_orientation;
    Vector3d vOrientation = rayCasted.m_orientation;
    vOrientation.normalize();

    // a = 1.0
    double b = 2.0 * (rayCasted.m_point1).dot(vOrientation);
    double c = (rayCasted.m_point1).dot(rayCasted.m_point1) - 1.0;
    //double b = 2.0 * (backwardRay.m_point1).dot(vOrientation);
    //double c = (backwardRay.m_point1).dot(backwardRay.m_point1) - 1.0;

    double delta = pow(b,2) - 4.0 * c;

    //Vector3d  pointLocal;
    if (delta > 0.0){
        double squareRoot = sqrt(delta);
        double root1 = (-b + squareRoot) / 2.0;
        double root2 = (-b - squareRoot) / 2.0;

        // intersection behind camera exclusion
        if (root1 < 0.0 || root2 < 0.0){
            return false;
        } else { // select closest intersection to camera
            if (root1 < root2){
                intersectionPoint = rayCasted.m_point1 + (vOrientation * root1);
                //pointLocal = backwardRay.m_point1 + (vOrientation * root1);
            } else {
                intersectionPoint = rayCasted.m_point1 + (vOrientation * root2);
                //pointLocal = backwardRay.m_point1 + (vOrientation * root2);
            }

            //intersectionPoint = m_transformation.applyTransformation(pointLocal, FORWARD_TRANSFORMATION);
            //Vector3d originOfObject(0.0,0.0,0.0);
            //Vector3d  newOriginOfObject = m_transformation.applyTransformation(originOfObject, FORWARD_TRANSFORMATION);
            localNormal = intersectionPoint;// - newOriginOfObject; // TODO
            localNormal.normalize();

            localColor = m_color;
        }
        return true;
    } else {
        return false;
    }
}
