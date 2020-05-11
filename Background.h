#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"

class Background{
  public:
    GLuint textureName = 0;
    GLuint matName = 0;
    int textureUnit = 0;
    int matUnit = 0;
    GLuint shader = 0;
    GLuint vBuffer = 0;
    mat4   transform;

    void initialize(int id, const char *imageFile);

    void SetTransform(mat4 m);

    void Display();
};
