#ifndef RAY_TRACER_APPLICATION_OBJECTCOMPOSITION_HPP
#define RAY_TRACER_APPLICATION_OBJECTCOMPOSITION_HPP

#include "ObjectGeneric.hpp"
#include "ObjectCuboid.hpp"

namespace App {
    class ObjectComposition : public ObjectGeneric {
    public:
        ObjectCuboid m_boundingBox;
        bool m_isUsingBoundingBox;
        Vector2d m_xLimit;
        Vector2d m_yLimit;
        Vector2d m_zLimit;
        std::vector<std::shared_ptr<ObjectGeneric>> m_objects; // own list of objects

        ObjectComposition();
        ~ObjectComposition() override;

        void addObject(const std::shared_ptr<ObjectGeneric>& objectToAdd);

        void getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) override;
        bool isIntersecting(const Ray &castRay, HitInformation &hitInformation) override;

    private:
        // tests intersections with own list of objects
        std::shared_ptr<App::ObjectGeneric> isIntersecting(const Ray &rayCasted,
                                                            const Ray &backwardRay,
                                                            Vector3d &worldIntersectionPoint,
                                                            double &currentDistance,
                                                            HitInformation &hitInformationTemporary);
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTCOMPOSITION_HPP
