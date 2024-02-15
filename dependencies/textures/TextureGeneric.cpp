#include "TextureGeneric.hpp"
#include <cmath>
#include <iostream>

App::Textures::TextureGeneric::TextureGeneric() {

}

App::Textures::TextureGeneric::~TextureGeneric() {

}

Vector3d App::Textures::TextureGeneric::getColor(const Vector2d &uvCoordinates) {
    return {};
}

void App::Textures::TextureGeneric::setTransformation(const Vector2d &translation, const double &rotation,
                                                      const Vector2d &scale) {
    Matrix3d rotationMatrix {{cos(rotation), -sin(rotation), 0.0},
                             {sin(rotation), cos(rotation), 0.0},
                             {0.0, 0.0, 1.0}};

    Matrix3d scaleMatrix {{scale(0), 0.0, 0.0},
                          {0.0, scale(1), 0.0},
                          {0.0, 0.0, 1.0}};

    Matrix3d translationMatrix {{1.0, 0.0, translation(0)},
                                {0.0, 1.0, translation(1)},
                                {0.0, 0.0, 1.0}};

    m_transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

Vector3d App::Textures::TextureGeneric::blendRGBAColorsToRGB(const std::vector<Vector3d> &inputColorList) {
    return {};
}

Vector2d App::Textures::TextureGeneric::applyTransformation(const Vector2d &input) {
    Vector3d newInput;
    newInput(0) = input(0);
    newInput(1) = input(1);
    newInput(2) = 0.0;

    Vector3d result = m_transformationMatrix * newInput;

    Vector2d output;
    output(0) = result(0);
    output(1) = result(1);

    return output;
}
