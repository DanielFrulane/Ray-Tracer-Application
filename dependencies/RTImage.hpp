#ifndef RAY_TRACER_APPLICATION_RTIMAGE_HPP
#define RAY_TRACER_APPLICATION_RTIMAGE_HPP

#include <vector>
#include <string>
#include "../include/SDL2/SDL.h"
#include "../include/Eigen/Eigen"

using namespace Eigen;

namespace App {
    // image class to be rendered
    class RTImage {
    private:
        // matrix for 3 different channels of colors
        std::vector<std::vector<double>> m_redChannel;
        std::vector<std::vector<double>> m_greenChannel;
        std::vector<std::vector<double>> m_blueChannel;
        int m_widthSize, m_heightSize, m_depthSize;
        double m_maxRed, m_maxGreen, m_maxBlue, m_maxOverall;

    public:
        RTImage();
        ~RTImage();
        void initialize(const int widthSize, const int heightSize, const int depthtSize, SDL_Renderer *pRenderer);
        void setPixel(const int x, const int y, const double red, const double green, const double blue);
        void setPixel(const int x, const int y, const Vector3d rgb);
        void display();

        int getHeightSize() const;
        int getWidthSize() const;

    private:
        Uint32 convertColorFrom(const double red, const double green, const double blue);
        void initializeTexture();
        SDL_Renderer *m_pRenderer{};
        SDL_Texture *m_pTexture;
        void calculateMaximumValues();
    };
}

#endif //RAY_TRACER_APPLICATION_RTIMAGE_HPP
