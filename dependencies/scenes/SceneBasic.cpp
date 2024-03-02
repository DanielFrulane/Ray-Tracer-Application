#include "SceneBasic.hpp"

// this is an example version for generating a scene and is not used for the final application

App::SceneBasic::SceneBasic() {
    generateSceneObjects();
    checkIfHasAllNecessaryComponents();
    std::cout<<"generated objects"<<std::endl;
}

App::SceneBasic::~SceneBasic() = default;

void App::SceneBasic::generateSceneObjects() {
    // camera setup
    m_hasCamera = true;
    m_camera.setPosition({0.0,-10.0,-2.0});
    m_camera.setLookAt({0.0,0.0,0.0});
    m_camera.setUp({0.5,0.0,1.0});
    m_camera.setHorizontalSize(0.75);
    m_camera.setAspectRatio(1.0);
    m_camera.updateStatus();

    // materials
    std::shared_ptr<MaterialCompleteSimple> floorMaterial = std::make_shared<MaterialCompleteSimple> (MaterialCompleteSimple());
    floorMaterial -> m_reflectivity = 0.5;
    floorMaterial -> m_shininess = 0.0;
    floorMaterial -> m_translucency = 0.0;
    std::shared_ptr<Textures::TextureChecker> floorTexture = std::make_shared<Textures::TextureChecker> (Textures::TextureChecker());
    floorTexture->setTransformation({0.0,0.0},1.0/8.0,{16.0,16.0});
    floorMaterial->setTexture(floorTexture);

    std::shared_ptr<MaterialCompleteSimple> gradientMaterial = std::make_shared<MaterialCompleteSimple> (MaterialCompleteSimple());
    gradientMaterial -> m_reflectivity = 0.05;
    gradientMaterial -> m_shininess = 0.0;
    gradientMaterial -> m_translucency = 0.0;
    std::shared_ptr<Textures::TextureGradient> gradientTexture = std::make_shared<Textures::TextureGradient> (Textures::TextureGradient());
    gradientTexture -> setStop(0.0, {1.0, 0.75, 0.0});
    gradientTexture -> setStop(0.5, {0.25, 0.25, 0.25});
    gradientTexture -> setStop(1.0, {1.0, 0.0, 0.0});
    gradientMaterial->setTexture(gradientTexture);

    std::shared_ptr<MaterialCompleteSimple> whiteMaterial = std::make_shared<MaterialCompleteSimple> (MaterialCompleteSimple());
    whiteMaterial -> m_reflectivity = 0.05;
    whiteMaterial -> m_shininess = 0.0;
    whiteMaterial -> m_translucency = 0.0;
    std::shared_ptr<Textures::TextureFlat> whiteTexture = std::make_shared<Textures::TextureFlat> (Textures::TextureFlat());
    whiteTexture ->setColor({0.8, 0.8, 0.8});
    whiteMaterial->setTexture(whiteTexture);

    std::shared_ptr<MaterialCompleteSimple> mirror = std::make_shared<MaterialCompleteSimple> (MaterialCompleteSimple());
    mirror -> m_reflectivity = 0.3;
    mirror -> m_shininess = 10.0;
    mirror -> m_translucency = 0.0;
    std::shared_ptr<Textures::TextureFlat> mirrorTexture = std::make_shared<Textures::TextureFlat> (Textures::TextureFlat());
    mirrorTexture ->setColor({0.8, 0.8, 0.8});
    mirror->setTexture(mirrorTexture);

    std::shared_ptr<MaterialCompleteSimple> glass = std::make_shared<MaterialCompleteSimple> (MaterialCompleteSimple());
    glass -> m_reflectivity = 0.3;
    glass -> m_shininess = 32.0;
    glass -> m_translucency = 0.75;
    glass -> m_indexOfRefraction = 1.333;
    std::shared_ptr<Textures::TextureFlat> glassTexture = std::make_shared<Textures::TextureFlat> (Textures::TextureFlat());
    glassTexture ->setColor({0.9, 0.9, 0.5});
    glass->setTexture(glassTexture);

    std::shared_ptr<MaterialCompleteSimple> flatMaterial = std::make_shared<MaterialCompleteSimple> (MaterialCompleteSimple());
    glass -> m_reflectivity = 0.3;
    glass -> m_shininess = 32.0;
    glass -> m_translucency = 0.75;
    glass -> m_indexOfRefraction = 1.333;
    std::shared_ptr<Textures::TextureFlat> flatTexture = std::make_shared<Textures::TextureFlat> (Textures::TextureFlat());
    flatTexture ->setColor({1.0,0.0,0.0});
    flatMaterial->setTexture(flatTexture);

    // objects
    std::shared_ptr<ObjectGeneric> myPlane = std::make_shared<ObjectPlane>(ObjectPlane());
    GeometricalTransformation testMatrixPlane;
    testMatrixPlane.setTransformation({0.0, 0.0, 2.0},{0.0, 0.0, 0.0},{16.0, 16.0, 16.0});
    myPlane-> setTransformation(testMatrixPlane);
    myPlane->setMaterial(floorMaterial);
    m_objectsInScene.push_back(myPlane);

    std::shared_ptr<ObjectGeneric> myCylinder = std::make_shared<ObjectCylinder>(ObjectCylinder());
    GeometricalTransformation testMatrix1;
    testMatrix1.setTransformation({0.0, 0.0, -3.0},{1.0/8.0, 1.0/8.0, 1.0/8.0},{1.0, 1.0, 1.0});
    myCylinder-> setTransformation(testMatrix1);
    myCylinder->setMaterial(whiteMaterial);
    m_objectsInScene.push_back(myCylinder);

    std::shared_ptr<ObjectGeneric> myEgg = std::make_shared<ObjectSphere>(ObjectSphere());
    GeometricalTransformation testMatrix2;
    testMatrix2.setTransformation({-2.0, -2.0, -2.0},{0.0, 0.0, 0.0},{2.0, 2.0, 1.0});
    myEgg-> setTransformation(testMatrix2);
    myEgg->setMaterial(mirror);
    m_objectsInScene.push_back(myEgg);

    std::shared_ptr<ObjectGeneric> myCuboid = std::make_shared<ObjectCuboid>(ObjectCuboid());
    GeometricalTransformation testMatrix3;
    testMatrix3.setTransformation({0.0, -6.0, 0.0}, {1.0/8.0, 1.0/8.0, 1.0/8.0}, {2.0, 1.5, 0.5});
    myCuboid-> setTransformation(testMatrix3);
    myCuboid->setMaterial(gradientMaterial);
    m_objectsInScene.push_back(myCuboid);

    std::shared_ptr<ObjectGeneric> myGlass = std::make_shared<ObjectSphere>(ObjectSphere());
    GeometricalTransformation testMatrix4;
    testMatrix4.setTransformation({2.0, -6.0, -2.0},{0.0, 0.0, 0.0},{1.5, 1.5, 1.5});
    myGlass-> setTransformation(testMatrix4);
    myGlass->setMaterial(glass);
    m_objectsInScene.push_back(myGlass);

    std::shared_ptr<ObjectGeneric> myCone = std::make_shared<ObjectCone>(ObjectCone());
    GeometricalTransformation testMatrix9;
    testMatrix9.setTransformation({-2.0, -5.0, 0.0},{0.0, 0.0, 0.0},{1.0, 1.0, 2.0});
    myCone-> setTransformation(testMatrix9);
    myCone->setMaterial(flatMaterial);
    m_objectsInScene.push_back(myCone);

    // lights
    std::shared_ptr<LightPoint> light1 = std::make_shared<LightPoint>(LightPoint());
    light1->m_position << 5.0,-10.0,-5.0;
    light1->m_color << 1.0,0.4,0.4;
    m_lightsInScene.push_back(light1);

    std::shared_ptr<LightPoint> light2 = std::make_shared<LightPoint>(LightPoint());
    light2->m_position << -5.0,-10.0,-5.0;
    light2->m_color << 0.4,1.0,0.4;
    m_lightsInScene.push_back(light2);

    std::shared_ptr<LightPoint> light3 = std::make_shared<LightPoint>(LightPoint());
    light3->m_position << 0.0,-10.0,-5.0;
    light3->m_color << 0.4,0.4,1.0;
    m_lightsInScene.push_back(light3);

    std::cout<<"Objects in scene: "<<m_objectsInScene.size()<<std::endl;
    std::cout<<"Lights in scene: "<<m_lightsInScene.size()<<std::endl;
}
