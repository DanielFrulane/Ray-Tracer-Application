#ifndef RAY_TRACER_APPLICATION_TILEINFORMATION_HPP
#define RAY_TRACER_APPLICATION_TILEINFORMATION_HPP

#include <memory>
#include "../include/Eigen/Eigen"
#include "../include/SDL2/SDL.h"

using namespace Eigen;

namespace App {
    class ObjectGeneric;

    struct TileInformation {
        int x;
        int y;
        int width;
        int height;
        int renderComplete = 0;
        bool textureComplete = false;
        SDL_Texture *pTexture;
        std::vector<Vector3d> rgb;
    };

}

#endif //RAY_TRACER_APPLICATION_TILEINFORMATION_HPP
