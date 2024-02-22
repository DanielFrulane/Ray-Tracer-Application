#include "ObjectSphere.hpp"
#include <cmath>

App::ObjectSphere::ObjectSphere() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvSPHERE;
}

App::ObjectSphere::~ObjectSphere() = default;

// tests intersection for the object
bool App::ObjectSphere::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    App::Ray backwardRay  = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    // calculate values for a, b, c
    Vector3d vOrientation = backwardRay.m_orientation;
    vOrientation.normalize();

    // a = 1; always
    double b = 2.0 * (backwardRay.m_point1).dot(vOrientation);
    double c = (backwardRay.m_point1).dot(backwardRay.m_point1) - 1.0;
    double delta = pow(b,2) - 4.0 * c;

    Vector3d  pointLocal;
    if (delta > 0.0){
        double squareRoot = sqrt(delta);
        double root1 = (-b + squareRoot) / 2.0;
        double root2 = (-b - squareRoot) / 2.0;

        // excludes intersection behind the camera
        if (root1 < 0.0 && root2 < 0.0){
            return false;
        } else { // select closest intersection to the camera
            if (root1 < root2){
                if (root1 > 0.0){
                    pointLocal= backwardRay.m_point1 + (vOrientation * root1);
                } else {
                    if (root2 > 0.0){
                        pointLocal = backwardRay.m_point1 + (vOrientation * root2);
                    } else {
                        return false;
                    }
                }
            } else { // root1 > root2
                if (root2 > 0.0){
                    pointLocal= backwardRay.m_point1 + (vOrientation * root2);
                } else {
                    if (root1 > 0.0){
                        pointLocal = backwardRay.m_point1 + (vOrientation * root1);
                    } else {
                        return false;
                    }
                }
            }

            // configures hit information
            hitInformation.pointOfIntersection = m_transformation.applyTransformation(pointLocal, FORWARD_TRANSFORMATION);

            Vector3d normalVector = pointLocal;
            hitInformation.normal = m_transformation.applyNorm(normalVector);
            hitInformation.normal.normalize();

            hitInformation.hitObject = this -> shared_from_this();
            calculateUVSpace(pointLocal, hitInformation.uvCoordinates);
            hitInformation.localPointOfIntersection = pointLocal;

            return true;
        }
    } else {
        return false;
    }
}
