#ifndef RAY_TRACER_APPLICATION_SCENEBASIC_HPP
#define RAY_TRACER_APPLICATION_SCENEBASIC_HPP

// this is a test version for generating a scene and is not used for the final application

#include "SceneGeneric.hpp"

namespace App {
    class SceneBasic : public SceneGeneric{
    public:
        SceneBasic();
        ~SceneBasic() override;
        void generateSceneObjects() override;
    };
}
#endif //RAY_TRACER_APPLICATION_SCENEBASIC_HPP
