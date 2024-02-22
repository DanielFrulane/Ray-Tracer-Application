#include "ObjectGeneric.hpp"
#include <cmath>

#define PRECISION_CONSTANT 1e-21f;
#define ARBITRARY_HIGH_NUMBER 1e6

App::ObjectGeneric::ObjectGeneric() = default;

App::ObjectGeneric::~ObjectGeneric()= default;

bool App::ObjectGeneric::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    return false;
}

bool App::ObjectGeneric::isWithinProximityPrecision(const double float1, const double float2) {
    return abs(float1-float2) < PRECISION_CONSTANT;
}

void App::ObjectGeneric::setTransformation(const App::GeometricalTransformation &transformation) {
    m_transformation = transformation;
}

void App::ObjectGeneric::setMaterial(const std::shared_ptr<MaterialGeneric> &material) {
    m_material = material;
}

// precise the extents of the bounding box
void App::ObjectGeneric::getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) {
    // gets corners of a cuboid based on the current limits.
    std::vector<Vector3d> cornerPoints = getCuboid(-1.0,1.0,-1.0,1.0,-1.0,1.0);
    GeometricalTransformation combinedTransformation = m_transformation * m_boundingBoxTransformation;

    // applies the compositions transformation to the cuboid
    double minX = ARBITRARY_HIGH_NUMBER;
    double minY = ARBITRARY_HIGH_NUMBER;
    double minZ = ARBITRARY_HIGH_NUMBER;
    double maxX = -ARBITRARY_HIGH_NUMBER;
    double maxY = -ARBITRARY_HIGH_NUMBER;
    double maxZ = -ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<8; ++i){
        cornerPoints.at(i) = combinedTransformation.applyTransformation(cornerPoints.at(i), FORWARD_TRANSFORMATION);

        if (cornerPoints.at(i)(0) < minX){
            minX = cornerPoints.at(i)(0);            
        }
        if (cornerPoints.at(i)(0) > maxX){
            maxX = cornerPoints.at(i)(0);            
        }

        if (cornerPoints.at(i)(1) < minY){
            minY = cornerPoints.at(i)(1);            
        }
        if (cornerPoints.at(i)(1) > maxY){
            maxY = cornerPoints.at(i)(1);            
        }

        if (cornerPoints.at(i)(2) < minZ){
            minZ = cornerPoints.at(i)(2);            
        }
        if (cornerPoints.at(i)(2) > maxZ){
            maxZ = cornerPoints.at(i)(2);            
        }
    }

    xLim(0) = minX;
    xLim(1) = maxX;
    yLim(0) = minY;
    yLim(1) = maxY;
    zLim(0) = minZ;
    zLim(1) = maxZ;
}

// returns a cuboid defined with the dimensions
std::vector<Vector3d> App::ObjectGeneric::getCuboid(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
    std::vector<Vector3d> corners (8); // 8 vertices
    corners.at(0) = {xMin, yMin, zMin};
    corners.at(1) = {xMin, yMin, zMax};
    corners.at(2) = {xMax, yMin, zMax};
    corners.at(3) = {xMax, yMin, zMin};
    corners.at(4) = {xMin, yMax, zMin};
    corners.at(5) = {xMin, yMax, zMax};
    corners.at(6) = {xMax, yMax, zMax};
    corners.at(7) = {xMax, yMax, zMin};
    return corners;
}

// calculates the UV projection for each type of surface
void App::ObjectGeneric::calculateUVSpace(const Vector3d &localPointOfIntersection, Vector2d &uvCoordinates) const {
    switch (m_uvMapType) {
        case uvSPHERE:{
            double x = localPointOfIntersection(0);
            double y = localPointOfIntersection(1);
            double z = localPointOfIntersection(2);
            double u = atan2(y, x) / M_PI;
            double v = 2.0 * (atan2(sqrt(pow(x, 2.0) + pow(y, 2.0)), z) / M_PI) - 1.0;
            uvCoordinates(0) = u;
            uvCoordinates(1) = v;
            break;
        }
        case uvPLANE:{
            uvCoordinates(0) = localPointOfIntersection(0);
            uvCoordinates(1) = localPointOfIntersection(1);
            break;
        }
        case uvCYLINDER:{
            double x = localPointOfIntersection(0);
            double y = localPointOfIntersection(1);
            double z = localPointOfIntersection(2);
            double u = atan2(y, x) / M_PI;
            double v = -z;
            uvCoordinates(0) = u;
            uvCoordinates(1) = v;
            break;
        }
        case uvCUBOID:{
            double x = localPointOfIntersection(0);
            double y = localPointOfIntersection(1);
            double z = localPointOfIntersection(2);
            double u = 0.0;
            double v = 0.0;

            Matrix3d uvTransformation = Matrix3d::Identity();
            if (isWithinProximityPrecision(x, -1.0)){ // left face
                u = -(y * 0.25) - 0.75;
                v = -z * 0.25;
            } else if (isWithinProximityPrecision(x, 1.0)){ // right face
                u = (y * 0.25) + 0.25;
                v = -z * 0.25;
            } else if (isWithinProximityPrecision(y, -1.0)){ // front face
                u = (x * 0.25) - 0.25;
                v = (-z * 0.25) - 0.5;
            } else if (isWithinProximityPrecision(y, 1.0)){ // back face
                u = -(x * 0.25) - 0.25;
                v = (-z * 0.25) + 0.5;
            } else if (isWithinProximityPrecision(z, -1.0)){ // top face
                u = (x * 0.25) + 0.75;
                v = y * 0.25;
            } else if (isWithinProximityPrecision(z, 1.0)){ // bottom face
                u = (x * 0.25) - 0.25;
                v = y * 0.25;
            }
            // else: object is not a cuboid

            uvCoordinates(0) = u;
            uvCoordinates(1) = v;
            break;
        }
    }
}
