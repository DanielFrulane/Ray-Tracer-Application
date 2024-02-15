#ifndef RAY_TRACER_APPLICATION_TEXTUREFLAT_HPP
#define RAY_TRACER_APPLICATION_TEXTUREFLAT_HPP

#include "TextureGeneric.hpp"

namespace App{
    namespace Textures {
        class TextureFlat : public TextureGeneric {
        private:
            Vector3d m_color;
        public:
            TextureFlat();
            virtual ~TextureFlat() override;

            virtual Vector3d getColor(const Vector2d &uvCoordinates) override;

            void setColor(const Vector3d &newColor);
        };
    }
}

#endif //RAY_TRACER_APPLICATION_TEXTUREFLAT_HPP
