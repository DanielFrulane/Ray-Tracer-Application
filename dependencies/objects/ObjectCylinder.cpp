#include "ObjectCylinder.hpp"
#include <cmath>

#define ARBITRARY_HIGH_NUMBER 100e6

App::ObjectCylinder::ObjectCylinder() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvCYLINDER;
}

App::ObjectCylinder::~ObjectCylinder() = default;

// tests intersection for the object
bool App::ObjectCylinder::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    Ray backwardRay = m_transformation.applyTransformation(rayCasted, BACKWARD_TRANSFORMATION);

    Vector3d vec = backwardRay.m_orientation;
    vec.normalize();

    Vector3d p = backwardRay.m_point1; // start point

    // calculates intersection parameters
    double a = pow(vec(0), 2.0) + pow(vec(1), 2.0);
    double b = 2.0 * (p(0) * vec(0) + p(1) * vec(1));
    double c = std::pow(p(0), 2.0) + pow(p(1), 2.0) - 1.0;
    double numSQRT = sqrt(pow(b, 2.0) - 4 * a * c);

    std::array<Vector3d, 4> pointsOfIntersection;
    std::array<double, 4> t{};
    bool t1Valid, t2Valid, t3Valid, t4Valid;
    if (numSQRT > 0.0){ // checks circular part of the object
        t.at(0) = (-b + numSQRT) / (2 * a);
        t.at(1) = (-b - numSQRT) / (2 * a);

        pointsOfIntersection.at(0) = backwardRay.m_point1 + (vec * t[0]);
        pointsOfIntersection.at(1) = backwardRay.m_point1 + (vec * t[1]);

        // check valid values
        if ((t.at(0) > 0.0) && (fabs(pointsOfIntersection.at(0)(2)) < 1.0)){
            t1Valid = true;
        }else{
            t1Valid = false;
            t.at(0) = ARBITRARY_HIGH_NUMBER;
        }

        if ((t.at(1) > 0.0) && (fabs(pointsOfIntersection.at(1)(2)) < 1.0)){
            t2Valid = true;
        }else{
            t2Valid = false;
            t.at(1) = ARBITRARY_HIGH_NUMBER;
        }
    }else{
        t1Valid = false;
        t2Valid = false;
        t.at(0) = ARBITRARY_HIGH_NUMBER;
        t.at(1) = ARBITRARY_HIGH_NUMBER;
    }

    // checks flat part of the object
    if (isWithinProximityPrecision(vec(2), 0.0)){
        t3Valid = false;
        t4Valid = false;
        t.at(2) = ARBITRARY_HIGH_NUMBER;
        t.at(3) = ARBITRARY_HIGH_NUMBER;
    } else {
        t.at(2) = (backwardRay.m_point1(2) - 1.0) / -vec(2);
        t.at(3) = (backwardRay.m_point1(2) + 1.0) / -vec(2);

        pointsOfIntersection.at(2) = backwardRay.m_point1 + t.at(2) * vec;
        pointsOfIntersection.at(3) = backwardRay.m_point1 + t.at(3) * vec;

        // check valid values
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
        return false; // case no intersection
    }

    // gets the smallest valid value of t
    int minimalIndex = 0;
    double minValue = ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<4; ++i){
        if (t.at(i) < minValue){
            minValue = t.at(i);
            minimalIndex = i;
        }
    }

    Vector3d validPointOfIntersection = pointsOfIntersection.at(minimalIndex);
    if (minimalIndex < 2){ // it is the circular part of the object
        hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

        // configures hit details
        Vector3d originalNormal = {0.0,0.0,0.0};
        Vector3d newNormal = {0.0,0.0,0.0};
        Vector3d localOrigin = {0.0, 0.0, 0.0};
        Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);

        originalNormal = validPointOfIntersection;
        originalNormal(2) = 0.0; // circle cross-section of infinite cylinder
        hitInformation.normal = m_transformation.applyNorm(originalNormal);
        hitInformation.normal.normalize();

        hitInformation.hitObject = this -> shared_from_this();

        calculateUVSpace(validPointOfIntersection, hitInformation.uvCoordinates);
        hitInformation.localPointOfIntersection = validPointOfIntersection;

        return true;
    } else { // it is the base of the object
        if (!isWithinProximityPrecision(vec(2), 0.0)) {
            // check if inside the disk
            if (sqrt(pow(validPointOfIntersection(0), 2.0) + pow(validPointOfIntersection(1), 2.0)) < 1.0) {
                // configures hit information
                hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

                Vector3d localOrigin = {0.0, 0.0, 0.0};
                Vector3d normalVector  = {0.0, 0.0, 0.0 + validPointOfIntersection(2)};
                Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
                hitInformation.normal = m_transformation.applyTransformation(normalVector, FORWARD_TRANSFORMATION) - globalOrigin;
                hitInformation.normal.normalize();

                hitInformation.hitObject = this -> shared_from_this();


                calculateUVSpace(validPointOfIntersection, hitInformation.uvCoordinates);
                hitInformation.localPointOfIntersection = validPointOfIntersection;

                return true;
            } else {
                return false;
            }
        } else{
            return false;
        }
    }
}
