#include "ObjectCone.hpp"
#include <cmath>

#define ARBITRARY_HIGH_NUMBER 100e6

App::ObjectCone::ObjectCone() {
    m_boundingBoxTransformation.setTransformation({ 0.0, 0.0, 0.0 },
                                                  { 0.0, 0.0, 0.0 },
                                                  { 1.0, 1.0, 1.0 });
    m_uvMapType = uvCYLINDER;
}

App::ObjectCone::~ObjectCone() {

}

bool App::ObjectCone::isIntersecting(const Ray &castedRay, HitInformation &hitInformation) {
    // Copy the ray and applyTransformation the backwards transform.
    Ray backward = m_transformation.applyTransformation(castedRay, BACKWARD_TRANSFORMATION);

    // Copy the m_lab vector from bckRay and normalize it.
    Vector3d vec = backward.m_orientation;
    vec.normalize();

    // Get the start point of the line.
    Vector3d p = backward.m_point1;

    // Compute a, b and c.
    double a = pow(vec(0), 2.0) + pow(vec(1), 2.0) - pow(vec(2), 2.0);
    double b = 2 * (p(0) * vec(0) + p(1) * vec(1) - p(2) * vec(2));
    double c = pow(p(0), 2.0) + pow(p(1), 2.0) - pow(p(2), 2.0);

    // Compute b^2 - 4ac.
    double numSQRT = sqrt(pow(b, 2.0) - 4 * a * c);

    std::array<Vector3d, 3> pointOfIntersection;
    std::array<double, 3> t{};
    bool t1Valid, t2Valid, t3Valid;
    if (numSQRT > 0.0){
        // Compute the values of t.
        t.at(0) = (-b + numSQRT) / (2 * a);
        t.at(1) = (-b - numSQRT) / (2 * a);

        // Compute the points of intersection.
        pointOfIntersection.at(0) = backward.m_point1 + (vec * t[0]);
        pointOfIntersection.at(1) = backward.m_point1 + (vec * t[1]);

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

    // And test the end cap.
    if (isWithinProximityPrecision(vec(2), 0.0)){
        t3Valid = false;
        t.at(2) = ARBITRARY_HIGH_NUMBER;
    }
    else{
        // Compute values for t.
        t.at(2) = (backward.m_point1(2) - 1.0) / -vec(2);

        // Compute points of intersection.
        pointOfIntersection.at(2) = backward.m_point1 + t.at(2) * vec;

        // Check if these are valid.
        if ((t.at(2) > 0.0) && (sqrt(pow(pointOfIntersection.at(2)(0), 2.0) + pow(pointOfIntersection.at(2)(1), 2.0)) < 1.0))
        {
            t3Valid = true;
        }else{
            t3Valid = false;
            t.at(2) = ARBITRARY_HIGH_NUMBER;
        }
    }

    // If no valid intersections found, then we can stop.
    if ((!t1Valid) && (!t2Valid) && (!t3Valid))
        return false;

    // Check for the smallest valid value of t.
    int minimalIndex = 0;
    double minimalValue = ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<3; ++i){
        if (t.at(i) < minimalValue){
            minimalValue = t.at(i);
            minimalIndex = i;
        }
    }

    /* If minIndex is either 0 or 1, then we have a valid intersection
        with the cone itself. */
    Vector3d validPointOfIntersection = pointOfIntersection.at(minimalIndex);
    if (minimalIndex < 2){
        // Transform the intersection point back into world coordinates.
        hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

        // Compute the local normal.
        Vector3d originalNormal;
        Vector3d newNormal;
        Vector3d localOrigin = {0.0, 0.0, 0.0};
        Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
        double tX = validPointOfIntersection(0);
        double tY = validPointOfIntersection(1);
        double tZ = -sqrt(pow(tX, 2.0) + pow(tY, 2.0));

        Vector3d txyz = {tX, tY, tZ};
        originalNormal = txyz;

        originalNormal.normalize();
        //newNormal = m_transformation.applyTransformation(originalNormal, FORWARD_TRANSFORMATION) - globalOrigin;
        //newNormal.normalize();
        //localNormal = newNormal; EXCLUIR DEPENDENCIAS

        hitInformation.normal = m_transformation.applyNorm(originalNormal);
        hitInformation.normal.normalize();

        // Return the base color.
        hitInformation.hitObject = this -> shared_from_this();

        double x = validPointOfIntersection(0);
        double y = validPointOfIntersection(1);
        double z = validPointOfIntersection(2);

        double u = atan2(y,x)/M_PI;
        double v = (z * 2.0) + 1.0;

        calculateUVSpace(validPointOfIntersection, hitInformation.uvCoordinates);
        hitInformation.localPointOfIntersection = validPointOfIntersection;
        //m_uvCoordinates = {u,v};

        return true;
    }else{
        // Check the end cap.
        if (!isWithinProximityPrecision(vec(2), 0.0)){
            // Check if we are inside the disk.
            if (sqrt(pow(validPointOfIntersection(0), 2.0) + pow(validPointOfIntersection(1), 2.0)) < 1.0){
                // Transform the intersection point back into world coordinates.
                hitInformation.pointOfIntersection = m_transformation.applyTransformation(validPointOfIntersection, FORWARD_TRANSFORMATION);

                // Compute the local normal.
                Vector3d localOrigin = {0.0, 0.0, 0.0};
                Vector3d normalVector = {0.0, 0.0, 1.0};
                Vector3d globalOrigin = m_transformation.applyTransformation(localOrigin, FORWARD_TRANSFORMATION);
                hitInformation.normal = m_transformation.applyTransformation(normalVector, FORWARD_TRANSFORMATION) - globalOrigin;
                //localNormal = m_transformation.applyNorm(normalVector);
                hitInformation.normal.normalize();

                // Return the base color.
                hitInformation.hitObject = this -> shared_from_this();

                double x = validPointOfIntersection(0);
                double y = validPointOfIntersection(1);
                double z = validPointOfIntersection(2);

                calculateUVSpace(validPointOfIntersection, hitInformation.uvCoordinates);
                hitInformation.localPointOfIntersection = validPointOfIntersection;
                //m_uvCoordinates = {x,y};

                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    return false;
}
