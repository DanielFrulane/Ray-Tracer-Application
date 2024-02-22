#include "TextureGeneric.hpp"
#include <cmath>

App::Textures::TextureGeneric::TextureGeneric() = default;

App::Textures::TextureGeneric::~TextureGeneric() = default;

Vector3d App::Textures::TextureGeneric::getColor(const Vector2d &uvCoordinates) {
    return {};
}

// sets the UV transformation (2D) to be applied when calculating the correct placement of colors
void App::Textures::TextureGeneric::setTransformation(const Vector2d &translation, const double &rotationInPercentage,
                                                      const Vector2d &scale) {


    double rotation = rotationInPercentage * 2 * M_PI;
    Matrix3d rotationMatrix {{cos(rotation), -sin(rotation), 0.0},
                             {sin(rotation), cos(rotation),  0.0},
                             {0.0, 0.0, 1.0}};

    Matrix3d scaleMatrix {{scale(0), 0.0, 0.0},
                          {0.0, scale(1), 0.0},
                          {0.0, 0.0, 1.0}};

    Matrix3d translationMatrix {{1.0, 0.0, translation(0)},
                                {0.0, 1.0, translation(1)},
                                {0.0, 0.0, 1.0}};

    m_transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

// transforms a vector to its new UV coordinates
Vector2d App::Textures::TextureGeneric::applyTransformation(const Vector2d &input) {
    // turns it into 3D
    Vector3d newInput;
    newInput(0) = input(0);
    newInput(1) = input(1);
    newInput(2) = 0.0;

    Vector3d result = m_transformationMatrix * newInput;

    // turns it back into 2D
    Vector2d output;
    output(0) = result(0);
    output(1) = result(1);

    return output;
}
