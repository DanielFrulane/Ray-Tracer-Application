#ifndef RAY_TRACER_APPLICATION_RTMOTORSIMPLE_HPP
#define RAY_TRACER_APPLICATION_RTMOTORSIMPLE_HPP

#include "RTMotorGeneric.hpp"

namespace App {
    class RTMotorSimple : public RTMotorGeneric {
    public:
        int m_currentTileIndex;

        RTMotorSimple();
        ~RTMotorSimple();

        void iterateTiles(std::vector<App::TileInformation>& listOfTiles, std::vector<std::atomic<int> *>& listOfTileFlags) override;

    private:
        void renderTile(App::TileInformation *tile);
        void renderTileFromIteration(App::TileInformation *tile, std::atomic<int> *tileFlag);
        bool castRay(Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject, HitInformation &closestHitInformation);

        Vector3d renderPixel(int x, int y, int width, int height);
    };
}

#endif //RAY_TRACER_APPLICATION_RTMOTORSIMPLE_HPP
