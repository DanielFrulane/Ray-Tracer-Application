#include "ColorMap.hpp"

App::Textures::ColorMap::ColorMap() {
    m_numberOfStops = 0;
}

App::Textures::ColorMap::~ColorMap() = default;

// creates a stop, given the position and color
void App::Textures::ColorMap::setStop(double position, const Vector3d &value) {
    m_stopPositions.push_back(position);
    m_stopValues.push_back(value);
    m_numberOfStops++;
}

// gets the color in the desired position
Vector3d App::Textures::ColorMap::getColor(double position) {
    // get the closest stops to the current position.
    int firstStop = 0;
    int secondStop = 0;
    double differenceInPosition = 2.0;
    for (int i=0; i < m_numberOfStops; ++i){
        double t = m_stopPositions.at(i) - position;
        if (abs(t) < differenceInPosition){
            differenceInPosition = abs(t);
            firstStop = i;
            // defines the final stops
            if (t < 0.0){
                secondStop = std::min(m_numberOfStops, (i + 1));
            } else if (t > 0.0){
                secondStop = std::max((i - 1), 0);
            } else {
                secondStop = i;
            }
        }
    }

    // case when the position is exactly at the stop
    if (firstStop == secondStop){
        return m_stopValues.at(firstStop);
    }

    // linear interpolation of the values between the two stops
    if (secondStop < firstStop) {
        std::swap(firstStop, secondStop);
    }
    // y = y0 + ((x - x0)*((y1 - y0)/(x1 - x0)))
    double x = position;
    double x0 = m_stopPositions.at(firstStop);
    double x1 = m_stopPositions.at(secondStop);
    return m_stopValues.at(firstStop) + (x - x0) * ((m_stopValues.at(secondStop) - m_stopValues.at(firstStop)) * (1.0 / (x1 - x0)));
}
