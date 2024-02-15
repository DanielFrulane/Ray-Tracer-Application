#include "TextureChecker.hpp"

App::Textures::TextureChecker::TextureChecker() {
    m_color1 = {0.0,0.0,0.0};
    m_color2 = {1.0,1.0,1.0};
}

App::Textures::TextureChecker::~TextureChecker() {

}

Vector3d App::Textures::TextureChecker::getColor(const Vector2d &uvCoordinates) {
    Vector2d inputLocal = uvCoordinates;
    Vector2d newLocal = applyTransformation(inputLocal);
    double newU = newLocal(0);
    double newV = newLocal(1);

    Vector3d localColor;
    int check = static_cast<int>(floor(newU)) + static_cast<int>(floor(newV));

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