#include "RTImage.hpp"
#include <fstream>
#include <cstring>

App::RTImage::RTImage(){
    m_widthSize = 0;
    m_heightSize = 0;
    m_pTexture = NULL;
}

App::RTImage::~RTImage(){
    if (m_pTexture != NULL){
        SDL_DestroyTexture(m_pTexture);
    }
}


void App::RTImage::initialize(const int widthSize, const int heightSize, const int depthtSize, SDL_Renderer* pRenderer){
    // creating matrices
    m_redChannel.resize(widthSize, std::vector<double>(heightSize, 0.0));
    m_greenChannel.resize(widthSize, std::vector<double>(heightSize, 0.0));
    m_blueChannel.resize(widthSize, std::vector<double>(heightSize, 0.0));

    m_widthSize = widthSize;
    m_heightSize = heightSize;
    m_depthSize = depthtSize;
    m_pRenderer = pRenderer;

    initializeTexture();
}

// returns the image as SDL2 texture.
void App::RTImage::initializeTexture(){
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN // from SDL documentation
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else // from SDL documentation
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    if (m_pTexture != NULL){
        SDL_DestroyTexture(m_pTexture);
    }
    // creates texture that stores the image
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_widthSize, m_heightSize, m_depthSize, rmask, gmask, bmask, amask);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

// individual configuration of pixel in the display
void App::RTImage::setPixel(const int x, const int y, const double red, const double green, const double blue){
    m_redChannel.at(x).at(y) = red;
    m_greenChannel.at(x).at(y) = green;
    m_blueChannel.at(x).at(y) = blue;
}

// generation of what is going to be shown
void App::RTImage::display(){
    // allocates and clears memory for a pixel buffer that will go into SDL texture.
    Uint32 *tempPixels = new Uint32[m_widthSize * m_heightSize];
    memset(tempPixels, 0, m_widthSize * m_heightSize * sizeof(Uint32));

    for (int x=0; x < m_widthSize; ++x){
        for (int y=0; y < m_heightSize; ++y){
            tempPixels[(y * m_widthSize) + x] = convertColorFrom(m_redChannel.at(x).at(y), m_greenChannel.at(x).at(y),
                                                                 m_blueChannel.at(x).at(y));
        }
    }

    // turns buffer into SDL texture
    SDL_UpdateTexture(m_pTexture, NULL, tempPixels, m_widthSize * sizeof(Uint32));
    delete[] tempPixels;

    // copies texture to renderer.
    SDL_Rect srcRect, bounds;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = m_widthSize;
    srcRect.h = m_heightSize;
    bounds = srcRect;
    SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

// converts color to Uint32 from char conversion
Uint32 App::RTImage::convertColorFrom(const double red, const double green, const double blue){
    unsigned char r = static_cast<unsigned char>(red);
    unsigned char g = static_cast<unsigned char>(green);
    unsigned char b = static_cast<unsigned char>(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN // from SDL documentation
    Uint32 pixelColor = (b << 24) + (g << 16) + (r << 8) + 255;
#else // from SDL documentation
    Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

    return pixelColor;
}

int App::RTImage::getHeightSize() const {
    return m_heightSize;
}

int App::RTImage::getWidthSize() const {
    return m_widthSize;
}
