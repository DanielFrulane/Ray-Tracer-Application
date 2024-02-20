#ifndef APPLICATION_H
#define APPLICATION_H

#include "include/SDL2/SDL.h"
#include "dependencies/RTImage.hpp"
#include "dependencies/scenes/SceneGeneric.hpp"
#include "dependencies/scenes/SceneBasic.hpp"
#include "dependencies/scenes/SceneFromJson.hpp"
#include "dependencies/Camera.hpp"
#include "dependencies/TileInformation.hpp"
#include "include/opencv2/opencv.hpp"
#include <thread>
#include <atomic>


class Application{
private:
    // mainly SDL related functions
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    App::RTImage m_image; // image to be rendered
    int m_numberOfTilesInWidth, m_numberOfTilesInHeight;
    int m_width, m_height;
    void convertImageToTexture(App::TileInformation &tile);
    Uint32 convertColor(const double r, const double g, const double b);
    double m_maxGammaLevel = 1.0;

    bool generateTileGrid(int tileWidth, int tileHeight);
    bool destroyTileGrid();
    void resetTileFlags();

    int m_maxThreads = 8;
    int m_currentNumberOfThreads = 0;
    std::vector<std::thread> m_threads;
    std::atomic<int> *m_threadCounter;
    std::vector<App::TileInformation> m_tiles;
    std::vector<std::atomic<int>*> m_tileFlags;

public:
    App::SceneGeneric m_scene = App::SceneBasic(); ////////////// TODO ANY SCENE
    // mainly SDL related functions
    Application();
    int inExecution();
    bool inInitialization();
    void inEvent(SDL_Event *event);
    void inLoop();
    void inRender();
    void inExit();
    void renderTile(App::TileInformation *tile, std::atomic<int> *threadCounter, std::atomic<int> *tileFlag);
};

#endif //APPLICATION_H
