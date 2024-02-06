#include "Application.hpp"
#include "include/Eigen/Eigen"
#include <iostream>

#define SCREEN_WIDTH        1280
#define SCREEN_HEIGHT       720
#define IMAGE_DEPTH         32

Application::Application() {
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
    m_scene = App::Scene(static_cast<double>(SCREEN_WIDTH) / static_cast<double>(SCREEN_HEIGHT));
}

// firstly runned code
bool Application::inInitialization() {
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){
        return false;
    }
    pWindow = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (pWindow != NULL){
        // window initialization and image rendering
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        m_image.initialize(SCREEN_WIDTH, SCREEN_HEIGHT, IMAGE_DEPTH, pRenderer);

        SDL_SetRenderDrawColor(pRenderer, 255.0, 255.0, 255.0, 255.0);
        SDL_RenderClear(pRenderer);
        m_scene.render(m_image);
        m_image.display();
        SDL_RenderPresent(pRenderer);
    } else {
        return false;
    }
    return true;
}

int Application::inExecution(){
    SDL_Event event;
    if (!inInitialization()){ // initialization successful
        return -1;
    }
    std::cout<<"Initialized\n";
    while (isRunning){
        while(SDL_PollEvent(&event) != 0){
            inEvent(&event);
        }
        inLoop();
        inRender();
    }
    inExit();
    return 0;
}

// for SDL related events
void Application::inEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT){
        isRunning = false;
    }
}

void Application::inLoop() {
    ;
}

// display image
void Application::inRender() {
    /*SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);
    m_scene.render(m_image);
    m_image.display();
    SDL_RenderPresent(pRenderer); // renders*/
}

// quit
void Application::inExit() {
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

void Application::printVector(const Eigen::Vector3d &toPrint) {
    std::cout<<toPrint<<std::endl;
}