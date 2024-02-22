#include "ObjectComposition.hpp"

#define ARBITRARY_HIGH_NUMBER 1e6

App::ObjectComposition::ObjectComposition() {
    m_boundingBox = ObjectCuboid();
    m_isUsingBoundingBox = true;
    m_xLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
    m_yLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
    m_zLimit = {ARBITRARY_HIGH_NUMBER, -ARBITRARY_HIGH_NUMBER};
}

App::ObjectComposition::~ObjectComposition() = default;

// adds an object to own list of objects
void App::ObjectComposition::addObject(const std::shared_ptr<ObjectGeneric>& objectToAdd) {
    Vector2d xLim = {0.0,0.0};
    Vector2d yLim = {0.0,0.0};
    Vector2d zLim = {0.0,0.0};
    // gets its extents
    objectToAdd -> getExtents(xLim, yLim, zLim);
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

    // reshape the bounding box
    double xSize = m_xLimit(1) - m_xLimit(0);
    double ySize = m_yLimit(1) - m_yLimit(0);
    double zSize = m_zLimit(1) - m_zLimit(0);
    // centralize
    double xCenter = m_xLimit(0) + (xSize / 2.0);
    double yCenter = m_yLimit(0) + (ySize / 2.0);
    double zCenter = m_zLimit(0) + (zSize / 2.0);

    // updates transformation
    m_boundingBoxTransformation.setTransformation({xCenter, yCenter, zCenter},
                                            {0.0, 0.0, 0.0},
                                            {xSize/2.0, ySize/2.0, zSize/2.0});
    m_boundingBox.setTransformation(m_boundingBoxTransformation);

    m_objects.push_back(objectToAdd);
}

// gets extents of the bounding box
void App::ObjectComposition::getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) {
    // gets base cuboid
    std::vector<Vector3d> cornerPoints = getCuboid(	m_xLimit(0), m_xLimit(1),
                                                    m_yLimit(0), m_yLimit(1),
                                                    m_zLimit(0), m_zLimit(1));

    // gets new dimensions for the cuboid
    double minX = ARBITRARY_HIGH_NUMBER;
    double minY = ARBITRARY_HIGH_NUMBER;
    double minZ = ARBITRARY_HIGH_NUMBER;
    double maxX = -ARBITRARY_HIGH_NUMBER;
    double maxY = -ARBITRARY_HIGH_NUMBER;
    double maxZ = -ARBITRARY_HIGH_NUMBER;
    for (int i=0; i<8; ++i){
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

// tests intersections with objects within the bounding box
std::shared_ptr<App::ObjectGeneric> App::ObjectComposition::isIntersecting(const Ray &rayCasted,
                                            const Ray &backwardRay,
                                            Vector3d &worldIntersectionPoint,
                                            double &currentDistance, HitInformation &hitInformationTemporary){
    std::shared_ptr<App::ObjectGeneric> returnObject = nullptr;
    HitInformation hitInformation;
    for (const std::shared_ptr<App::ObjectGeneric>& currentObject : m_objects){ // for all their objects
        bool objectTest = currentObject -> isIntersecting(backwardRay, hitInformation);
        if (objectTest){
            // gets world coordinates for intersection
            Vector3d intersectionPointLocal = m_transformation.applyTransformation(hitInformation.pointOfIntersection, FORWARD_TRANSFORMATION);
            double distance = (intersectionPointLocal - rayCasted.m_point1).norm();
            // use the closest object
            if (distance < currentDistance){
                currentDistance = distance;
                returnObject = currentObject;
                worldIntersectionPoint = intersectionPointLocal;
                hitInformationTemporary = hitInformation;
            }
        }
    }
    return returnObject;
}

// tests intersection with the bounding box
bool App::ObjectComposition::isIntersecting(const App::Ray &castRay, HitInformation &hitInformation) {
    Ray backwardRay = m_transformation.applyTransformation(castRay, BACKWARD_TRANSFORMATION);

    if (!m_isUsingBoundingBox || m_boundingBox.isIntersecting(backwardRay)){ // is intersecting bounding box
        Vector3d worldIntPoint = {0.0,0.0,0.0};
        double currentDistance = ARBITRARY_HIGH_NUMBER;
        HitInformation hitInformationTemporary;
        std::shared_ptr<App::ObjectGeneric> currentObject = isIntersecting(castRay, backwardRay, worldIntPoint, currentDistance, hitInformationTemporary);

        if (currentObject != nullptr){ // is intersecting object
            Vector3d newNormal = m_transformation.applyNorm(hitInformationTemporary.normal);
            newNormal.normalize();
            // TODO check bug threaded UV coordinates
            hitInformationTemporary.hitObject-> calculateUVSpace(hitInformationTemporary.localPointOfIntersection, hitInformation.uvCoordinates);
            hitInformation.uvCoordinates = hitInformationTemporary.uvCoordinates;
            // sets what will be returned
            hitInformation.pointOfIntersection = worldIntPoint;
            hitInformation.normal = newNormal;
            hitInformation.color = hitInformationTemporary.color;
            hitInformation.localPointOfIntersection = hitInformationTemporary.localPointOfIntersection;
            hitInformation.hitObject = hitInformationTemporary.hitObject;

            return true;
        }else{
            return false; // no intersections with own objects
        }
    } else {
        return false; // no intersection with the bounding box
    }
}
