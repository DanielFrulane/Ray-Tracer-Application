#include "ColorMap.hpp"

App::Textures::ColorMap::ColorMap() {

}

App::Textures::ColorMap::~ColorMap() {

}

void App::Textures::ColorMap::setStop(double position, const Vector3d &value) {
    m_stopPositions.push_back(position);
    m_stopValues.push_back(value);
}

Vector3d App::Textures::ColorMap::getColor(double position) {
    // Find the closest stops to the current position.
    int numberOfStops = m_stopPositions.size();
    int firstStop = 0;
    int secondStop = 0;
    double differenceInPosition = 2.0;
    for (int i=0; i < numberOfStops; ++i){
        double t = m_stopPositions.at(i) - position;
        if (fabs(t) < differenceInPosition){
            differenceInPosition = fabs(t);
            firstStop = i;
            if (t < 0.0){
                secondStop = std::min(numberOfStops, (i + 1));
            } else if (t > 0.0){
                secondStop = std::max((i - 1), 0);
            } else {
                secondStop = i;
            }
        }
    }

    // If the position was exactly at a stop, we simply return that value.
    if (firstStop == secondStop){
        return m_stopValues.at(firstStop);
    }

    // Otherwise we need to interpolate between the first and second stops.
    // Make sure the stops are in the right order.
    if (secondStop < firstStop){
        std::swap(firstStop, secondStop);
    }

    // Perform linear interpolation of the values between the two stops.
    // y0 + ((x - x0)*((y1 - y0)/(x1 - x0)))
    double x = position;
    double x0 = m_stopPositions.at(firstStop);
    double x1 = m_stopPositions.at(secondStop);
    return m_stopValues.at(firstStop) + (x - x0) * ((m_stopValues.at(secondStop) - m_stopValues.at(firstStop)) * (1.0 / (x1 - x0)));
}
