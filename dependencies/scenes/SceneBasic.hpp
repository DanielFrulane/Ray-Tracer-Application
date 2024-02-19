#ifndef RAY_TRACER_APPLICATION_SCENEBASIC_HPP
#define RAY_TRACER_APPLICATION_SCENEBASIC_HPP

#include "SceneGeneric.hpp"

namespace App {
    class SceneBasic : public SceneGeneric{
    public:
        SceneBasic();
        virtual ~SceneBasic() override;
        virtual void generateSceneObjects() override;
    };
}
#endif //RAY_TRACER_APPLICATION_SCENEBASIC_HPP
