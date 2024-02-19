#include "SceneBasic.hpp"

#define aspectRatio 1.0 /// TODO REFACTOR

App::SceneBasic::SceneBasic() {
    generateSceneObjects();
    std::cout<<"generated objects"<<std::endl;
}

App::SceneBasic::~SceneBasic() {

}

void App::SceneBasic::generateSceneObjects() {
    m_camera.setPosition({0.0,-10.0,-2.0});
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
    testMatrix4.setTransformation({2.0, -6.0, -2.0},{0.0, 0.0, 0.0},{1.5, 1.5, 1.5});
    m_objectsInScene.at(4) -> setTransformation(testMatrix4);
    m_objectsInScene.at(4) ->setMaterial(glass);

    m_objectsInScene.push_back(std::make_shared<ObjectCone>(ObjectCone()));
    m_objectsInScene.at(5) -> m_color << 1.0, 0.3, 0.3;
    GeometricalTransformation testMatrix9;
    testMatrix9.setTransformation({-2.0, -5.0, 0.0},{0.0, 0.0, 0.0},{1.0, 1.0, 2.0});
    m_objectsInScene.at(5) -> setTransformation(testMatrix9);

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

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(0) -> m_location << 5.0,-10.0,-5.0;
    m_lightsInScene.at(0) -> m_color << 1.0,0.4,0.4;

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(1) -> m_location << -5.0,-10.0,-5.0;
    m_lightsInScene.at(1) -> m_color << 0.4,1.0,0.4;

    m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));
    m_lightsInScene.at(2) -> m_location << 0.0,-10.0,-5.0;
    m_lightsInScene.at(2) -> m_color << 0.4,0.4,1.0;

    std::cout<<"Objects in scene: "<<m_objectsInScene.size()<<std::endl;
    //std::cout<<"Objects in composite: "<<composite->m_objects.size()<<std::endl;
    std::cout<<"Lights in scene: "<<m_lightsInScene.size()<<std::endl;
}
