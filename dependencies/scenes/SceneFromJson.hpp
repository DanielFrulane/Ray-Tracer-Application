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

        void interpretJSONCamera(rapidjson::Document &d);
        void interpretJSONLight(rapidjson::Document &d);

        Vector3d getXYZVector3dFromValue(const rapidjson::Value &value);

        GeometricalTransformation getTransformationFromJSON(const rapidjson::Value &value);

        std::shared_ptr<ObjectGeneric> configureConcreteJSONObject(rapidjson::Document &d);

        std::shared_ptr<MaterialCompleteSimple> getMaterialFromJSON(const rapidjson::Value &value);

        std::shared_ptr<Textures::TextureGeneric> getTextureFromJSON(const rapidjson::Value &value);

        std::shared_ptr<Textures::TextureFlat> getTextureFlatFromJSON(const rapidjson::Value &value);

        std::shared_ptr<Textures::TextureChecker> getTextureCheckerFromJSON(const rapidjson::Value &value);

        std::shared_ptr<Textures::TextureGradient> getTextureGradientFromJSON(const rapidjson::Value &value);

        Vector2d getXYVecto2dFromValue(const rapidjson::Value &value);

        Vector3d getRGBVector3dFromValue(const rapidjson::Value &value);
    };
}

#endif //RAY_TRACER_APPLICATION_SCENEFROMJSON_HPP
