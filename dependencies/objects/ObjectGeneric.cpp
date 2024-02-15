#include "ObjectGeneric.hpp"
#include <math.h>

#define PRECISION_CONSTANT 1e-21f;
#define ARBITRARY_HIGH_NUMBER 1e6

App::ObjectGeneric::ObjectGeneric() {
    m_hasMaterial=false;
}

App::ObjectGeneric::~ObjectGeneric(){
    ;
}

bool App::ObjectGeneric::isIntersecting(const App::Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) {
    return false;
}

bool App::ObjectGeneric::isWithinProximityPrecision(const double float1, const double float2) {
    return fabs(float1-float2) < PRECISION_CONSTANT;
}

const Vector3d &App::ObjectGeneric::getColor() const {
    return m_color;
}

void App::ObjectGeneric::setColor(const Vector3d &color) {
    m_color = color;
}

void App::ObjectGeneric::setTransformation(const App::GeometricalTransformation &transformation) {
    m_transformation = transformation;
}

bool App::ObjectGeneric::setMaterial(const std::shared_ptr<MaterialGeneric> &material) {
    m_material = material;
    m_hasMaterial = true;
    return m_hasMaterial;
}

void App::ObjectGeneric::getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) {
    // Construct corners of a cube based on the current limits.
    std::vector<Vector3d> cornerPoints = getCuboid(-1.0,1.0,-1.0,1.0,-1.0,1.0);
    GeometricalTransformation combinedTransformation = m_transformation * m_boundingBoxTransformation;

    // Apply the transforms to the unit cube corner points and compute limits.
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

    // Return the limits.
    xLim(0) = minX;
    xLim(1) = maxX;
    yLim(0) = minY;
    yLim(1) = maxY;
    zLim(0) = minZ;
    zLim(1) = maxZ;
}

void App::ObjectGeneric::getExtents(const App::GeometricalTransformation &parentTransformationMatrix, Vector3d &xLim,
                                    Vector3d &yLim, Vector3d &zLim) {
    /// TODO
}

std::vector<Vector3d> App::ObjectGeneric::getCuboid(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
    std::vector<Vector3d> corners (8);
    corners.at(0) = {xMin - m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMin - m_boundingBoxPadding};
    corners.at(1) = {xMin - m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMax + m_boundingBoxPadding};
    corners.at(2) = {xMax + m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMax + m_boundingBoxPadding};
    corners.at(3) = {xMax + m_boundingBoxPadding, yMin - m_boundingBoxPadding, zMin - m_boundingBoxPadding};
    corners.at(4) = {xMin - m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMin - m_boundingBoxPadding};
    corners.at(5) = {xMin - m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMax - m_boundingBoxPadding}; // TODO VERIFY
    corners.at(6) = {xMax + m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMax + m_boundingBoxPadding};
    corners.at(7) = {xMax + m_boundingBoxPadding, yMax + m_boundingBoxPadding, zMin - m_boundingBoxPadding};
    return corners;
}
