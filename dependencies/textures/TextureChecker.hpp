#ifndef RAY_TRACER_APPLICATION_TEXTURECHECKER_HPP
#define RAY_TRACER_APPLICATION_TEXTURECHECKER_HPP

#include "TextureGeneric.hpp"


namespace App::Textures {
    class TextureChecker : public TextureGeneric{
    public:
        TextureChecker();
        ~TextureChecker() override;

        // Function to return the color.
        Vector3d getColor(const Vector2d &uvCoordinates) override;

        // Function to set the colors.
        void setColor(const Vector3d &newColor1, const Vector3d &newColor2);

    private:
        Vector3d m_color1;
        Vector3d m_color2;
    };
}


#endif //RAY_TRACER_APPLICATION_TEXTURECHECKER_HPP
