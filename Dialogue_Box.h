#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"

class Dialogue_Box{
  public:
    GLuint d_textureName = 0;
    GLuint d_matName = 0;
    int d_textureUnit = 0;
    int d_matUnit = 0;
    mat4 d_transform;
    
    void d_initialize(int id, const char *imageFile, const char *matFile);

    void D_SetTransform(mat4 m);

    void D_Display();
};
