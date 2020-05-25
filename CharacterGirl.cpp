#include "CharacterGirl.h"

GLuint c_textureName = 0;
GLuint c_matName = 0;
int c_textureUnit = 0;
int c_matUnit = 0;
GLuint c_shader = 0;
GLuint c_vBuffer = 0;
mat4   c_transform;


void CharacterGirl::initialize(int id, const char *imageFile, const char *matFile) {

  c_textureUnit = 2*id;
  c_matUnit = c_textureUnit+1;
  glGenTextures(1, &c_textureName);
  c_textureName = LoadTexture(imageFile, c_textureUnit);
  glGenTextures(1, &c_matName);
  c_matName = LoadTexture(matFile, c_matUnit);
}

void CharacterGirl::SetTransform(mat4 m) {
  c_transform = m;
}

void CharacterGirl::Display() {

	//vec2 pts[] = vec2[4](vec2(-1,-1), vec2(-1,1), vec2(1,1), vec2(1,-1));
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
      pColor.a = texture(textureMat, uv).r;
    }
  )";

  if (!c_shader)
    c_shader = LinkProgramViaCode(&vShader, &pShader);
    glUseProgram(c_shader);
    glActiveTexture(GL_TEXTURE0+c_textureUnit);
    glBindTexture(GL_TEXTURE_2D, c_textureName);
    SetUniform(c_shader, "textureImage", c_textureUnit);
    SetUniform(c_shader, "textureMat", c_matUnit);
    SetUniform(c_shader, "useMat", c_matName > 0? 1 : 0);

    glActiveTexture(GL_TEXTURE0+c_matUnit);
    glBindTexture(GL_TEXTURE_2D, c_matName);

    SetUniform(c_shader, "view", c_transform);
    glDrawArrays(GL_QUADS, 0, 4);
}
