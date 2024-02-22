#include "ObjectCone.hpp"
#include <cmath>

#define ARBITRARY_HIGH_NUMBER 100e6

App::ObjectCone::ObjectCone() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvCYLINDER;
}

App::ObjectCone::~ObjectCone() = default;

// tests intersection for the object
bool App::ObjectCone::isIntersecting(const Ray &castedRay, HitInformation &hitInformation) {
    Ray backward = m_transformation.applyTransformation(castedRay, BACKWARD_TRANSFORMATION);

    Vector3d vec = backward.m_orientation;
    vec.normalize();

    Vector3d p = backward.m_point1;  // start point

    // calculates intersection parameters
    double a = pow(vec(0), 2.0) + pow(vec(1), 2.0) - pow(vec(2), 2.0);
    double b = 2 * (p(0) * vec(0) + p(1) * vec(1) - p(2) * vec(2));
    double c = pow(p(0), 2.0) + pow(p(1), 2.0) - pow(p(2), 2.0);
    double numSQRT = sqrt(pow(b, 2.0) - 4 * a * c);

    std::array<Vector3d, 3> pointOfIntersection;
    std::array<double, 3> t{};
    bool t1Valid, t2Valid, t3Valid;
    if (numSQRT > 0.0){ // checks circular part of the object
        t.at(0) = (-b + numSQRT) / (2 * a);
        t.at(1) = (-b - numSQRT) / (2 * a);

        pointOfIntersection.at(0) = backward.m_point1 + (vec * t[0]);
        pointOfIntersection.at(1) = backward.m_point1 + (vec * t[1]);

        // check valid values
        if ((t.at(0) > 0.0) && (pointOfIntersection.at(0)(2) > 0.0) && (pointOfIntersection.at(0)(2) < 1.0)){
            t1Valid = true;
        }else{
            t1Valid = false;
            t.at(0) = ARBITRARY_HIGH_NUMBER;
        }

        if ((t.at(1) > 0.0) && (pointOfIntersection.at(1)(2) > 0.0) && (pointOfIntersection.at(1)(2) < 1.0)){
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
        t.at(2) = ARBITRARY_HIGH_NUMBER;
    }
    else{
        t.at(2) = (backward.m_point1(2) - 1.0) / -vec(2);

        pointOfIntersection.at(2) = backward.m_point1 + t.at(2) * vec;

        // check valid values
        if ((t.at(2) > 0.0) && (sqrt(pow(pointOfIntersection.at(2)(0), 2.0) + pow(pointOfIntersection.at(2)(1), 2.0)) < 1.0)){
            t3Valid = true;
        }else{
            t3Valid = false;
            t.at(2) = ARBITRARY_HIGH_NUMBER;
        }
    }

    if ((!t1Valid) && (!t2Valid) && (!t3Valid)){
        return false;  // case no intersection
    }

    // gets the smallest valid value of t
    int minimalIndex = 0;
    double minimalValue = ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<3; ++i){
        if (t.at(i) < minimalValue){
            minimalValue = t.at(i);
            minimalIndex = i;
        }
    }

    Vector3d validPointOfIntersection = pointOfIntersection.at(minimalIndex);
    if (minimalIndex < 2){ // it is the circular part of the object
        hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

        // configures hit details
        Vector3d originalNormal = {0.0,0.0,0.0};
        Vector3d newNormal = {0.0,0.0,0.0};
        Vector3d localOrigin = {0.0, 0.0, 0.0};
        Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
        double tX = validPointOfIntersection(0);
        double tY = validPointOfIntersection(1);
        double tZ = -sqrt(pow(tX, 2.0) + pow(tY, 2.0));

        Vector3d txyz = {tX, tY, tZ};
        originalNormal = txyz;
        originalNormal.normalize();
        hitInformation.normal = m_transformation.applyNorm(originalNormal);
        hitInformation.normal.normalize();

        hitInformation.hitObject = this -> shared_from_this();

        calculateUVSpace(validPointOfIntersection, hitInformation.uvCoordinates);
        hitInformation.localPointOfIntersection = validPointOfIntersection;

        return true;
    }else{ // it is the base of the object
        if (!isWithinProximityPrecision(vec(2), 0.0)){
            // check if inside the disk
            if (sqrt(pow(validPointOfIntersection(0), 2.0) + pow(validPointOfIntersection(1), 2.0)) < 1.0){
                // configures hit information
                hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

                Vector3d localOrigin = {0.0, 0.0, 0.0};
                Vector3d normalVector = {0.0, 0.0, 1.0};
                Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
                hitInformation.normal = m_transformation.applyTransformation(normalVector, FORWARD_TRANSFORMATION) - globalOrigin;
                hitInformation.normal.normalize();

                hitInformation.hitObject = this -> shared_from_this();

                calculateUVSpace(validPointOfIntersection, hitInformation.uvCoordinates);
                hitInformation.localPointOfIntersection = validPointOfIntersection;

                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
}
