#include "Application.hpp"
#include "include/Eigen/Eigen"
#include <iostream>

#define SCREEN_WIDTH        720
#define SCREEN_HEIGHT       720
#define IMAGE_DEPTH         32
#define TILE_WIDTH          72
#define TILE_HEIGHT         72

#define TILE_NOT_RENDERED   0
#define TILE_RENDERING      1
#define TILE_RENDERED       2

Application::Application() {
    m_scene = App::SceneFromJSON();
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
    m_width = SCREEN_WIDTH;
    m_height = SCREEN_HEIGHT;
    m_maxGammaLevel = 1.0;
    m_maxThreads = 8;
    m_currentNumberOfThreads = 0;
    m_threadCounter = new std::atomic<int> (0);
}

// called to execute the program
int Application::inExecution(){
    SDL_Event event;
    if (!inInitialization()){ // SDL: ensures successful initialization
        return -1;
    }
    std::cout<<"SDL Done initializing\n";
    while (isRunning){
        while(SDL_PollEvent(&event) != 0){
            inEvent(&event);
        }
        inLoop();
        inRender();
        SDL_Delay(1);
    }
    inExit();
    return 0;
}

// in the begining of execution
bool Application::inInitialization() {
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){ // SDL: ensures successful initialization
        return false;
    }
    pWindow = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (pWindow != NULL){
        // initialization of image configurations
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        m_scene.m_height = m_width;
        m_scene.m_width = m_height;

        if (!generateTileGrid(TILE_WIDTH,TILE_HEIGHT)){
            std::cout<<"failed tile Grid"<<std::endl;
            return false;
        }

        std::cout<<"number of tiles: "<<m_tiles.size()<<std::endl;
        SDL_SetRenderDrawColor(pRenderer, 255.0, 255.0, 255.0, 255.0);
        SDL_RenderClear(pRenderer);
        SDL_RenderPresent(pRenderer);
    } else {
        return false;
    }
    return true;
}

// function to generate the all the pixels for all the tiles
void Application::inLoop() {
    for (int i = 0; i < m_tiles.size(); ++i) {
        if (m_tileFlags.at(i)->load(std::memory_order_acquire) == TILE_NOT_RENDERED){
            if (m_threadCounter ->load(std::memory_order_acquire) < m_maxThreads){
                int numberOfActiveThreads = m_threadCounter->load(std::memory_order_acquire);
                m_threadCounter->store(numberOfActiveThreads+1, std::memory_order_release);
                std::thread renderThread (&Application::renderTile, this, &m_tiles.at(i),m_threadCounter,m_tileFlags.at(i));
                renderThread.detach();
            }
        }
    }
}

// displays the image / SDL related
void Application::inRender() {
    double widthFactor = 1.0;
    double heightFactor = 1.0;
    for (int i = 0; i < m_tiles.size(); ++i) {
        if(m_tileFlags.at(i)->load(std::memory_order_acquire)==TILE_RENDERED){
            SDL_Rect sourceRectangle, destinationRectangle;
            sourceRectangle.x = 0;
            sourceRectangle.y = 0;
            sourceRectangle.w = m_tiles.at(i).width;
            sourceRectangle.h = m_tiles.at(i).height;
            destinationRectangle.x = static_cast<int>(std::round(static_cast<double>(m_tiles.at(i).x)*widthFactor));
            destinationRectangle.y = static_cast<int>(std::round(static_cast<double>(m_tiles.at(i).y)*heightFactor));
            destinationRectangle.w = static_cast<int>(std::round(static_cast<double>(m_tiles.at(i).width)*widthFactor));
            destinationRectangle.h = static_cast<int>(std::round(static_cast<double>(m_tiles.at(i).height)*heightFactor));

            if (!m_tiles.at(i).textureComplete){
                convertImageToTexture(m_tiles.at(i));
                m_tiles.at(i).textureComplete = true;
                SDL_RenderCopy(pRenderer, m_tiles.at(i).pTexture, &sourceRectangle, &destinationRectangle);
            }
        }
    }
    SDL_RenderPresent(pRenderer);
}

// for SDL related events
void Application::inEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT){
        isRunning = false;
    }
}

// quit
void Application::inExit() {
    bool result = destroyTileGrid();
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

void Application::convertImageToTexture(App::TileInformation &tile) {
    // pixel buffer.
    Uint32 *tempPixels = new Uint32[tile.width * tile.height];
    memset(tempPixels, 0, tile.width * tile.height * sizeof(Uint32));

    // copies the image
    for (int i=0; i<tile.width * tile.height; ++i){
        tempPixels[i] = convertColor(tile.rgb.at(i)(0), tile.rgb.at(i)(1), tile.rgb.at(i)(2));
    }

    // updates the texture with the pixel buffer
    SDL_UpdateTexture(tile.pTexture, NULL, tempPixels, tile.width * sizeof(Uint32));
    delete[] tempPixels;
}

// Convert colors ranged from 0 to 1 to unsigned integers for SDL interpretation
Uint32 Application::convertColor(const double red, const double green, const double blue) {
    double newRed = std::max(std::min(std::pow(red, m_maxGammaLevel), 1.0), 0.0);
    double newGreen = std::max(std::min(std::pow(green, m_maxGammaLevel), 1.0), 0.0);
    double newBlue = std::max(std::min(std::pow(blue, m_maxGammaLevel), 1.0), 0.0);

    unsigned char r = static_cast<unsigned char>(newRed * 255.0);
    unsigned char g = static_cast<unsigned char>(newGreen * 255.0);
    unsigned char b = static_cast<unsigned char>(newBlue * 255.0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN // from SDL documentation
    Uint32 pixelColor = (b << 24) + (g << 16) + (r << 8) + 255;
#else // from SDL documentation
    Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

    return pixelColor;
}

// tile grid with pixels
bool Application::generateTileGrid(int tileWidth, int tileHeight) {
    int numberOfTilesWidth = std::floor(m_width / tileWidth);
    m_numberOfTilesInWidth = numberOfTilesWidth;

    int numberOfTilesHeight = std::floor(m_height / tileHeight);
    m_numberOfTilesInHeight = numberOfTilesHeight;

    // setup an SDL surface to generate each tile's texture
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface *temporarySurface = SDL_CreateRGBSurface(0, tileWidth, tileHeight, IMAGE_DEPTH, rmask, gmask, bmask, amask);

    // generate tiles
    for (int y=0; y < numberOfTilesHeight; ++y){
        for (int x=0; x < numberOfTilesWidth; ++x){
            App::TileInformation temporaryTile;
            temporaryTile.x = x * tileWidth;
            temporaryTile.y = y * tileHeight;
            temporaryTile.width = tileWidth;
            temporaryTile.height = tileHeight;
            temporaryTile.pTexture = SDL_CreateTextureFromSurface(pRenderer, temporarySurface);
            temporaryTile.rgb.resize(temporaryTile.width * temporaryTile.height);
            m_tiles.push_back(temporaryTile);
            // render complete
        }
    }

    // reset all tile flags
    for (int i=0; i<m_tiles.size(); ++i){
        m_tileFlags.push_back(new std::atomic<int> (TILE_NOT_RENDERED));
    }

    SDL_FreeSurface(temporarySurface);
    return true;
}

// destroys the grid
bool Application::destroyTileGrid() {
    for (int i = 0; i < m_tiles.size(); ++i) {
        if(m_tiles.at(i).pTexture != NULL){
            SDL_DestroyTexture(m_tiles.at(i).pTexture);
        }
    }
    return true;
}

// resets tile flags
void Application::resetTileFlags() {
    for (int i = 0; i < m_tiles.size(); ++i) {
        m_tileFlags.at(i)->store(TILE_NOT_RENDERED,std::memory_order_release);
        m_tiles.at(i).textureComplete = false;
    }
}

// render tile
void Application::renderTile(App::TileInformation *tile, std::atomic<int> *threadCounter, std::atomic<int> *tileFlag) {
    tileFlag->store(TILE_RENDERING,std::memory_order_release);
    m_scene.renderTile(tile);
    int numberOfActiveThreads = threadCounter->load(std::memory_order_acquire);
    threadCounter->store(numberOfActiveThreads-1,std::memory_order_release);
    tileFlag->store(TILE_RENDERED,std::memory_order_release);
}
