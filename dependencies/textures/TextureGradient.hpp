#ifndef RAY_TRACER_APPLICATION_TEXTUREGRADIENT_HPP
#define RAY_TRACER_APPLICATION_TEXTUREGRADIENT_HPP

#include "TextureGeneric.hpp"
#include "ColorMap.hpp"


namespace App::Textures {
    class TextureGradient : public TextureGeneric {
    private:
        ColorMap m_colorMap;
    public:
        TextureGradient();
        ~TextureGradient() override;

        Vector3d getColor(const Vector2d &uvCoords) override;
        void setStop(double position, const Vector3d &value);
    };
}


#endif //RAY_TRACER_APPLICATION_TEXTUREGRADIENT_HPP
