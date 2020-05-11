#pragma once
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"

class Character{
  public:
    GLuint c_textureName = 0;
    GLuint c_matName = 0;
    int c_textureUnit = 0;
    int c_matUnit = 0;
    GLuint c_shader = 0;
    GLuint c_vBuffer = 0;
    mat4   c_transform;

    void initialize(int id, const char *imageFile, const char *matFile);

    void SetTransform(mat4 m);

    void Display();
};
