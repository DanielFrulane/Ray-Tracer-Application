#ifndef RAY_TRACER_APPLICATION_TEXTUREGRADIENT_HPP
#define RAY_TRACER_APPLICATION_TEXTUREGRADIENT_HPP

#include "TextureGeneric.hpp"
#include "ColorMap.hpp"

namespace App {
    namespace Textures {
        class TextureGradient : public TextureGeneric {
        private:
            ColorMap m_colorMap;
        public:
            TextureGradient();
            virtual ~TextureGradient() override;

            // Function to return the color.
            virtual Vector3d getColor(const Vector2d &uvCoords) override;

            // Function to set stops for the color map.
            void setStop(double position, const Vector3d &value);
        };
    }
}

#endif //RAY_TRACER_APPLICATION_TEXTUREGRADIENT_HPP
