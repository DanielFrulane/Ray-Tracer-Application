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

bool App::ObjectGeneric::isIntersecting(const App::Ray &rayCasted, HitInformation &hitInformation) {
    return false;
}

bool App::ObjectGeneric::isWithinProximityPrecision(const double float1, const double float2) {
    return fabs(float1-float2) < PRECISION_CONSTANT;
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
    std::cout<<"transformations"<<std::endl;
    std::cout<<m_transformation.getForward()<<std::endl;
    std::cout<<m_boundingBoxTransformation.getForward()<<std::endl;
    std::cout<<combinedTransformation.getForward()<<std::endl;

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

void App::ObjectGeneric::calculateUVSpace(const Vector3d &localPointOfIntersection, Vector2d &uvCoordinates) {
    switch (m_uvMapType) {
        case uvSPHERE:
        {
            // Spherical projection.
            double x = localPointOfIntersection(0);
            double y = localPointOfIntersection(1);
            double z = localPointOfIntersection(2);
            double u = atan2(y, x) / M_PI;
            double v = 2.0 * (atan2(sqrt(pow(x, 2.0) + pow(y, 2.0)), z) / M_PI) - 1.0;
            uvCoordinates(0) = u;
            uvCoordinates(1) = v;
            break;
        }
        case uvPLANE:
        {
            // Plane projection.
            uvCoordinates(0) = localPointOfIntersection(0);
            uvCoordinates(1) = localPointOfIntersection(1);
            break;
        }
        case uvCYLINDER:
        {
            // Cylinder projection.
            double x = localPointOfIntersection(0);
            double y = localPointOfIntersection(1);
            double z = localPointOfIntersection(2);
            double u = atan2(y, x) / M_PI;
            double v = -z;
            uvCoordinates(0) = u;
            uvCoordinates(1) = v;
            break;
        }
        case uvCUBOID:
        {
            // Box projection.
            double x = localPointOfIntersection(0);
            double y = localPointOfIntersection(1);
            double z = localPointOfIntersection(2);
            double u = 0.0;
            double v = 0.0;

            // Define default UV transform matrix.
            Matrix3d uvTransformation = Matrix3d::Identity(); ///////////// ?

            if (isWithinProximityPrecision(x, -1.0))
            {
                // Left face.
                u = -(y * 0.25) - 0.75;
                v = -z * 0.25;

            }
            else if (isWithinProximityPrecision(x, 1.0))
            {
                // Right face.
                u = (y * 0.25) + 0.25;
                v = -z * 0.25;

            }
            else if (isWithinProximityPrecision(y, -1.0))
            {
                // Front face.
                u = (x * 0.25) - 0.25;
                v = (-z * 0.25) - 0.5;

            }
            else if (isWithinProximityPrecision(y, 1.0))
            {
                // Back face.
                u = -(x * 0.25) - 0.25;;
                v = (-z * 0.25) + 0.5;

            }
            else if (isWithinProximityPrecision(z, -1.0))
            {
                // Top face.
                u = (x * 0.25) + 0.75;
                v = y * 0.25;

            }
            else if (isWithinProximityPrecision(z, 1.0))
            {
                // Bottom face.
                u = (x * 0.25) - 0.25;
                v = y * 0.25;

            }
            else
            {
                // The object isn't a box...

            }

            uvCoordinates(0) = u;
            uvCoordinates(1) = v;

            break;
        }
    }
}
