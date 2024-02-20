#ifndef RAY_TRACER_APPLICATION_SCENEFROMJSON_HPP
#define RAY_TRACER_APPLICATION_SCENEFROMJSON_HPP

#include "SceneGeneric.hpp"
#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/filereadstream.h"
#include <filesystem>

namespace App {
    class SceneFromJSON : public SceneGeneric {
    public:
        SceneFromJSON();
        virtual ~SceneFromJSON() override;
        virtual void generateSceneObjects() override;
    private:
        std::filesystem::path m_path;
        void setDirectory();

        void interpretFile(const char* fileName);
        void interpretJSONType(rapidjson::Document& d);

        bool m_hasAtLeastOneLight = false;
        bool m_hasAtLeastOneObject = false;
        bool m_hasCamera = false;
        void checkIfHasAllNecessaryComponents();

        void interpretJSONCamera(rapidjson::Document &d);
        void interpretJSONLight(rapidjson::Document &d);

        Vector3d getXYZVector3dFromValue(const rapidjson::Value &value);

        GeometricalTransformation getTransformationFromJSON(const rapidjson::Value &value);
    };
}

#endif //RAY_TRACER_APPLICATION_SCENEFROMJSON_HPP
