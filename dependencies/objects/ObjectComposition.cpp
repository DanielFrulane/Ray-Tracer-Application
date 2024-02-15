#include "ObjectComposition.hpp"

#define ARBITRARY_HIGH_NUMBER 1e6

App::ObjectComposition::ObjectComposition() {
    m_xLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
    m_yLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
    m_zLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
}

App::ObjectComposition::~ObjectComposition() {

}

void App::ObjectComposition::addObject(std::shared_ptr<ObjectGeneric> subShape) {
    Vector2d xLim;
    Vector2d yLim;
    Vector2d zLim;
    subShape -> getExtents(xLim, yLim, zLim);
    if (xLim(0) < m_xLimit(0))
        m_xLimit(0) = xLim(0);
    if (xLim(1) > m_xLimit(1))
        m_xLimit(1) = xLim(1);

    if (yLim(0) < m_yLimit(0))
        m_yLimit(0) = yLim(0);
    if (yLim(1) > m_yLimit(1))
        m_yLimit(1) = yLim(1);

    if (zLim(0) < m_zLimit(0))
        m_zLimit(0) = zLim(0);
    if (zLim(1) > m_zLimit(1))
        m_zLimit(1) = zLim(1);

    // Modify the composite shape bounding box to fit the new extents.
    // Determine the size;
    double xSize = m_xLimit(1) - m_xLimit(0);
    double ySize = m_yLimit(1) - m_yLimit(0);
    double zSize = m_zLimit(1) - m_zLimit(0);

    // Determine centre points.
    double xCenter = m_xLimit(0) + (xSize / 2.0);
    double yCenter = m_yLimit(0) + (ySize / 2.0);
    double zCenter = m_zLimit(0) + (zSize / 2.0);

    // Update the transform matrix.
    m_boundingBoxTransformation.setTransformation({xCenter, yCenter, zCenter},
                                            {0.0, 0.0, 0.0},
                                            {xSize/2.0, ySize/2.0, zSize/2.0});

    // And modify the bounding box.
    m_boundingBox.setTransformation(m_boundingBoxTransformation);

    // Add the sub-shape to the list of sub-shapes.
    m_objects.push_back(subShape);
}

void App::ObjectComposition::getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) {
    // Construct corners of a cube based on the current limits.
    std::vector<Vector3d> cornerPoints = getCuboid(	m_xLimit(0), m_xLimit(1),
                                                    m_yLimit(0), m_yLimit(1),
                                                    m_zLimit(0), m_zLimit(1));

    // Apply the transforms to the unit cube corner points and compute limits.
    double minX = ARBITRARY_HIGH_NUMBER;
    double minY = ARBITRARY_HIGH_NUMBER;
    double minZ = ARBITRARY_HIGH_NUMBER;
    double maxX = -ARBITRARY_HIGH_NUMBER;
    double maxY = -ARBITRARY_HIGH_NUMBER;
    double maxZ = -ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<8; ++i)
    {
        cornerPoints.at(i) = m_transformation.applyTransformation(cornerPoints.at(i), FORWARD_TRANSFORMATION);

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

std::shared_ptr<App::ObjectGeneric> App::ObjectComposition::isIntersecting(const Ray &rayCasted,
                                            const Ray &backwardRay,
                                            Vector3d &intersectionPoint,
                                            double &currentDistance,
                                            Vector3d &localNormal, Vector3d &localColor){
                                            //qbRT::DATA::hitData &hitData) { // PRIVATE
    //isIntersecting(const Ray &rayCasted, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor);
    // Test for intersections with the sub-shapes.
    std::shared_ptr<App::ObjectGeneric> validObject = nullptr;
    //qbRT::DATA::hitData hitData;
    Vector3d localNormal2 = {0.0,0.0,0.0};
    Vector3d localColor2 = {0.0,0.0,0.0};
    for (std::shared_ptr<App::ObjectGeneric> currentObject : m_objects){
        if (currentObject -> m_isVisible){
            bool shapeTest = currentObject -> isIntersecting(backwardRay, intersectionPoint, localNormal2, localColor2); //// modificado
            if (shapeTest){
                // Transform the intersection point back into world coordinates.
                Vector3d intersectionPointLocal = m_transformation.applyTransformation(intersectionPoint, FORWARD_TRANSFORMATION); //// modificado

                // Compute the distance.
                double dist = (intersectionPointLocal - rayCasted.m_point1).norm();

                // If closest, then this is the shape to use.
                if (dist < currentDistance){
                    currentDistance = dist;
                    validObject = currentObject;
                    intersectionPoint = intersectionPointLocal;
                    localNormal = localNormal2;
                    localColor = localColor2;
                }
            }
        }
    }
    return validObject;
}

void App::ObjectComposition::updateBounds() {
// Set default initial values for the limits.
    m_xLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
    m_yLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
    m_zLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};

    Vector2d xLim;
    Vector2d yLim;
    Vector2d zLim;
    for (const std::shared_ptr<ObjectGeneric>& shape : m_objects){
        shape -> getExtents(xLim, yLim, zLim);
        if (xLim(0) < m_xLimit(0)){
            m_xLimit(0) = xLim(0);
        }
        if (xLim(1) > m_xLimit(1)){
            m_xLimit(1) = xLim(1);
        }

        if (yLim(0) < m_yLimit(0)){
            m_yLimit(0) = yLim(0);
        }
        if (yLim(1) > m_yLimit(1)){
            m_yLimit(1) = yLim(1);
        }

        if (zLim(0) < m_zLimit(0)){
            m_zLimit(0) = zLim(0);
        }
        if (zLim(1) > m_zLimit(1)){
            m_zLimit(1) = zLim(1);
        }
    }

    // Modify the composite shape bounding box to fit the new extents.
    // Determine the size;
    double xSize = m_xLimit(1) - m_xLimit(0);
    double ySize = m_yLimit(1) - m_yLimit(0);
    double zSize = m_zLimit(1) - m_zLimit(0);

    // Determine centre points.
    double xCenter = m_xLimit(0) + (xSize / 2.0);
    double yCenter = m_yLimit(0) + (ySize / 2.0);
    double zCenter = m_zLimit(0) + (zSize / 2.0);

    // Update the transform matrix.
    m_transformation.setTransformation({xCenter, yCenter, zCenter},
                                       {0.0, 0.0, 0.0},
                                       {xSize/2.0, ySize/2.0, zSize/2.0});

    // And modify the bounding box.
    m_boundingBox.setTransformation(m_boundingBoxTransformation);
}

bool App::ObjectComposition::isIntersecting(const App::Ray &castRay, Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) { ////OVERRIDE modificado
    // Check if the object is visible.
    if (!m_isVisible){
        return false;
    }

    // Copy the ray and apply the backwards transform.
    Ray backwardRay = m_transformation.applyTransformation(castRay, BACKWARD_TRANSFORMATION);

    // Check for intersection with the bounding box.
    if (!m_isUsingBoundingBox || m_boundingBox.isIntersecting(backwardRay)){
        // We intersected with the bounding box, so check everything else.
        Vector3d worldIntPoint;
        double currentDist = 100e6;
        //qbRT::DATA::hitData tempHitData;

        Vector3d pointOfIntersection2;
        Vector3d normal2;
        Vector3d color2;
        Vector3d localPointOfIntersection2;
        Vector3d uvCoordinates2;
        std::shared_ptr<ObjectGeneric> hitObject2;

        //std::shared_ptr<App::ObjectGeneric> validObject = TestIntersections(castRay, backwardRay, worldIntPoint, currentDist, tempHitData);
        std::shared_ptr<App::ObjectGeneric> validObject = isIntersecting(castRay, backwardRay, worldIntPoint, currentDist, localNormal, localColor);
        if (validObject != nullptr){

            // TODO apply normal?
            Vector3d originOfObject(0.0,0.0,0.0);
            Vector3d  newOriginOfObject = m_transformation.applyTransformation(originOfObject, FORWARD_TRANSFORMATION);
            Vector3d  localNormal2 = worldIntPoint - newOriginOfObject;
            localNormal.normalize();

            //validObject -> (tempHitData.localPOI, hitData.uvCoords);
            m_uvCoordinates = validObject->m_uvCoordinates;//TODO REMOVE W THREADS

            // Return these values.
            intersectionPoint = worldIntPoint;
            localNormal = localNormal2;
            localColor = color2;

            return true;
        }
        else
        {
            // No intersections with internal sub-shapes.
            return false;
        }
    }
    else
    {
        // No intersection with the bounding box.
        return false;
    }

    return false;
}
