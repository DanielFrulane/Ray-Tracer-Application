#include "SceneFromJson.hpp"
#include <iostream>

#define OBJECT_FOLDER_NAME "objectFiles"
#define VALUE_TYPE_CAMERA "camera"
#define VALUE_TYPE_LIGHT "light"
#define VALUE_TYPE_SPHERE "sphere"
#define VALUE_TYPE_PLANE "plane"
#define VALUE_TYPE_CYLINDER "cylinder"
#define VALUE_TYPE_CONE "cone"
#define VALUE_TYPE_CUBOID "cuboid"
#define VALUE_TYPE_COMPOSITION "composition"
#define VALUE_POSITION "position"
#define VALUE_LOOK_AT "lookAt"
#define VALUE_UP "up"
#define VALUE_COLOR "color"
#define VALUE_HORIZONTAL_SIZE "horizontalSize"
#define VALUE_ASPECT_RATIO "aspectRatio"
#define VALUE_TRANSFORMATION "transformation"
#define VALUE_TRANSLATION "translation"
#define VALUE_ROTATION "rotation"
#define VALUE_SCALE "scale"

App::SceneFromJSON::SceneFromJSON() {
    setDirectory();
    generateSceneObjects();
    //checkIfHasAllNecessaryComponents();
    m_camera.updateStatus();
    std::cout<<"configured objects"<<std::endl;
}

void App::SceneFromJSON::checkIfHasAllNecessaryComponents() {

}

App::SceneFromJSON::~SceneFromJSON() {

}

void App::SceneFromJSON::generateSceneObjects() {
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            if (entry.path().extension() == ".json") {
                std::cout << "Reading file: " << entry.path().filename() << std::endl;
                interpretFile(entry.path().filename().string().c_str());
            }
        }
    }
}

void App::SceneFromJSON::setDirectory() {
    m_path = std::filesystem::current_path();
    //m_path = m_path.parent_path().parent_path();
    //std::cout<<"path "<<m_path<<std::endl;
    m_path = m_path / OBJECT_FOLDER_NAME;
    std::cout<<"path: "<<m_path<<std::endl;
    std::filesystem::current_path(m_path);
}

void App::SceneFromJSON::interpretFile(const char* fileName) {
    // creates file reader
    FILE* fp = fopen(fileName, "r");
    char readBuffer[50000];
    rapidjson::FileReadStream is(fp, readBuffer,sizeof(readBuffer));

    // Parse the JSON data using a Document object
    rapidjson::Document d;
    d.ParseStream(is);

    // Close the file
    fclose(fp);

    interpretJSONType(d);
}

void App::SceneFromJSON::interpretJSONType(rapidjson::Document& d) {
    const char* comparisonString(d["type"].GetString());
    std::cout<<"type: "<<comparisonString<<std::endl;

    if (strcmp(comparisonString, VALUE_TYPE_CAMERA) == 0) {
        interpretJSONCamera(d);
    } else if (strcmp(comparisonString, VALUE_TYPE_LIGHT) == 0) {
        interpretJSONLight(d);
    } else { // is object?
        const rapidjson::Value& transformationValue = d[VALUE_TRANSFORMATION];
        GeometricalTransformation myTransformation = getTransformationFromJSON(transformationValue);
        std::shared_ptr<ObjectGeneric> myObject;

        if (strcmp(comparisonString, VALUE_TYPE_COMPOSITION) == 0) {
            //myObject = std::make_shared<ObjectComposition> (ObjectComposition());
            //myObject->setTransformation(myTransformation);
            //// TODO RECURSIVE

        } else {
            if (strcmp(comparisonString, VALUE_TYPE_SPHERE) == 0) {
                myObject = std::make_shared<ObjectSphere>(ObjectSphere());
                myObject->m_color = {0.5, 0.5, 0.5};
                myObject->setTransformation(myTransformation);
                m_objectsInScene.push_back(myObject);

            } else if (strcmp(comparisonString, VALUE_TYPE_PLANE) == 0) {
                myObject = std::make_shared<ObjectPlane>(ObjectPlane());

            } else if (strcmp(comparisonString, VALUE_TYPE_CYLINDER) == 0) {
                myObject = std::make_shared<ObjectCylinder>(ObjectCylinder());

            } else if (strcmp(comparisonString, VALUE_TYPE_CONE) == 0) {
                myObject = std::make_shared<ObjectCone>(ObjectCone());

            } else if (strcmp(comparisonString, VALUE_TYPE_CUBOID) == 0) {
                myObject = std::make_shared<ObjectCuboid>(ObjectCuboid());

            } else {
                // error throw;
            }
        }
    }
}

void App::SceneFromJSON::interpretJSONCamera(rapidjson::Document &d) {
    if (m_hasCamera){
        // TODO error throw
    }
    const rapidjson::Value& positionValue = d[VALUE_POSITION];
    const rapidjson::Value& lookAtValue = d[VALUE_LOOK_AT];
    const rapidjson::Value& upValue = d[VALUE_UP];
    const rapidjson::Value& horizontalSizeValue = d[VALUE_HORIZONTAL_SIZE];
    const rapidjson::Value& aspectRatioValue = d[VALUE_ASPECT_RATIO];
    Vector3d position = getXYZVector3dFromValue(positionValue);
    Vector3d lookAt = getXYZVector3dFromValue(lookAtValue);
    Vector3d up = getXYZVector3dFromValue(upValue);
    double horizontalSize = horizontalSizeValue.GetDouble();
    double aspectRatio = aspectRatioValue.GetDouble();
    std::cout << "position: " << position << std::endl;
    std::cout << "lookat: " << lookAt << std::endl;
    std::cout << "up: " << up << std::endl;
    std::cout << "hs: " << horizontalSize << std::endl;
    std::cout << "ar: " << aspectRatio << std::endl;

    m_camera.setPosition(position);
    m_camera.setLookAt(lookAt);
    m_camera.setUp(up);
    m_camera.setHorizontalSize(horizontalSize);
    m_camera.setAspectRatio(aspectRatio);

    m_hasCamera = true;
}

void App::SceneFromJSON::interpretJSONLight(rapidjson::Document &d) {
    const rapidjson::Value& positionValue = d[VALUE_POSITION];
    const rapidjson::Value& colorValue = d[VALUE_COLOR];
    Vector3d position = getXYZVector3dFromValue(positionValue);
    Vector3d color = getXYZVector3dFromValue(colorValue);
    std::cout << "position: " << position << std::endl;
    std::cout << "color: " << color << std::endl;

    std::shared_ptr<LightPoint> myLight = std::make_shared<LightPoint>(LightPoint());
    myLight -> m_position = position;
    myLight -> m_color = color;
    m_lightsInScene.push_back(myLight);

    m_hasAtLeastOneLight = true;
}

Vector3d App::SceneFromJSON::getXYZVector3dFromValue(const rapidjson::Value &value) {
    double x, y, z;
    x = value["x"].GetDouble();
    y = value["y"].GetDouble();
    z = value["z"].GetDouble();
    return {x,y,z};
}

App::GeometricalTransformation App::SceneFromJSON::getTransformationFromJSON(const rapidjson::Value &value) {
    const rapidjson::Value& translationValue = value[VALUE_TRANSLATION];
    const rapidjson::Value& rotationValue = value[VALUE_ROTATION];
    const rapidjson::Value& scaleValue = value[VALUE_SCALE];
    Vector3d translation = getXYZVector3dFromValue(translationValue);
    Vector3d rotation = getXYZVector3dFromValue(rotationValue);
    Vector3d scale = getXYZVector3dFromValue(scaleValue);
    std::cout << "translation: " << translation << std::endl;
    std::cout << "rotation: " << rotation << std::endl;
    std::cout << "scale: " << scale << std::endl;
    GeometricalTransformation myTransformation;
    myTransformation.setTransformation(translation,rotation,scale);
    return myTransformation;
}



// getTransformationFromJSON
// getXYZVector3dFromJSON


// m_objectsInScene.push_back(std::make_shared<ObjectPlane>(ObjectPlane()));
// std::shared_ptr<MaterialSimple> whiteDiffuse = std::make_shared<MaterialSimple> (MaterialSimple());
// m_lightsInScene.push_back(std::make_shared<LightPoint>(LightPoint()));


// exceptions

/*
std::string message = "Transform dimensions must be 4x4, check:" +
                      std::string("\nForward Transformation) Rows: ") + std::to_string(forward.rows()) +
                      std::string(", Columns: ") + std::to_string(forward.cols()) +
                      std::string("\nBackward Transformation) Rows: ") + std::to_string( backward.rows()) +
                      std::string(", Columns: ") + std::to_string(backward.cols());
throw std::invalid_argument(message);
 * */

/*
 * if (!fp) {
        std::cerr << "Failed to open input.json for reading." << std::endl;
        return 1;
    }
 */

// directory not found
// convert to float
// not enough scene descriptions