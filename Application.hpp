#ifndef APPLICATION_H
#define APPLICATION_H

#include "include/SDL2/SDL.h"
#include "dependencies/RTImage.hpp"
#include "dependencies/Scene.hpp"
#include "dependencies/Camera.hpp"


class Application{
private:
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    App::RTImage m_image; // image to be rendered
    App::Scene m_scene;
    void printVector(const Vector3d &toPrint);

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
