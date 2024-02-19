#include "SceneFromJson.hpp"
#include <iostream>

#define OBJECT_FOLDER_NAME "objectFiles"
#define TYPE_NAME_CAMERA "sphere"
#define TYPE_NAME_LIGHT "objectFiles"
#define TYPE_NAME_SPHERE "sphere"
#define TYPE_NAME_PLANE "plane"
#define TYPE_NAME_CYLINDER "cylinder"
#define TYPE_NAME_CONE "cone"
#define TYPE_NAME_CUBOID "cuboid"
#define TYPE_NAME_COMPOSITION "composition"

App::SceneFromJSON::SceneFromJSON() {
    setDirectory();
    generateSceneObjects();
    checkIfHasAllNecessaryComponents();
    std::cout<<"generated objects"<<std::endl;
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
                std::cout << "JSON File: " << entry.path().filename() << std::endl;
                interpretFile(entry.path().filename().string().c_str());
            }
        }
    }
}

void App::SceneFromJSON::setDirectory() {
    m_path = std::filesystem::current_path();
    m_path = m_path.parent_path().parent_path() / OBJECT_FOLDER_NAME;
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

    if (strcmp(comparisonString, TYPE_NAME_CAMERA) == 0) {
        interpretJSONCamera(d);
    } else if (strcmp(comparisonString, TYPE_NAME_LIGHT) == 0) {
        LightGeneric myLight;
        interpretJSONLight(d, myLight);
    } else { // is object?
        if (strcmp(comparisonString, TYPE_NAME_COMPOSITION) == 0) {
            interpretJSONComposition(d);
        } else if (strcmp(comparisonString, TYPE_NAME_SPHERE) == 0) {
            interpretJSONSphere(d);
        } else if (strcmp(comparisonString, TYPE_NAME_PLANE) == 0) {
            interpretJSONPlane(d);
        } else if (strcmp(comparisonString, TYPE_NAME_CYLINDER) == 0) {
            interpretJSONCylinder(d);
        } else if (strcmp(comparisonString, TYPE_NAME_CONE) == 0) {
            interpretJSONCone(d);
        } else if (strcmp(comparisonString, TYPE_NAME_CUBOID) == 0) {
            interpretJSONCuboid(d);
        } else {
            // error throw;
        }
    }
}

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

// directory not found
// convert to float
// not enough scene descriptions