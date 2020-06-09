#include "Dialogue_Choice1.h"

	GLuint c1_textureName = 0;
	GLuint c1_matName = 0;
	int c1_textureUnit = 0;
	int c1_matUnit = 0;
	GLuint c1_shader = 0;
	GLuint c1_vBuffer = 0;
	mat4 c1_transform;


	void Dialogue_Choice1::c1_initialize(int id, const char *imageFile, const char *matFile) {

		c1_textureUnit = 2 * id;
		c1_matUnit = c1_textureUnit + 1;
		glGenTextures(1, &c1_textureName);
		c1_textureName = LoadTexture(imageFile, c1_textureUnit);
		glGenTextures(1, &c1_matName);
		c1_matName = LoadTexture(matFile, c1_matUnit);
	}

	void Dialogue_Choice1::c1_SetTransform(mat4 m) {
		c1_transform = m;
	}

	void Dialogue_Choice1::c1_Display() {
		const char *vShader = R"(
    #version 330
    uniform mat4 view;
    out vec2 uv;
    void main() {
      vec2 pts[] = vec2[4](vec2(-0.7,-0.2), vec2(-0.7,0.2), vec2(-0.2, 0.2), vec2(-0.2, -0.2) );
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

		if (!c1_shader)
			c1_shader = LinkProgramViaCode(&vShader, &pShader);
		glUseProgram(c1_shader);
		glActiveTexture(GL_TEXTURE0 + c1_textureUnit);
		glBindTexture(GL_TEXTURE_2D, c1_textureName);
		SetUniform(c1_shader, "textureImage", c1_textureUnit);
		SetUniform(c1_shader, "textureMat", c1_matUnit);
		SetUniform(c1_shader, "useMat", c1_matName > 0 ? 1 : 0);

		glActiveTexture(GL_TEXTURE0 + c1_matUnit);
		glBindTexture(GL_TEXTURE_2D, c1_matName);

		SetUniform(c1_shader, "view", c1_transform);
		glDrawArrays(GL_QUADS, 0, 4);
	}
