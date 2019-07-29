#ifndef _BASIC_GL_OBJ_H_
#define _BASIC_GL_OBJ_H_

#include <string>
#include <vector>
#include <map>

namespace BasicGL
{
    typedef struct 
    {
        unsigned int vb_id;
        int numTriangles;
        size_t material_id;
    } DrawObject;

    struct vec3 
    {
        float v[3];
        vec3() 
        {
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = 0.0f;
        }
    };

    class OBJ
    {
        private:
            bool loaded;
            float normalizedPosition[3];
            float normalizedScale;
            std::vector<DrawObject> drawObjects;
            std::map<std::string, unsigned int> textures;
            int materialsId;
            static std::string GetBaseDir(const std::string& filepath);
            static bool FileExists(const std::string& abs_filename);
        public:
            OBJ();
            virtual ~OBJ();

            bool available();
            bool load(const std::string& fileName);
            void free();

            void draw(bool wireframe);
    };
}

#endif
