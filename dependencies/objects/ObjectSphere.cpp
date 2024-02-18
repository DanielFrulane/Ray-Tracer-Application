#include "ObjectSphere.hpp"
#include <cmath>

App::ObjectSphere::ObjectSphere() {
    m_hasMaterial = false;
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvSPHERE;
}

App::ObjectSphere::~ObjectSphere() {

}

bool App::ObjectSphere::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    App::Ray backwardRay  = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    // ref doc ///////////////
    // calculate values for a, b, c
    Vector3d vOrientation = backwardRay.m_orientation;
    vOrientation.normalize();

    // a = 1.0
    double b = 2.0 * (backwardRay.m_point1).dot(vOrientation);
    double c = (backwardRay.m_point1).dot(backwardRay.m_point1) - 1.0;

    double delta = pow(b,2) - 4.0 * c;

    Vector3d  pointLocal;
    if (delta > 0.0){
        double squareRoot = sqrt(delta);
        double root1 = (-b + squareRoot) / 2.0;
        double root2 = (-b - squareRoot) / 2.0;

        // intersection behind camera exclusion
        if (root1 < 0.0 && root2 < 0.0){
            return false;
        } else { // select closest intersection to camera
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

            hitInformation.pointOfIntersection = m_transformation.applyTransformation(pointLocal, FORWARD_TRANSFORMATION);
            /*Vector3d originOfObject(0.0,0.0,0.0);
            Vector3d  newOriginOfObject = m_transformation.applyTransformation(originOfObject, FORWARD_TRANSFORMATION);
            localNormal = intersectionPoint - newOriginOfObject;
            localNormal.normalize();*/

            Vector3d normalVector = pointLocal;
            hitInformation.normal = m_transformation.applyNorm(normalVector);
            hitInformation.normal.normalize();

            hitInformation.color = m_color;
            hitInformation.hitObject = this -> shared_from_this();
            calculateUVSpace(pointLocal, hitInformation.uvCoordinates);
            hitInformation.localPointOfIntersection = pointLocal;

            /*double x = pointLocal(0);
            double y = pointLocal(1);
            double z = pointLocal(2);

            double u = atan(sqrt(pow(x,2.0) + pow(y, 2.0))/z);
            double v = atan(y/x);
            if (x < 0.0){
                v += M_PI;
            }
            u /= M_PI;
            v /= M_PI;
            m_uvCoordinates = {u,v};*/

            return true;
        }
    } else {
        return false;
    }
}
