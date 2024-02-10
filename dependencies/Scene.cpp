#include "Scene.hpp"
#include <iostream>

# define MINIMAL_DISTANCE 1e6

using namespace Eigen;

App::Scene::Scene() {
    m_camera.setPosition({0.0,-10.0,-1.0});
    m_camera.setLookAt({0.0,0.0,0.0});
    m_camera.setUp({0.0,0.0,1.0});
    m_camera.setHorizontalSize(0.25);
    m_camera.setAspectRatio(16.0/9.0);
    m_camera.updateStatus();

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_lightsInScene.push_back(std::make_shared<LightSource>(LightPoint()));
    m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    m_lightsInScene.at(0) -> m_color << 0.5,0.5,0.5;
}

App::Scene::Scene(double aspectRatio) {
    m_camera.setPosition({0.0,-10.0,-1.0});
    m_camera.setLookAt({0.0,0.0,0.0});
    m_camera.setUp({0.0,0.0,1.0});
    m_camera.setHorizontalSize(0.25);
    m_camera.setAspectRatio(aspectRatio);
    m_camera.updateStatus();

    std::shared_ptr<MaterialSimple> testMaterial1 = std::make_shared<MaterialSimple> (MaterialSimple());
    std::shared_ptr<MaterialSimple> testMaterial2 = std::make_shared<MaterialSimple> (MaterialSimple());
    std::shared_ptr<MaterialSimple> testMaterial3 = std::make_shared<MaterialSimple> (MaterialSimple());
    std::shared_ptr<MaterialSimple> floorMaterial = std::make_shared<MaterialSimple> (MaterialSimple());

    // Setup the materials.
    testMaterial1 -> m_color = {0.25, 0.5, 0.8};
    testMaterial1 -> m_reflectivity = 0.1;
    testMaterial1 -> m_shininess = 10.0;

    testMaterial2 -> m_color = {1.0, 0.5, 0.0};
    testMaterial2 -> m_reflectivity = 0.75;
    testMaterial2 -> m_shininess = 10.0;

    testMaterial3 -> m_color = {1.0, 0.8, 0.0};
    testMaterial3 -> m_reflectivity = 0.25;
    testMaterial3 -> m_shininess = 10.0;

    floorMaterial -> m_color = {1.0, 1.0, 1.0};
    floorMaterial -> m_reflectivity = 0.5;
    floorMaterial -> m_shininess = 0.0;

    GeometricalTransform testMatrix1, testMatrix2, testMatrix3, testMatrixPlane;

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_objectsInScene.at(0) -> m_color << 0.25, 0.5, 0.8;
    testMatrix1.setTransformation({-1.5, 0.0, 0.0},{0.0, 0.0, 0.0},{0.5, 0.5, 0.5});
    m_objectsInScene.at(0) -> setTransformation(testMatrix1);

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_objectsInScene.at(1) -> m_color << 1.0, 0.5, 0.0;
    testMatrix2.setTransformation({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.5, 0.5, 0.5});
    m_objectsInScene.at(1) -> setTransformation(testMatrix2);

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_objectsInScene.at(2) -> m_color << 1.0, 0.8, 0.0;
    testMatrix3.setTransformation({1.5, 0.0, 0.0},{0.0, 0.0, 0.0},{0.5, 0.5, 0.5});
    m_objectsInScene.at(2) -> setTransformation(testMatrix3);

    m_objectsInScene.push_back(std::make_shared<ObjectPlane>(ObjectPlane()));
    m_objectsInScene.at(3) -> m_color << 0.5, 0.5, 0.5;
    testMatrixPlane.setTransformation({0.0, 0.0, 0.75},{0.0, 0.0, 0.0},{4.0, 4.0, 1.0});
    m_objectsInScene.at(3) -> setTransformation(testMatrixPlane);

    m_objectsInScene.at(0) ->setMaterial(testMaterial1);
    m_objectsInScene.at(1) ->setMaterial(testMaterial2);
    m_objectsInScene.at(2) ->setMaterial(testMaterial3);
    m_objectsInScene.at(3) ->setMaterial(floorMaterial);


    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    m_lightsInScene.at(0) -> m_color << 1.0,0.0,0.0;

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(1) -> m_location << -5.0,-10.0,-5.0;
    m_lightsInScene.at(1) -> m_color << 0.0,1.0,0.0;

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(2) -> m_location << 0.0,-10.0,-5.0;
    m_lightsInScene.at(2) -> m_color << 0.0,0.0,1.0;

    std::cout<<"Objects in scene: "<<m_objectsInScene.size()<<std::endl;
    std::cout<<"Lights in scene: "<<m_lightsInScene.size()<<std::endl;
}

bool App::Scene::render(RTImage &outputImage) {
    int height = outputImage.getHeightSize();
    int width = outputImage.getWidthSize();

    // for every pixel in image
    App::Ray cameraRay;
    double xFactor = 1.0 / (static_cast<double>(width)/2.0);
    double yFactor = 1.0 / (static_cast<double>(height)/2.0);
    for (int x = 0; x < width; ++x) {
        if (x % 100 == 0){
            std::cout << "Processing line " << x << " of " << width << "." << std::endl;
        }
        for (int y = 0; y < height; ++y) {
            double normX = (static_cast<double>(x) * xFactor) - 1.0;
            double normY = (static_cast<double>(y) * yFactor) - 1.0;

            m_camera.generateRay(normX, normY, cameraRay);

            std::shared_ptr<ObjectGeneric> closestObject;
            Vector3d closestIntersectionPoint;
            Vector3d closestLocalNormal;
            Vector3d closestLocalColor;
            bool intersectionFound = castRay(cameraRay, closestObject, closestIntersectionPoint, closestLocalNormal, closestLocalColor);

            if (intersectionFound){
                if (closestObject->m_hasMaterial){
                    // color from material
                    MaterialGeneric::m_currentNumberOfReflections = 0;
                    Vector3d color = closestObject->m_material->calculateColor(m_objectsInScene, m_lightsInScene, closestObject, closestIntersectionPoint, closestLocalNormal, cameraRay);
                    outputImage.setPixel(x,y,color);
                } else {
                    // basic color calculation
                    Vector3d color = MaterialGeneric::calculateDiffuseColor(m_objectsInScene, m_lightsInScene, closestObject, closestIntersectionPoint, closestLocalNormal, closestObject->m_color);
                    outputImage.setPixel(x,y,color);
                }
            }
        }
    }
    return true;
}

bool App::Scene::castRay(App::Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject,
                         Vector3d &closestIntersectionPoint, Vector3d &closestLocalNormal,
                         Vector3d &closestLocalColor) {
    Vector3d intersectionPoint;
    Vector3d localNormal;
    Vector3d localColor;
    double localMinimalDistance = MINIMAL_DISTANCE;
    bool intersectionFound = false;

    for (std::shared_ptr<ObjectGeneric> &currentObject: m_objectsInScene) {
        bool isValidIntersection = currentObject->isIntersecting(castedRay, intersectionPoint, localNormal, localColor);
        if (isValidIntersection){
            intersectionFound = true;

            double distanceFromCameraToIntersection = (intersectionPoint - castedRay.m_point1).norm();
            if (distanceFromCameraToIntersection < localMinimalDistance){
                localMinimalDistance = distanceFromCameraToIntersection;
                closestObject = currentObject;
                closestIntersectionPoint = intersectionPoint;
                closestLocalNormal = localNormal;
                closestLocalColor = localColor;
            }
        }
    }
    return intersectionFound;
}