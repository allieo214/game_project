#include "Background.h"

GLuint textureName = 0;
GLuint matName = 0;
int textureUnit = 0;
int matUnit = 0;
GLuint shader = 0;
GLuint vBuffer = 0;
mat4   transform;

void Background::initialize(int id, const char *imageFile) {

  textureUnit = 2*id;
  matUnit = textureUnit+1;
  glGenTextures(1, &textureName);
  textureName = LoadTexture(imageFile, textureUnit);
}

void Background::SetTransform(mat4 m) {
  transform = m;
}

void Background::Display() {
  const char *vShader = R"(
    #version 330
    uniform mat4 view;
    out vec2 uv;
    void main() {
      vec2 pts[] = vec2[4](vec2(-1,-1), vec2(-1,1), vec2(1,1), vec2(1,-1));
      uv = (pts[2]+pts[gl_VertexID])/2;
      gl_Position = view*vec4(pts[gl_VertexID], 0, 1);
    }
  )";

  const char *pShader = R"(
    #version 330
    in vec2 uv;
    out vec4 pColor;
    uniform sampler2D textureImage;
    uniform sampler2D textureMat;
    uniform int useMat = 0;
    void main() {
      pColor = vec4(texture(textureImage, uv).rgb, 1);
    }
  )";

  if (!shader)
    shader = LinkProgramViaCode(&vShader, &pShader);
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0+textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureName);
  SetUniform(shader, "textureImage", textureUnit);
  SetUniform(shader, "textureMat", matUnit);
  SetUniform(shader, "useMat", matName > 0? 1 : 0);
  SetUniform(shader, "view", transform);
  glDrawArrays(GL_QUADS, 0, 4);
}
