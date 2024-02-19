#include "Application.hpp"
#include "include/Eigen/Eigen"
#include <iostream>

#define SCREEN_WIDTH        720
#define SCREEN_HEIGHT       720
#define TILE_WIDTH          72
#define TILE_HEIGHT         72
#define IMAGE_DEPTH         32

Application::Application() {
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
    m_threadCounter = new std::atomic<int> (0);
    //m_scene = App::Scene(static_cast<double>(SCREEN_WIDTH) / static_cast<double>(SCREEN_HEIGHT));
}

// firstly runned code
bool Application::inInitialization() {
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){
        return false;
    }
    m_width = SCREEN_WIDTH;
    m_height = SCREEN_HEIGHT;
    pWindow = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (pWindow != NULL){
        // window initialization and image rendering
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        //m_image.initialize(SCREEN_WIDTH, SCREEN_HEIGHT, IMAGE_DEPTH, pRenderer);

        m_scene.m_height = m_width;
        m_scene.m_width = m_height;
        if (!generateTileGrid(TILE_WIDTH,TILE_HEIGHT)){
            std::cout<<"failed tile Grid"<<std::endl;
            return false;
        }
        m_maxThreads = 8; // store value 0 in the variable
        m_threadCounter ->store(0, std::memory_order_release);

        std::cout<<"number of tiles: "<<m_tiles.size()<<std::endl;
        SDL_SetRenderDrawColor(pRenderer, 255.0, 255.0, 255.0, 255.0);
        SDL_RenderClear(pRenderer);
        //m_scene.render(m_image);
        //m_image.display();
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
    std::cout<<"Done initializing\n";
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

// for SDL related events
void Application::inEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT){
        isRunning = false;
    }
}

void Application::inLoop() {
    /*for (int i = 0; i < m_tiles.size(); ++i) {
        if (m_tileFlags.at(i)->load(std::memory_order_acquire) == 0){
            m_scene.renderTile(&m_tiles.at(i));
            m_tileFlags.at(i)->store(2,std::memory_order_release);
            break;
        }
    }*/
    for (int i = 0; i < m_tiles.size(); ++i) {
        if (m_tileFlags.at(i)->load(std::memory_order_acquire) == 0){
            if (m_threadCounter ->load(std::memory_order_acquire) < m_maxThreads){
                int numberOfActiveThreads = m_threadCounter->load(std::memory_order_acquire);
                m_threadCounter->store(numberOfActiveThreads+1, std::memory_order_release);
                std::thread renderThread (&Application::renderTile, this, &m_tiles.at(i),m_threadCounter,m_tileFlags.at(i));
                renderThread.detach();
            }
        }
    }
}

// display image
void Application::inRender() {
    /*SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);
    m_scene.render(m_image);
    m_image.display();
    SDL_RenderPresent(pRenderer); // renders*/
    double widthFactor = 1.0;
    double heightFactor = 1.0;
    for (int i = 0; i < m_tiles.size(); ++i) {
        if(m_tileFlags.at(i)->load(std::memory_order_acquire)==2){
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

// quit
void Application::inExit() {
    bool result = destroyTileGrid();
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

void Application::convertImageToTexture(App::TileInformation &tile) {
    // Allocate memory for a pixel buffer.
    Uint32 *tempPixels = new Uint32[tile.width * tile.height];

    // Clear the pixel buffer.
    memset(tempPixels, 0, tile.width * tile.height * sizeof(Uint32));

    // Copy the image into tempPixels.
    for (int i=0; i<tile.width * tile.height; ++i){
        tempPixels[i] = convertColor(tile.rgb.at(i)(0), tile.rgb.at(i)(1), tile.rgb.at(i)(2));
    }

    // Update the texture with the pixel buffer.
    SDL_UpdateTexture(tile.pTexture, NULL, tempPixels, tile.width * sizeof(Uint32));

    // Delete the pixel buffer.
    delete[] tempPixels;
}

Uint32 Application::convertColor(const double red, const double green, const double blue) {
    // Convert the colours to unsigned integers.
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

bool Application::generateTileGrid(int tileWidth, int tileHeight) {
    // How many tiles will fit horizontally?
    int numTilesX = std::floor(m_width / tileWidth);
    m_numberOfTilesInWidth = numTilesX;

    // How many tiles will fit vertically?
    int numTilesY = std::floor(m_height / tileHeight);
    m_numberOfTilesInHeight = numTilesY;

    // Setup an SDL surface from which we can generate the textures for each tile.
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

    // Generate the actual tiles.
    for (int y=0; y<numTilesY; ++y){
        for (int x=0; x<numTilesX; ++x){
            App::TileInformation temporaryTile;
            temporaryTile.x = x * tileWidth;
            temporaryTile.y = y * tileHeight;
            temporaryTile.width = tileWidth;
            temporaryTile.height = tileHeight;
            temporaryTile.renderComplete = 0;
            temporaryTile.pTexture = SDL_CreateTextureFromSurface(pRenderer, temporarySurface);
            temporaryTile.rgb.resize(temporaryTile.width * temporaryTile.height);
            m_tiles.push_back(temporaryTile);
        }
    }

    // Set all the tile flags to zero.
    for (int i=0; i<m_tiles.size(); ++i){
        m_tileFlags.push_back(new std::atomic<int> (0));
    }

    // Tidy up before returning.
    SDL_FreeSurface(temporarySurface);
    return true;
}

bool Application::destroyTileGrid() {
    for (int i = 0; i < m_tiles.size(); ++i) {
        if(m_tiles.at(i).pTexture != NULL){
            SDL_DestroyTexture(m_tiles.at(i).pTexture);
        }
    }
    return true;
}

void Application::resetTileFlags() {
    for (int i = 0; i < m_tiles.size(); ++i) {
        m_tileFlags.at(i)->store(0,std::memory_order_release);
        m_tiles.at(i).textureComplete = false;
    }
}

void Application::renderTile(App::TileInformation *tile, std::atomic<int> *threadCounter, std::atomic<int> *tileFlag) {
    tileFlag->store(1,std::memory_order_release);
    m_scene.renderTile(tile);
    int numberOfActiveThreads = threadCounter->load(std::memory_order_acquire);
    threadCounter->store(numberOfActiveThreads-1,std::memory_order_release);
    tileFlag->store(2,std::memory_order_release);
}
