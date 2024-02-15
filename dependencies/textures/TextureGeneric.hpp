#ifndef RAY_TRACER_APPLICATION_TEXTUREGENERIC_HPP
#define RAY_TRACER_APPLICATION_TEXTUREGENERIC_HPP

#include <memory>
#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"

using namespace Eigen;

namespace App {
    namespace Textures {
        class TextureGeneric {
        private:
            Matrix3d m_transformationMatrix = Matrix3d::Identity();
        public:
            TextureGeneric();
            virtual ~TextureGeneric();
            
            virtual Vector3d getColor(const Vector2d &uvCoordinates);

            void setTransformation(const Vector2d &translation, const double &rotation, const Vector2d &scale);

            static Vector3d blendRGBAColorsToRGB(const std::vector<Vector3d> &inputColorList);

            Vector2d applyTransformation(const Vector2d &input);
        };
    }
}

#endif //RAY_TRACER_APPLICATION_TEXTUREGENERIC_HPP
