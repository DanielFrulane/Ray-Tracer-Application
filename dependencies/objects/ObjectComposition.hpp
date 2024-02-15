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
        void addObject(std::shared_ptr<ObjectGeneric> subShape);

        // Override the GetExtents function.
        virtual void getExtents(Vector2d &xLim, Vector2d &yLim, Vector2d &zLim) override;

        // Override the function to test for intersections.
        bool isIntersecting(const Ray &castRay,  Vector3d &intersectionPoint, Vector3d &localNormal, Vector3d &localColor) override;

        // Function to update the bounds after sub-shapes have been modified.
        void updateBounds();

    private:
        // Test for intersections with the list of sub-objects.
        virtual std::shared_ptr<ObjectGeneric> isIntersecting(const Ray &rayCasted,
                                    const Ray &backwardRay,
                                    Vector3d &intersectionPoint,
                                    double &currentDistance,
                                    //qbRT::DATA::hitData &hitData);
                                    Vector3d &localNormal, Vector3d &localColor);

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
