#ifndef APPLICATION_H
#define APPLICATION_H

#include "include/SDL2/SDL.h"
#include "dependencies/RTImage.hpp"
#include "dependencies/Scene.hpp"
#include "dependencies/Camera.hpp"
#include "dependencies/TileInformation.hpp"


class Application{
private:
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    App::RTImage m_image; // image to be rendered
    App::Scene m_scene;
    std::vector<App::TileInformation> m_tiles;
    std::vector<int> m_tileFlags;
    int numberOfTilesInWidth, numberOfTilesInHeight;
    int m_width, mHeight;
    void convertImageToTexture(App::TileInformation &tile);
    Uint32 convertColor(const double r, const double g, const double b);
    double m_maxGammaLevel = 0.8;

    void printVector(const Vector3d &toPrint);

    bool generateTileGrid(int tileWidth, int tileHeight);
    bool destroyTileGrid();

public: // mainly SDL related functions
    Application();
    int inExecution();
    bool inInitialization();
    void inEvent(SDL_Event *event);
    void inLoop();
    void inRender();
    void inExit();
};

#endif //APPLICATION_H
