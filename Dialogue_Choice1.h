#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"

class Dialogue_Choice1{
  public:
    GLuint c1_textureName = 0;
    GLuint c1_matName = 0;
    int c1_textureUnit = 0;
    int c1_matUnit = 0;
    mat4 c1_transform;

    void c1_initialize(int id, const char *imageFile, const char *matFile);

    void c1_SetTransform(mat4 m);

    void c1_Display();
};
