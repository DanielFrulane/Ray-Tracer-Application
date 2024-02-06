#include "Scene.hpp"
#include <iostream>

using namespace Eigen;

App::Scene::Scene() {
    m_camera.setPosition({0.0,-10.0,0.0});
    m_camera.setLookAt({0.0,0.0,0.0});
    m_camera.setUp({0.0,0.0,1.0});
    m_camera.setHorizontalSize(0.25);
    m_camera.setAspectRatio(16.0/9.0);
    m_camera.updateStatus();

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_lightsInScene.push_back(std::make_shared<LightSource>(LightPoint()));
    m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    m_lightsInScene.at(0) -> m_color << 255.0,255.0,255.0;
}

App::Scene::Scene(double aspectRatio) {
    m_camera.setPosition({0.0,-10.0,0.0});
    m_camera.setLookAt({0.0,0.0,0.0});
    m_camera.setUp({0.0,0.0,1.0});
    m_camera.setHorizontalSize(0.25);
    m_camera.setAspectRatio(aspectRatio);
    m_camera.updateStatus();

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_objectsInScene.at(0) -> m_color << 64.0, 128.0, 200.0;

    //m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    //m_objectsInScene.at(1) -> m_color << 255.0, 128.0, 0.0;

    //m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    //m_objectsInScene.at(2) -> m_color << 255.0, 200.0, 0.0;

    //GeometricalTransform testMatrix1, testMatrix2, testMatrix3;
    //testMatrix1.setTransformation({-1.5, 0.0, 0.0},{0.0, 0.0, 0.0},{0.5, 0.5, 0.75});
    //testMatrix2.setTransformation({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.75, 0.5, 0.5});
    //testMatrix3.setTransformation({1.5, 0.0, 0.0},{0.0, 0.0, 0.0},{0.75, 0.75, 0.75});

    //m_objectsInScene.at(0) -> setTransformation(testMatrix1);
    //m_objectsInScene.at(1) -> setTransformation(testMatrix2);
    //m_objectsInScene.at(2) -> setTransformation(testMatrix3);


    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    m_lightsInScene.at(0) -> m_color << 255.0,255.0,255.0;

    std::cout<<"Objects in scene: "<<m_objectsInScene.size()<<std::endl;
    std::cout<<"Lights in scene: "<<m_lightsInScene.size()<<std::endl;
}

bool App::Scene::render(RTImage &outputImage) {
    int height = outputImage.getHeightSize();
    int width = outputImage.getWidthSize();

    // for every pixel in image
    App::Ray cameraRay;
    Vector3d intersectionPoint;
    Vector3d localNormal;
    Vector3d localColor;
    double xFactor = 1.0 / (static_cast<double>(width)/2.0);
    double yFactor = 1.0 / (static_cast<double>(height)/2.0);
    double minimalDistance = 1e6;
    double maximalDistance = 0.0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            outputImage.setPixel(x, y, 200.0, 100.0, 0.0);

            double normX = (static_cast<double>(x) * xFactor) - 1.0;
            double normY = (static_cast<double>(y) * yFactor) - 1.0;

            m_camera.generateRay(normX, normY, cameraRay);

            for (std::shared_ptr<ObjectGeneric>& currentObject : m_objectsInScene) {
                bool isValidIntersection = currentObject -> isIntersecting(cameraRay, intersectionPoint, localNormal, localColor);

                if (isValidIntersection){
                    double intensity;
                    Vector3d color;
                    bool isValidLight = false;
                    for (std::shared_ptr<LightSource>& currentLight : m_lightsInScene) {
                        isValidLight = currentLight ->CalculateIllumination(intersectionPoint, localNormal, m_objectsInScene, currentObject, color, intensity);
                    }

                    double distanceToIntersection = (intersectionPoint - cameraRay.m_point1).norm();
                    if (distanceToIntersection > maximalDistance){
                        maximalDistance = distanceToIntersection;
                    }
                    if (distanceToIntersection < minimalDistance){
                        minimalDistance = distanceToIntersection;
                    }

                    if(isValidLight){
                        outputImage.setPixel(x, y, localColor(0) * intensity, localColor(1) * intensity, localColor(2) * intensity);
                        //outputImage.setPixel(x, y, 0.0, 0.0, 50.0);
                    } else{
                        //outputImage.setPixel(x, y, 50.0, 0.0, 0.0);
                    }
                } else{
                    //outputImage.setPixel(x, y, 0.0, 120.0, 0.0);
                }
            }

            //outputImage.setPixel(x, y, 0.0, 120.0, 120.0);
        }
    }

    std::cout<<"Maximal distance: "<<maximalDistance<<std::endl;
    std::cout<<"Minimal distance: "<<minimalDistance<<std::endl;

    return true;
}
