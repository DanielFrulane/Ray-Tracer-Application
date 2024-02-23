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
        ~SceneFromJSON() override;
        void generateSceneObjects() override;

    private:
        void interpretFile(const char* fileName);
        void interpretJSONType(const rapidjson::Value& d);
        void interpretJSONCamera(const rapidjson::Value& d);
        void interpretJSONLight(const rapidjson::Value& d);

        static Vector2d getXYVector2dFromValue(const rapidjson::Value &value);
        static Vector3d getRGBVector3dFromValue(const rapidjson::Value &value);
        static Vector3d getXYZVector3dFromValue(const rapidjson::Value &value);
        static GeometricalTransformation getTransformationFromJSON(const rapidjson::Value &value);

        static std::shared_ptr<ObjectGeneric> getConcreteJSONObject(const rapidjson::Value& d);
        static std::shared_ptr<MaterialCompleteSimple> getMaterialFromJSON(const rapidjson::Value &value);
        static std::shared_ptr<Textures::TextureGeneric> getTextureFromJSON(const rapidjson::Value &value);
        static std::shared_ptr<Textures::TextureFlat> getTextureFlatFromJSON(const rapidjson::Value &value);
        static std::shared_ptr<Textures::TextureGradient> getTextureGradientFromJSON(const rapidjson::Value &value);
        static std::shared_ptr<Textures::TextureChecker> getTextureCheckerFromJSON(const rapidjson::Value &value);
    };
}

#endif //RAY_TRACER_APPLICATION_SCENEFROMJSON_HPP
