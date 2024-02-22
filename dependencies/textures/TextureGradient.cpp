#include "TextureGradient.hpp"

// n colors displayed in a gradient pattern

App::Textures::TextureGradient::TextureGradient() = default;

App::Textures::TextureGradient::~TextureGradient() = default;

Vector3d App::Textures::TextureGradient::getColor(const Vector2d &uvCoords) {
    Vector2d newLocal = applyTransformation(uvCoords);
    double newU = std::min((newLocal(1) + 1.0) / 2.0, 1.0);
    return m_colorMap.getColor(newU);
}

// set stops for the color map
void App::Textures::TextureGradient::setStop(double position, const Vector3d &value) {
    m_colorMap.setStop(position, value);
}
