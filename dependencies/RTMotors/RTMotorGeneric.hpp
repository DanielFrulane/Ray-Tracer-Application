#ifndef RAY_TRACER_APPLICATION_RTMOTORGENERIC_HPP
#define RAY_TRACER_APPLICATION_RTMOTORGENERIC_HPP

#include "../../include/Eigen/Eigen"
#include "../Ray.hpp"
#include "../TileInformation.hpp"
#include "../HitInformation.hpp"
#include "../scenes/SceneGeneric.hpp"
#include <thread>
#include <atomic>

# define ARBITRARY_HIGH_NUMBER 1e6

#define TILE_NOT_RENDERED   0
#define TILE_RENDERING      1
#define TILE_RENDERED       2

namespace App {
    class RTMotorGeneric {
    public:
        RTMotorGeneric();
        ~RTMotorGeneric();

        SceneGeneric m_scene;
        void setScene(const SceneGeneric &scene);

        static int convertCoordinatesToLinearIndex(int x, int y, int width, int height);
        virtual void iterateTiles(std::vector<App::TileInformation>& listOfTiles, std::vector<std::atomic<int> *>& listOfTileFlags);
    };
}

#endif //RAY_TRACER_APPLICATION_RTMOTORGENERIC_HPP
