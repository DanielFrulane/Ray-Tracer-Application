#include "TextureGradient.hpp"

App::Textures::TextureGradient::TextureGradient() {

}

App::Textures::TextureGradient::~TextureGradient() {

}

Vector3d App::Textures::TextureGradient::getColor(const Vector2d &uvCoords) {
    // Apply the local transform to the (u,v) coordinates.
    Vector2d inputLocal = uvCoords;
    Vector2d newLocal = applyTransformation(inputLocal);
    double newU = std::min((newLocal(1) + 1.0) / 2.0, 1.0); // TODO CONVERSION BASE
    return m_colorMap.getColor(newU);
}

void App::Textures::TextureGradient::setStop(double position, const Vector3d &value) {
    m_colorMap.setStop(position, value);
}
