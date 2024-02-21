#include "SceneGeneric.hpp"
#include <iostream>

# define MINIMAL_DISTANCE 1e6

using namespace Eigen;

App::SceneGeneric::SceneGeneric() {
    //m_camera.setPosition({0.0,-10.0,-2.0});
    //m_camera.setLookAt({0.0,0.0,0.0});
    //m_camera.setUp({0.0,0.0,1.0});
    //m_camera.setHorizontalSize(0.25);
    //m_camera.setAspectRatio(16.0/9.0);
    //m_camera.updateStatus();

    //m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    //m_lightsInScene.push_back(std::make_shared<LightSource>(LightPoint()));
    //m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    //m_lightsInScene.at(0) -> m_color << 0.5,0.5,0.5;
}

App::SceneGeneric::SceneGeneric(double aspectRatio) {
    generateSceneObjects();
    /*m_camera.setPosition({0.0,-10.0,-2.0});
    m_camera.setLookAt({0.0,0.0,0.0});
    m_camera.setUp({0.0,0.0,1.0});
    m_camera.setHorizontalSize(0.75);
    m_camera.setAspectRatio(aspectRatio);
    m_camera.updateStatus();

    std::shared_ptr<MaterialSimple> floorMaterial = std::make_shared<MaterialSimple> (MaterialSimple());
    floorMaterial -> m_color = {1.0, 1.0, 1.0};
    floorMaterial -> m_reflectivity = 0.5;
    floorMaterial -> m_shininess = 0.0;
    std::shared_ptr<Textures::TextureChecker> floorTexture = std::make_shared<Textures::TextureChecker> (Textures::TextureChecker());
    floorTexture->setTransformation({0.0,0.0},M_PI/4,{16.0,16.0});
    floorMaterial->setTexture(floorTexture);


    std::shared_ptr<MaterialSimple> gradMat = std::make_shared<MaterialSimple> (MaterialSimple());
    gradMat -> m_color = {0.5, 0.3, 0.3}; // TODO ALL IN MATERIAL
    gradMat -> m_reflectivity = 0.05;
    gradMat -> m_shininess = 0.0;
    std::shared_ptr<Textures::TextureGradient> gradTexture = std::make_shared<Textures::TextureGradient> (Textures::TextureGradient());
    gradTexture -> setStop(0.0, {1.0, 0.75, 0.0});
    gradTexture -> setStop(0.5, {0.25, 0.25, 0.25});
    gradTexture -> setStop(1.0, {1.0, 0.0, 0.0});
    gradMat->setTexture(gradTexture);

    std::shared_ptr<MaterialSimple> whiteDiffuse = std::make_shared<MaterialSimple> (MaterialSimple());
    whiteDiffuse -> m_color = {0.8, 0.8, 0.8};
    whiteDiffuse -> m_reflectivity = 0.05;
    whiteDiffuse -> m_shininess = 5.0;

    std::shared_ptr<MaterialSimple> mirror = std::make_shared<MaterialSimple> (MaterialSimple());
    mirror -> m_color = {0.8, 0.8, 0.8};
    mirror -> m_reflectivity = 0.3;
    mirror -> m_shininess = 10.0;

    std::shared_ptr<MaterialRefractiveSimple> glass = std::make_shared<MaterialRefractiveSimple> (MaterialRefractiveSimple());
    glass -> m_color = {0.7, 0.7, 0.2};
    glass -> m_reflectivity = 0.3;
    glass -> m_shininess = 32.0;
    glass -> m_translucency = 0.75;
    glass -> m_indexOfRefraction = 1.333;

    m_objectsInScene.push_back(std::make_shared<ObjectPlane>(ObjectPlane()));
    m_objectsInScene.at(0) -> m_color << 0.5, 0.5, 0.5;
    GeometricalTransformation testMatrixPlane;
    testMatrixPlane.setTransformation({0.0, 0.0, 2.0},{0.0, 0.0, 0.0},{16.0, 16.0, 16.0});
    m_objectsInScene.at(0) -> setTransformation(testMatrixPlane);
    m_objectsInScene.at(0) ->setMaterial(floorMaterial);
    m_objectsInScene.at(0) ->m_uvMapType = uvPLANE; // TODO use?

    m_objectsInScene.push_back(std::make_shared<ObjectCylinder>(ObjectCylinder()));
    m_objectsInScene.at(1) -> m_color << 0.25, 0.5, 0.8;
    GeometricalTransformation testMatrix1;
    testMatrix1.setTransformation({0.0, 0.0, -3.0},{M_PI/4, M_PI/4, M_PI/4},{1.0, 1.0, 1.0});
    m_objectsInScene.at(1) -> setTransformation(testMatrix1);
    m_objectsInScene.at(1) ->setMaterial(whiteDiffuse);

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_objectsInScene.at(2) -> m_color << 0.25, 0.5, 0.8;
    GeometricalTransformation testMatrix2;
    testMatrix2.setTransformation({-2.0, -2.0, -2.0},{0.0, 0.0, 0.0},{2.0, 2.0, 1.0});
    m_objectsInScene.at(2) -> setTransformation(testMatrix2);
    m_objectsInScene.at(2) ->setMaterial(mirror);

    m_objectsInScene.push_back(std::make_shared<ObjectCuboid>(ObjectCuboid()));
    m_objectsInScene.at(3) -> m_color << 0.25, 0.5, 0.8;
    GeometricalTransformation testMatrix3;
    testMatrix3.setTransformation({0.0, -6.0, 0.0}, {M_PI/4, M_PI/4, M_PI/4}, {2.0, 1.5, 0.5});
    m_objectsInScene.at(3) -> setTransformation(testMatrix3);
    m_objectsInScene.at(3) ->setMaterial(gradMat);

    m_objectsInScene.push_back(std::make_shared<ObjectSphere>(ObjectSphere()));
    m_objectsInScene.at(4) -> m_color << 0.25, 0.5, 0.8;
    GeometricalTransformation testMatrix4;
    testMatrix4.setTransformation({2.0, -6.0, -2.0},{0.0, 0.0, 0.0},{1.0, 1.0, 1.0});
    m_objectsInScene.at(4) -> setTransformation(testMatrix4);
    m_objectsInScene.at(4) ->setMaterial(glass);*/

    /*std::shared_ptr<ObjectSphere> mySphere = std::make_shared<ObjectSphere>(ObjectSphere());
    mySphere->setMaterial(whiteDiffuse);
    std::shared_ptr<ObjectSphere> mySphere2 = std::make_shared<ObjectSphere>(ObjectSphere());
    mySphere2->setMaterial(whiteDiffuse);
    std::shared_ptr<ObjectComposition> composite = std::make_shared<ObjectComposition> (ObjectComposition());
    GeometricalTransformation testMatrix5, testMatrix6, testMatrix7;
    testMatrix5.setTransformation({-2.0, -2.0, -2.0},{0.0, 0.0, 0.0},{4.0, 4.0, 4.0});
    testMatrix6.setTransformation({0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.5, 0.5, 0.5});
    testMatrix7.setTransformation({5.0, 0.0, 0.0},{0.0, 0.0, 0.0},{1.0, 1.0, 1.0});
    composite -> setTransformation(testMatrix5);
    mySphere -> setTransformation(testMatrix6);
    mySphere2 -> setTransformation(testMatrix7);
    composite -> addObject(mySphere);
    composite -> addObject(mySphere2);
    m_objectsInScene.push_back(composite);*/

    /*m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    m_lightsInScene.at(0) -> m_color << 1.0,0.0,1.0;

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(1) -> m_location << -5.0,-10.0,-5.0;
    m_lightsInScene.at(1) -> m_color << 1.0,1.0,1.0;

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(2) -> m_location << 0.0,-10.0,-5.0;
    m_lightsInScene.at(2) -> m_color << 1.0,1.0,0.0;

    std::cout<<"Objects in scene: "<<m_objectsInScene.size()<<std::endl;
    //std::cout<<"Objects in composite: "<<composite->m_objects.size()<<std::endl;
    std::cout<<"Lights in scene: "<<m_lightsInScene.size()<<std::endl;*/
}

bool App::SceneGeneric::render(RTImage &outputImage) {
    int height = outputImage.getHeightSize();
    int width = outputImage.getWidthSize();

    // for every pixel in image
    App::Ray cameraRay;
    //double xFactor = 1.0 / (static_cast<double>(width)/2.0);
    //double yFactor = 1.0 / (static_cast<double>(height)/2.0);
    for (int x = 0; x < width; ++x) {
        if (x % 100 == 0){
            std::cout << "Processing line " << x << " of " << width << "." << std::endl;
        }
        for (int y = 0; y < height; ++y) {
            //double normX = (static_cast<double>(x) * xFactor) - 1.0;
            //double normY = (static_cast<double>(y) * yFactor) - 1.0;

            Vector3d pixelColor = renderPixel(x,y,width,height);
            outputImage.setPixel(x,y,pixelColor);
            //m_camera.generateRay(normX, normY, cameraRay);

            /*std::shared_ptr<ObjectGeneric> closestObject;
            //Vector3d closestIntersectionPoint;
            //Vector3d closestLocalNormal;
            //Vector3d closestLocalColor;
            HitInformation hitInformation;
            bool intersectionFound = castRay(cameraRay, closestObject, hitInformation);

            if (intersectionFound){
                if (hitInformation.hitObject->m_hasMaterial){
                    // color from material
                    MaterialGeneric::m_currentNumberOfReflections = 0;
                    Vector3d color = hitInformation.hitObject->m_material->calculateColor(m_objectsInScene, m_lightsInScene, hitInformation.hitObject, hitInformation.pointOfIntersection, hitInformation.normal, cameraRay);
                    outputImage.setPixel(x,y,color);
                } else {
                    // basic color calculation
                    Vector3d color = MaterialGeneric::calculateDiffuseColor(m_objectsInScene, m_lightsInScene, hitInformation.hitObject, hitInformation.pointOfIntersection, hitInformation.normal, closestObject->m_color);
                    outputImage.setPixel(x,y,color);
                }
            }*/
        }
    }
    return true;
}

bool App::SceneGeneric::castRay(App::Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject,
                                HitInformation &closestHitInformation) {
    //Vector3d intersectionPoint;
    //Vector3d localNormal;
    //Vector3d localColor;
    HitInformation hitInformation;
    double localMinimalDistance = MINIMAL_DISTANCE;
    bool intersectionFound = false;

    for (std::shared_ptr<ObjectGeneric> &currentObject: m_objectsInScene) {
        bool isValidIntersection = currentObject->isIntersecting(castedRay, hitInformation);
        if (isValidIntersection){
            intersectionFound = true;

            double distanceFromCameraToIntersection = (hitInformation.pointOfIntersection - castedRay.m_point1).norm();
            if (distanceFromCameraToIntersection < localMinimalDistance){
                localMinimalDistance = distanceFromCameraToIntersection;
                closestObject = currentObject;
                closestHitInformation = hitInformation;
            }
        }
    }
    return intersectionFound;
}

App::SceneGeneric::~SceneGeneric() {

}

void App::SceneGeneric::renderTile(App::TileInformation *tile) {
    Vector3d pixelColor;
    for (int y = 0; y < tile->height; ++y) {
        for (int x = 0; x < tile->width; ++x) {
            pixelColor = renderPixel(tile->x+x,tile->y+y,m_width,m_height);
            tile->rgb.at(convertCoordinatesToLinearIndex(x,y,tile->width,tile->height)) = pixelColor;
        }
    }
    tile->renderComplete = true;
}

void App::SceneGeneric::generateSceneObjects() {

}

Vector3d App::SceneGeneric::renderPixel(int x, int y, int width, int height) {

    std::shared_ptr<ObjectGeneric> closestObject;
    App::Ray cameraRay;
    double xFactor = 1.0 / (static_cast<double>(width)/2.0);
    double yFactor = 1.0 / (static_cast<double>(height)/2.0);
    double normX = (static_cast<double>(x) * xFactor) - 1.0;
    double normY = (static_cast<double>(y) * yFactor) - 1.0;

    m_camera.generateRay(normX, normY, cameraRay);

    HitInformation hitInformation;
    bool intersectionFound = castRay(cameraRay, closestObject, hitInformation);
    Vector3d color;
    if (intersectionFound){
        //if (hitInformation.hitObject->m_hasMaterial){
            // color from material
            MaterialGeneric::m_currentNumberOfReflections = 0;
            color = hitInformation.hitObject->m_material->calculateColor(m_objectsInScene, m_lightsInScene, hitInformation.hitObject, hitInformation.pointOfIntersection, hitInformation.normal, hitInformation.localPointOfIntersection, hitInformation.uvCoordinates, cameraRay);
        //} else {
            // basic color calculation
            //color = MaterialGeneric::calculateDiffuseColor(m_objectsInScene, m_lightsInScene, hitInformation.hitObject, hitInformation.pointOfIntersection, hitInformation.normal, closestObject->m_color);
        //}
    }else{
        color = {0.0,0.0,0.0};
    }
    return color;
}

int App::SceneGeneric::convertCoordinatesToLinearIndex(int x, int y, int width, int height) {
    if ((x<width) && (x>= 0) && (y<height) && (y >= 0)){
        return (y*width) + x;
    }
    return -1;
}

void App::SceneGeneric::checkIfHasAllNecessaryComponents() {
    if (!m_hasCamera){
        std::cout<<"camera not configured"<<std::endl;
        return;
    }
    if (m_lightsInScene.empty()){
        std::cout<<"no lights in scene"<<std::endl;
        return;
    }
    if (m_objectsInScene.empty()){
        std::cout<<"no objects in scene"<<std::endl;
        return;
    }
    for (const std::shared_ptr<ObjectGeneric>& currentObject : m_objectsInScene){
        if (currentObject->m_material == nullptr){
            std::cout<<"no material in object"<<std::endl;
        } else {
            if (currentObject->m_material->m_texture == nullptr){
                std::cout<<"no texture in material"<<std::endl;
            }
        }
    }
}
