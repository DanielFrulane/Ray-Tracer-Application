#ifndef RAY_TRACER_APPLICATION_RTIMAGE_HPP
#define RAY_TRACER_APPLICATION_RTIMAGE_HPP

#include <vector>
#include <string>
#include "../include/SDL2/SDL.h"

namespace App {
    // image class to be rendered
    class RTImage {
    private:
        // matrix for 3 different channels of colors
        std::vector<std::vector<double>> m_redChannel;
        std::vector<std::vector<double>> m_greenChannel;
        std::vector<std::vector<double>> m_blueChannel;
        int m_widthSize, m_heightSize, m_depthSize;

    public:
        RTImage();
        ~RTImage();
        void initialize(const int widthSize, const int heightSize, const int depthtSize, SDL_Renderer *pRenderer);
        void setPixel(const int x, const int y, const double red, const double green, const double blue);
        void display();

        int getHeightSize() const;
        int getWidthSize() const;

    private:
        Uint32 convertColorFrom(const double red, const double green, const double blue);
        void initializeTexture();
        SDL_Renderer *m_pRenderer{};
        SDL_Texture *m_pTexture;
    };
}

#endif //RAY_TRACER_APPLICATION_RTIMAGE_HPP
