#include "ObjectCylinder.hpp"
#include <cmath>
#define ARBITRARY_HIGH_NUMBER 100e6

App::ObjectCylinder::ObjectCylinder() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvCYLINDER;
}

App::ObjectCylinder::~ObjectCylinder() {

}

bool App::ObjectCylinder::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    Ray backwardRay = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    Vector3d vec = backwardRay.m_orientation;
    vec.normalize();

    Vector3d p = backwardRay.m_point1;

    // TODO REFACTOR
    double a = pow(vec(0), 2.0) + pow(vec(1), 2.0);
    double b = 2.0 * (p(0) * vec(0) + p(1) * vec(1));
    double c = std::pow(p(0), 2.0) + pow(p(1), 2.0) - 1.0;

    double numSQRT = sqrt(pow(b, 2.0) - 4 * a * c);

    std::array<Vector3d, 4> pointsOfIntersection;
    std::array<double, 4> t{};
    bool t1Valid, t2Valid, t3Valid, t4Valid;
    if (numSQRT > 0.0){
        t.at(0) = (-b + numSQRT) / (2 * a);
        t.at(1) = (-b - numSQRT) / (2 * a);

        // Compute the points of intersection.
        pointsOfIntersection.at(0) = backwardRay.m_point1 + (vec * t[0]);
        pointsOfIntersection.at(1) = backwardRay.m_point1 + (vec * t[1]);

        // Check if any of these are valid.
        if ((t.at(0) > 0.0) && (fabs(pointsOfIntersection.at(0)(2)) < 1.0)){
            t1Valid = true;
        }
        else{
            t1Valid = false;
            t.at(0) = ARBITRARY_HIGH_NUMBER;
        }

        if ((t.at(1) > 0.0) && (fabs(pointsOfIntersection.at(1)(2)) < 1.0)){
            t2Valid = true;
        }
        else{
            t2Valid = false;
            t.at(1) = ARBITRARY_HIGH_NUMBER;
        }
    }
    else{
        t1Valid = false;
        t2Valid = false;
        t.at(0) = ARBITRARY_HIGH_NUMBER;
        t.at(1) = ARBITRARY_HIGH_NUMBER;
    }
    // And test the end caps.
    if (isWithinProximityPrecision(vec(2), 0.0)){
        t3Valid = false;
        t4Valid = false;
        t.at(2) = ARBITRARY_HIGH_NUMBER;
        t.at(3) = ARBITRARY_HIGH_NUMBER;
    } else {
        // Compute the values of t.
        t.at(2) = (backwardRay.m_point1(2) - 1.0) / -vec(2);
        t.at(3) = (backwardRay.m_point1(2) + 1.0) / -vec(2);

        // Compute the points of intersection.
        pointsOfIntersection.at(2) = backwardRay.m_point1 + t.at(2) * vec;
        pointsOfIntersection.at(3) = backwardRay.m_point1 + t.at(3) * vec;

        // Check if these are valid.
        if ((t.at(2) > 0.0) && (sqrt(pow(pointsOfIntersection.at(2)(0), 2.0) + pow(pointsOfIntersection.at(2)(1), 2.0)) < 1.0)) {
            t3Valid = true;
        }else{
            t3Valid = false;
            t.at(2) = ARBITRARY_HIGH_NUMBER;
        }

        if ((t.at(3) > 0.0) && (sqrt(pow(pointsOfIntersection.at(3)(0), 2.0) + pow(pointsOfIntersection.at(3)(1), 2.0)) < 1.0)){
            t4Valid = true;
        }else{
            t4Valid = false;
            t.at(3) = ARBITRARY_HIGH_NUMBER;
        }
    }
    if ((!t1Valid) && (!t2Valid) && (!t3Valid) && (!t4Valid)){
        return false;
    }

    int minIndex = 0;
    double minValue = ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<4; ++i){
        if (t.at(i) < minValue){
            minValue = t.at(i);
            minIndex = i;
        }
    }

    Vector3d validPointOfIntersection = pointsOfIntersection.at(minIndex);
    if (minIndex < 2){
        // Transform the intersection point back into world coordinates.
        hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

        // Compute the local normal.
        Vector3d originalNormal = {0.0,0.0,0.0};
        Vector3d newNormal = {0.0,0.0,0.0};
        Vector3d localOrigin = {0.0, 0.0, 0.0};
        Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
        originalNormal = validPointOfIntersection;
        originalNormal(2) = 0.0; // circle cross-section of infinite cylinder
        hitInformation.normal = m_transformation.applyNorm(originalNormal);
        hitInformation.normal.normalize();

        //newNormal = m_transformation.applyTransformation(originalNormal, FORWARD_TRANSFORMATION) - globalOrigin;
        //newNormal.normalize();
        //localNormal = newNormal;
        hitInformation.color = m_color;
        hitInformation.hitObject = this -> shared_from_this();

        double x = validPointOfIntersection(0);
        double y = validPointOfIntersection(1);
        double z = validPointOfIntersection(2);

        double u = atan2(y,x)/M_PI;
        double v = z;

        m_uvCoordinates = {u,v};

        return true;
    } else {
        if (!isWithinProximityPrecision(vec(2), 0.0)) {
            // Check if we are inside the disk.
            if (sqrt(pow(validPointOfIntersection(0), 2.0) + pow(validPointOfIntersection(1), 2.0)) < 1.0) {
                // Transform the intersection point back into world coordinates.
                hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

                // Compute the local normal.
                Vector3d localOrigin = {0.0, 0.0, 0.0};
                Vector3d normalVector  = {0.0, 0.0, 0.0 + validPointOfIntersection(2)};
                Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
                hitInformation.normal = m_transformation.applyTransformation(normalVector, FORWARD_TRANSFORMATION) - globalOrigin;
                hitInformation.normal.normalize();

                //Vector3d normalVector  = {0.0, 0.0, validPointOfIntersection(2)};
                //localNormal = m_transformation.applyNorm(normalVector);
                //localNormal.normalize();

                // Return the base color.
                hitInformation.color = m_color;
                hitInformation.hitObject = this -> shared_from_this();

                double x = validPointOfIntersection(0);
                double y = validPointOfIntersection(1);

                m_uvCoordinates = {x,y};

                return true;
            } else {
                return false;
            }
        } else{
            return false;
        }
    }
    return false;
}
