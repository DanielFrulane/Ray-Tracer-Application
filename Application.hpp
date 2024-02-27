#ifndef APPLICATION_H
#define APPLICATION_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"
#include "dependencies/scenes/SceneGeneric.hpp"
#include "dependencies/scenes/SceneBasic.hpp"
#include "dependencies/scenes/SceneFromJson.hpp"
#include "dependencies/Camera.hpp"
#include "dependencies/TileInformation.hpp"
#include "dependencies/RTMotors/RTMotorGeneric.hpp"
#include "dependencies/RTMotors/RTMotorThreaded.hpp"
#include "dependencies/RTMotors/RTMotorSimple.hpp"
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

    // what will generate the scene
    int m_chosenMotor;
    App::RTMotorThreaded m_motorThreaded;
    App::RTMotorSimple m_motorSimple;

    // Convert colors ranged from 0 to 1 to unsigned integers for SDL interpretation
    Uint32 convertColor(const double r, const double g, const double b);
    double m_maxGammaLevel; // for gamma correction
    void convertImageToTexture(App::TileInformation &tile);

    // threaded initialization of tiles
    int m_numberOfTilesInWidth, m_numberOfTilesInHeight;
    std::vector<App::TileInformation> m_tiles;
    std::vector<std::atomic<int>*> m_tileFlags; // not rendered, rendered, rendering
    void resetTileFlags();
    bool generateTileGrid(int tileWidth, int tileHeight);
    bool destroyTileGrid();

public:
    Application();
    int inExecution();

private: // mainly SDL related functions for rendering and showing the image
    bool inInitialization();
    void inEvent(SDL_Event *event);
    void inLoop();
    void inRender();
    void inExit();

    // initializes the application
    void printWelcome();
    void setDirectory();

    // saves image
    void saveRenderedAsTypePNG();
};

#endif //APPLICATION_H
