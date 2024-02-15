#include "TextureFlat.hpp"

App::Textures::TextureFlat::TextureFlat() {
    m_color = {1.0,1.0,1.0};
}

App::Textures::TextureFlat::~TextureFlat() {

}

Vector3d App::Textures::TextureFlat::getColor(const Vector2d &uvCoordinates) {
    return m_color;
}

void App::Textures::TextureFlat::setColor(const Vector3d &newColor) {
    m_color = newColor;
}
