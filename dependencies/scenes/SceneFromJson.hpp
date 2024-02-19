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

        void checkIfHasAllNecessaryComponents();
    };
}

#endif //RAY_TRACER_APPLICATION_SCENEFROMJSON_HPP
