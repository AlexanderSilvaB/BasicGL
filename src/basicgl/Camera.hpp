#ifndef _BASIC_GL_CAMERA_H_
#define _BASIC_GL_CAMERA_H_

namespace BasicGL
{
    class Camera
    {
        public:
            float rotation[3];
            float position[3];
            float translation[3];
        
            Camera();
            virtual ~Camera();
            
            Camera& translate(float x = 0, float y = 0, float z = 0);
            Camera& moveTo(float x = 0, float y = 0, float z = 0);
            Camera& rotate(float x = 0, float y = 0, float z = 0);
            Camera& rotateTo(float x = 0, float y = 0, float z = 0);
    };
}

#endif
