#include "SceneFromJson.hpp"
#include <iostream>

#define VALUE_TYPE "type"
#define VALUE_TYPE_OBJECT_CAMERA "camera"
#define VALUE_TYPE_OBJECT_LIGHT "light"
#define VALUE_TYPE_OBJECT_SPHERE "sphere"
#define VALUE_TYPE_OBJECT_PLANE "plane"
#define VALUE_TYPE_OBJECT_CYLINDER "cylinder"
#define VALUE_TYPE_OBJECT_CONE "cone"
#define VALUE_TYPE_OBJECT_CUBOID "cuboid"
#define VALUE_TYPE_OBJECT_COMPOSITION "composition"
#define VALUE_COMPOSITION_OBJECTS "objectList"
#define VALUE_POSITION "position"
#define VALUE_LOOK_AT "lookAt"
#define VALUE_UP "up"
#define VALUE_COLOR "color"
#define VALUE_COLOR1 "color1"
#define VALUE_COLOR2 "color2"
#define VALUE_ZOOM_OUT "zoomOut"
#define VALUE_ASPECT_RATIO "aspectRatio"
#define VALUE_TRANSFORMATION "transformation"
#define VALUE_TRANSFORMATION_UV "transformationUV"
#define VALUE_TRANSLATION "translation"
#define VALUE_ROTATION "rotation"
#define VALUE_SCALE "scale"
#define VALUE_MATERIAL "material"
#define VALUE_REFLECTIVITY "reflectivity"
#define VALUE_SHININESS "shininess"
#define VALUE_TRANSLUCENCY "translucency"
#define VALUE_INDEX_OF_REFRACTION "indexOfRefraction"
#define VALUE_TEXTURE "texture"
#define VALUE_TYPE_TEXTURE_FLAT "flat"
#define VALUE_TYPE_TEXTURE_CHECKER "checker"
#define VALUE_TYPE_TEXTURE_GRADIENT "gradient"
#define VALUE_TEXTURE_GRADIENT_STOPS "stops"

App::SceneFromJSON::SceneFromJSON() {
    generateSceneObjects();
    checkIfHasAllNecessaryComponents();
    std::cout<<"configured objects: "<<m_objectsInScene.size()<<std::endl;
    std::cout<<"configured lights: "<<m_lightsInScene.size()<<std::endl;
}

App::SceneFromJSON::~SceneFromJSON() = default;

// reads all .json files to generate objects
void App::SceneFromJSON::generateSceneObjects() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentPath)) {
        if (std::filesystem::is_regular_file(entry)) {
            if (entry.path().extension() == ".json") {
                std::cout << "Reading file: " << entry.path().filename() << std::endl;
                interpretFile(entry.path().filename().string().c_str());
            }
        }
    }
}

void App::SceneFromJSON::interpretFile(const char* fileName) {
    // creates file reader
    FILE* fp = fopen(fileName, "r");
    char readBuffer[50000];
    rapidjson::FileReadStream is(fp, readBuffer,sizeof(readBuffer));

    // parse the .json data using a Document object
    rapidjson::Document d;
    d.ParseStream(is);

    fclose(fp);
    interpretJSONType(d);
}

// interprets what type of object the file refers to
void App::SceneFromJSON::interpretJSONType(const rapidjson::Value& d) {
    const char* comparisonString(d[VALUE_TYPE].GetString());
    std::cout<<"type: "<<comparisonString<<std::endl;

    if (strcmp(comparisonString, VALUE_TYPE_OBJECT_CAMERA) == 0) {
        interpretJSONCamera(d);
    } else if (strcmp(comparisonString, VALUE_TYPE_OBJECT_LIGHT) == 0) {
        interpretJSONLight(d);
    } else if (strcmp(comparisonString, VALUE_TYPE_OBJECT_COMPOSITION) == 0) {
        std::shared_ptr<ObjectComposition> myComposition = std::make_shared<ObjectComposition> (ObjectComposition());
        const rapidjson::Value& transformationValue = d[VALUE_TRANSFORMATION];
        GeometricalTransformation myTransformation  = getTransformationFromJSON(transformationValue);
        myComposition->setTransformation(myTransformation);
        const rapidjson::Value& objectsValue = d[VALUE_COMPOSITION_OBJECTS];
        for (rapidjson::Value const& objectValue: objectsValue.GetArray()){
            std::cout<<"adding object"<<objectValue[VALUE_TYPE].GetString()<<std::endl;
            std::shared_ptr<ObjectGeneric> myObject = getConcreteJSONObject(objectValue);
            myComposition->addObject(myObject);
        }
        m_objectsInScene.push_back(myComposition);
    } else {
        std::shared_ptr<ObjectGeneric> myObject = getConcreteJSONObject(d);
        if (myObject != nullptr){
            m_objectsInScene.push_back(myObject);
        }
    }
}

// interprets a camera file
void App::SceneFromJSON::interpretJSONCamera(const rapidjson::Value& d) {
    if (m_hasCamera){
        throw std::invalid_argument("scene tried to configure more than one camera");
    }
    const rapidjson::Value& positionValue = d[VALUE_POSITION];
    const rapidjson::Value& lookAtValue = d[VALUE_LOOK_AT];
    const rapidjson::Value& upValue = d[VALUE_UP];
    const rapidjson::Value& horizontalSizeValue = d[VALUE_ZOOM_OUT];
    const rapidjson::Value& aspectRatioValue = d[VALUE_ASPECT_RATIO];
    Vector3d position = getXYZVector3dFromValue(positionValue);
    Vector3d lookAt = getXYZVector3dFromValue(lookAtValue);
    Vector3d up = getXYZVector3dFromValue(upValue);
    double horizontalSize = horizontalSizeValue.GetDouble();
    double aspectRatio = aspectRatioValue.GetDouble();

    m_camera.setPosition(position);
    m_camera.setLookAt(lookAt);
    m_camera.setUp(up);
    m_camera.setHorizontalSize(horizontalSize);
    m_camera.setAspectRatio(aspectRatio);
    m_camera.updateStatus();

    m_hasCamera = true;
}

// interprets a light file
void App::SceneFromJSON::interpretJSONLight(const rapidjson::Value& d) {
    const rapidjson::Value& positionValue = d[VALUE_POSITION];
    const rapidjson::Value& colorValue = d[VALUE_COLOR];
    Vector3d position = getXYZVector3dFromValue(positionValue);
    Vector3d color = getRGBVector3dFromValue(colorValue);

    std::shared_ptr<LightPoint> myLight = std::make_shared<LightPoint>(LightPoint());
    myLight -> m_position = position;
    myLight -> m_color = color;
    m_lightsInScene.push_back(myLight);
}

// interprets a Vector3d formatted as xyz
Vector3d App::SceneFromJSON::getXYZVector3dFromValue(const rapidjson::Value &value) {
    double x, y, z;
    x = value["x"].GetDouble();
    y = value["y"].GetDouble();
    z = value["z"].GetDouble();
    return {x,y,z};
}

// interprets a Vector3d formatted as rgb
Vector3d App::SceneFromJSON::getRGBVector3dFromValue(const rapidjson::Value &value) {
    double x, y, z;
    x = value["red"].GetDouble();
    y = value["green"].GetDouble();
    z = value["blue"].GetDouble();
    return {x,y,z};
}

// interprets a Vector2d formatted as xy
Vector2d App::SceneFromJSON::getXYVector2dFromValue(const rapidjson::Value &value) {
    double x, y;
    x = value["x"].GetDouble();
    y = value["y"].GetDouble();
    return {x,y};
}

// interprets a transformation value
App::GeometricalTransformation App::SceneFromJSON::getTransformationFromJSON(const rapidjson::Value &value) {
    const rapidjson::Value& translationValue = value[VALUE_TRANSLATION];
    const rapidjson::Value& rotationValue = value[VALUE_ROTATION];
    const rapidjson::Value& scaleValue = value[VALUE_SCALE];
    Vector3d translation = getXYZVector3dFromValue(translationValue);
    Vector3d rotation = getXYZVector3dFromValue(rotationValue);
    Vector3d scale = getXYZVector3dFromValue(scaleValue);
    GeometricalTransformation myTransformation;
    myTransformation.setTransformation(translation,rotation,scale);
    return myTransformation;
}

// interprets a concrete object (not composition)
std::shared_ptr<App::ObjectGeneric> App::SceneFromJSON::getConcreteJSONObject(const rapidjson::Value& d) {
    const char* comparisonString(d[VALUE_TYPE].GetString());
    std::shared_ptr<ObjectGeneric> myObject;

    if (strcmp(comparisonString, VALUE_TYPE_OBJECT_SPHERE) == 0) {
        myObject = std::make_shared<ObjectSphere>(ObjectSphere());

    } else if (strcmp(comparisonString, VALUE_TYPE_OBJECT_PLANE) == 0) {
        myObject = std::make_shared<ObjectPlane>(ObjectPlane());

    } else if (strcmp(comparisonString, VALUE_TYPE_OBJECT_CYLINDER) == 0) {
        myObject = std::make_shared<ObjectCylinder>(ObjectCylinder());

    } else if (strcmp(comparisonString, VALUE_TYPE_OBJECT_CONE) == 0) {
        myObject = std::make_shared<ObjectCone>(ObjectCone());

    } else if (strcmp(comparisonString, VALUE_TYPE_OBJECT_CUBOID) == 0) {
        myObject = std::make_shared<ObjectCuboid>(ObjectCuboid());

    } else {
        throw std::invalid_argument("object type is not recognized");
    }
    const rapidjson::Value& transformationValue = d[VALUE_TRANSFORMATION];
    GeometricalTransformation myTransformation = getTransformationFromJSON(transformationValue);
    myObject->setTransformation(myTransformation);

    const rapidjson::Value& materialValue = d[VALUE_MATERIAL];
    std::shared_ptr<MaterialCompleteSimple> myMaterial = getMaterialFromJSON(materialValue);
    myObject->setMaterial(myMaterial);

    return myObject;
}

// interprets a material value
std::shared_ptr<App::MaterialCompleteSimple> App::SceneFromJSON::getMaterialFromJSON(const rapidjson::Value &value) {
    std::shared_ptr<MaterialCompleteSimple> myMaterial = std::make_shared<MaterialCompleteSimple>(MaterialCompleteSimple());

    const rapidjson::Value& reflectivityValue = value[VALUE_REFLECTIVITY];
    const rapidjson::Value& shininessValue = value[VALUE_SHININESS];
    const rapidjson::Value& translucencyValue = value[VALUE_TRANSLUCENCY];
    const rapidjson::Value& indexOfRefractionValue = value[VALUE_INDEX_OF_REFRACTION];
    const rapidjson::Value& textureValue = value[VALUE_TEXTURE];

    myMaterial->m_reflectivity = reflectivityValue.GetDouble();
    myMaterial->m_shininess = shininessValue.GetDouble();
    myMaterial->m_translucency = translucencyValue.GetDouble();
    myMaterial->m_indexOfRefraction = indexOfRefractionValue.GetDouble();
    std::shared_ptr<Textures::TextureGeneric> myTexture = getTextureFromJSON(textureValue);
    myMaterial->m_texture = myTexture;

    return myMaterial;
}

// interprets a texture value
std::shared_ptr<App::Textures::TextureGeneric> App::SceneFromJSON::getTextureFromJSON(const rapidjson::Value &value) {
    const char* comparisonString(value[VALUE_TYPE].GetString());
    std::cout<<"texture type: "<<comparisonString<<std::endl;

    if (strcmp(comparisonString, VALUE_TYPE_TEXTURE_FLAT) == 0) {
        std::shared_ptr<Textures::TextureFlat> myTexture;
        myTexture = getTextureFlatFromJSON(value);
        return myTexture;
    } else if (strcmp(comparisonString, VALUE_TYPE_TEXTURE_CHECKER) == 0) {
        std::shared_ptr<Textures::TextureChecker> myTexture;
        myTexture = getTextureCheckerFromJSON(value);
        return myTexture;
    } else if (strcmp(comparisonString, VALUE_TYPE_TEXTURE_GRADIENT) == 0) {
        std::shared_ptr<Textures::TextureGradient> myTexture;
        myTexture = getTextureGradientFromJSON(value);
        return myTexture;
    } else {
        throw std::invalid_argument("material texture type is not recognized");
    }
}

// interprets a texture flat value
std::shared_ptr<App::Textures::TextureFlat> App::SceneFromJSON::getTextureFlatFromJSON(const rapidjson::Value &value) {
    std::shared_ptr<Textures::TextureFlat> myTexture = std::make_shared<Textures::TextureFlat>(Textures::TextureFlat());
    const rapidjson::Value& colorValue = value[VALUE_COLOR];
    Vector3d color = getRGBVector3dFromValue(colorValue);
    myTexture->setColor(color);
    return myTexture;
}

// interprets a texture checker value
std::shared_ptr<App::Textures::TextureChecker> App::SceneFromJSON::getTextureCheckerFromJSON(const rapidjson::Value &value) {
    std::shared_ptr<Textures::TextureChecker> myTexture = std::make_shared<Textures::TextureChecker>(Textures::TextureChecker());
    const rapidjson::Value& colorValue1 = value[VALUE_COLOR1];
    const rapidjson::Value& colorValue2 = value[VALUE_COLOR2];
    Vector3d color1 = getRGBVector3dFromValue(colorValue1);
    Vector3d color2 = getRGBVector3dFromValue(colorValue2);
    myTexture->setColor(color1,color2);

    const rapidjson::Value& transformationUVValue = value[VALUE_TRANSFORMATION_UV];
    const rapidjson::Value& translationValue = transformationUVValue[VALUE_TRANSLATION];
    const rapidjson::Value& rotationValue = transformationUVValue[VALUE_ROTATION];
    const rapidjson::Value& scaleValue = transformationUVValue[VALUE_SCALE];
    Vector2d translation = getXYVector2dFromValue(translationValue);
    double rotation = rotationValue.GetDouble();
    Vector2d scale = getXYVector2dFromValue(scaleValue);
    myTexture->setTransformation(translation,rotation,scale);

    return myTexture;
}

// interprets a texture gradient value
std::shared_ptr<App::Textures::TextureGradient> App::SceneFromJSON::getTextureGradientFromJSON(const rapidjson::Value &value) {
    std::shared_ptr<Textures::TextureGradient> myTexture = std::make_shared<Textures::TextureGradient>(Textures::TextureGradient());
    const rapidjson::Value& stopsListValue = value[VALUE_TEXTURE_GRADIENT_STOPS];
    for(rapidjson::Value const& stop: stopsListValue.GetArray()){
        const rapidjson::Value& positionValue = stop[VALUE_POSITION];
        double position = positionValue.GetDouble();
        const rapidjson::Value& colorValue = stop[VALUE_COLOR];
        Vector3d color = getRGBVector3dFromValue(colorValue);
        myTexture -> setStop(position, color);
    }
    return myTexture;
}