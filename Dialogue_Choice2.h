#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"

class Dialogue_Choice2{
  public:
    GLuint c2_textureName = 0;
    GLuint d2_matName = 0;
    int c2_textureUnit = 0;
    int c2_matUnit = 0;
    mat4 c2_transform;

    void c2_initialize(int id, const char *imageFile, const char *matFile);

    void c2_SetTransform(mat4 m);

    void c2_Display();
};
