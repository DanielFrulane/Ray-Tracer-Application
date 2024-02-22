#include "TextureChecker.hpp"

// two colors displayed in a checker pattern

App::Textures::TextureChecker::TextureChecker() {
    m_color1 = {0.0,0.0,0.0};
    m_color2 = {1.0,1.0,1.0};
}

App::Textures::TextureChecker::~TextureChecker() = default;

Vector3d App::Textures::TextureChecker::getColor(const Vector2d &uvCoordinates) {
    // applies transformation in UV coordinates
    Vector2d newLocalUV = applyTransformation(uvCoordinates);
    Vector3d localColor;

    // conversion of UV coordinates to linear information
    int check = static_cast<int>(floor(newLocalUV(0))) + static_cast<int>(floor(newLocalUV(1)));
    if ((check % 2) == 0) {
        localColor = m_color1;
    } else {
        localColor = m_color2;
    }
    return localColor;
}

void App::Textures::TextureChecker::setColor(const Vector3d &newColor1, const Vector3d &newColor2) {
    m_color1 = newColor1;
    m_color2 = newColor2;
}