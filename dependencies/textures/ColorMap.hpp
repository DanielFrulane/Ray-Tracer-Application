#ifndef RAY_TRACER_APPLICATION_COLORMAP_HPP
#define RAY_TRACER_APPLICATION_COLORMAP_HPP

#include "../../include/Eigen/Eigen"

using namespace Eigen;

// the color map converts linear information about colors (position) to represent them in the objects
namespace App::Textures {
    class ColorMap {
    private:
        std::vector<double> m_stopPositions;
        std::vector<Vector3d> m_stopValues;
        int m_numberOfStops;
    public:
        ColorMap();
        ~ColorMap();

        void setStop(double position, const Vector3d &value);
        Vector3d getColor(double position);
    };
}

#endif //RAY_TRACER_APPLICATION_COLORMAP_HPP
