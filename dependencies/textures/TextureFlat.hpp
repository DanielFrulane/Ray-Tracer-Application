#ifndef RAY_TRACER_APPLICATION_TEXTUREFLAT_HPP
#define RAY_TRACER_APPLICATION_TEXTUREFLAT_HPP

#include "TextureGeneric.hpp"

// flat texture that displays only one homogeneous color

namespace App::Textures {
    class TextureFlat : public TextureGeneric {
    private:
        Vector3d m_color;
    public:
        TextureFlat();
        ~TextureFlat() override;

        Vector3d getColor(const Vector2d &uvCoordinates) override;

        void setColor(const Vector3d &newColor);
    };
}


#endif //RAY_TRACER_APPLICATION_TEXTUREFLAT_HPP
