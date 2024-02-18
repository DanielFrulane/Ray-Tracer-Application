#ifndef RAY_TRACER_APPLICATION_OBJECTCOMPOSITION_HPP
#define RAY_TRACER_APPLICATION_OBJECTCOMPOSITION_HPP

#include "ObjectGeneric.hpp"
#include "ObjectCuboid.hpp"

namespace App {
    class ObjectComposition : public ObjectGeneric {
    public:
        // Default constructor.
        ObjectComposition();

        // The destructor.
        virtual ~ObjectComposition() override;

        // Function to add a sub-shape.
        void addObject(const std::shared_ptr<ObjectGeneric>& objectToAdd);

        // Override the GetExtents function.
        virtual void getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) override;

        // Override the function to test for intersections.
        bool isIntersecting(const Ray &castRay, HitInformation &hitInformation) override;

        // Function to update the bounds after sub-shapes have been modified.
        void updateBounds();

    private:
        // Test for intersections with the list of sub-objects.
        std::shared_ptr<App::ObjectGeneric> isIntersecting(const Ray &rayCasted,
                            const Ray &backwardRay,
                            Vector3d &worldIntersectionPoint,
                            double &currentDistance,
                            HitInformation &hitInformationTemporary);

    public:
        // Bounding box.
        ObjectCuboid m_boundingBox = ObjectCuboid();
        bool m_isUsingBoundingBox = true;

        // List of sub-objects.
        std::vector<std::shared_ptr<ObjectGeneric>> m_objects;

        // Object limits.
        Vector2d m_xLimit;
        Vector2d m_yLimit;
        Vector2d m_zLimit;
    };
}

#endif //RAY_TRACER_APPLICATION_OBJECTCOMPOSITION_HPP
