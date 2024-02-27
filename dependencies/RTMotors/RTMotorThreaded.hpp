#ifndef RAY_TRACER_APPLICATION_RTMOTORTHREADED_HPP
#define RAY_TRACER_APPLICATION_RTMOTORTHREADED_HPP

#include "RTMotorGeneric.hpp"

namespace App {
    class RTMotorThreaded : public RTMotorGeneric{
    public:
        RTMotorThreaded();
        ~RTMotorThreaded();
        void iterateTiles(std::vector<App::TileInformation>& listOfTiles, std::vector<std::atomic<int> *>& listOfTileFlags) override;

    private:
        // thread support
        int m_maxThreads;
        std::atomic<int> *m_threadCounter;
        std::vector<std::thread> m_threads;

        void renderTile(App::TileInformation *tile);
        void renderTileFromIteration(App::TileInformation *tile, std::atomic<int> *threadCounter, std::atomic<int> *tileFlag);
        bool castRay(Ray &castedRay, std::shared_ptr<ObjectGeneric> &closestObject, HitInformation &closestHitInformation);

        Vector3d renderPixel(int x, int y, int width, int height);
    };
}

#endif //RAY_TRACER_APPLICATION_RTMOTORTHREADED_HPP
