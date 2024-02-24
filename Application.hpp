#ifndef APPLICATION_H
#define APPLICATION_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"
#include "dependencies/scenes/SceneGeneric.hpp"
#include "dependencies/scenes/SceneBasic.hpp"
#include "dependencies/scenes/SceneFromJson.hpp"
#include "dependencies/Camera.hpp"
#include "dependencies/TileInformation.hpp"
#include "include/Eigen/Eigen"
#include <thread>
#include <atomic>

// the application responsible for all functionalities

class Application{
private:
    // mainly SDL related functions for rendering and showing the image
    App::SceneGeneric m_scene;
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    int m_width, m_height, m_depth;
    std::filesystem::path m_path;

    // Convert colors ranged from 0 to 1 to unsigned integers for SDL interpretation
    Uint32 convertColor(const double r, const double g, const double b);
    double m_maxGammaLevel; // for gamma correction

    // threaded initialization of tiles
    int m_numberOfTilesInWidth, m_numberOfTilesInHeight;
    void convertImageToTexture(App::TileInformation &tile);
    bool generateTileGrid(int tileWidth, int tileHeight);
    bool destroyTileGrid();
    void resetTileFlags();

    // thread support
    int m_maxThreads;
    int m_currentNumberOfThreads;
    std::atomic<int> *m_threadCounter;
    std::vector<std::thread> m_threads;
    std::vector<App::TileInformation> m_tiles;
    std::vector<std::atomic<int>*> m_tileFlags; // not rendered, rendered, rendering

public:
    // mainly SDL related functions for rendering and showing the image
    Application();
    int inExecution();
    bool inInitialization();
    void inEvent(SDL_Event *event);
    void inLoop();
    void inRender();
    void inExit();

    // initializes the application
    void printWelcome();
    void setDirectory();

    // threaded initialization of tiles
    void renderTile(App::TileInformation *tile, std::atomic<int> *threadCounter, std::atomic<int> *tileFlag);

    // saves image
    void saveRenderedAsTypePNG();
};

#endif //APPLICATION_H
