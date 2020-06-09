#include "Dialogue_Choice2.h"

	GLuint c2_textureName = 0;
	GLuint c2_matName = 0;
	int c2_textureUnit = 0;
	int c2_matUnit = 0;
	GLuint c2_shader = 0;
	GLuint c2_vBuffer = 0;
	mat4 c2_transform;


	void Dialogue_Choice2::c2_initialize(int id, const char *imageFile, const char *matFile) {

		c2_textureUnit = 2 * id;
	  c2_matUnit = c2_textureUnit + 1;
		glGenTextures(1, &c2_textureName);
		c2_textureName = LoadTexture(imageFile, c2_textureUnit);
		glGenTextures(1, &c2_matName);
		c2_matName = LoadTexture(matFile, c2_matUnit);
	}

	void Dialogue_Choice2::c2_SetTransform(mat4 m) {
		c2_transform = m;
	}

	void Dialogue_Choice2::c2_Display() {
		const char *vShader = R"(
    #version 330
    uniform mat4 view;
    out vec2 uv;
    void main() {
      vec2 pts[] = vec2[4]( vec2(0.2, -0.2), vec2(0.2, 0.2), vec2(0.7,0.2), vec2(0.7,-0.2) );
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

		if (!c2_shader)
			c2_shader = LinkProgramViaCode(&vShader, &pShader);
		glUseProgram(c2_shader);
		glActiveTexture(GL_TEXTURE0 + c2_textureUnit);
		glBindTexture(GL_TEXTURE_2D, c2_textureName);
		SetUniform(c2_shader, "textureImage", c2_textureUnit);
		SetUniform(c2_shader, "textureMat", c2_matUnit);
		SetUniform(c2_shader, "useMat", c2_matName > 0 ? 1 : 0);

		glActiveTexture(GL_TEXTURE0 + c2_matUnit);
		glBindTexture(GL_TEXTURE_2D, c2_matName);

		SetUniform(c2_shader, "view", c2_transform);
		glDrawArrays(GL_QUADS, 0, 4);
	}
