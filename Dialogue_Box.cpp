#include "Dialogue_Box.h"

	GLuint d_textureName = 0;
	GLuint d_matName = 0;
	int d_textureUnit = 0;
	int d_matUnit = 0;
	GLuint d_shader = 0;
	GLuint d_vBuffer = 0;
	mat4 d_transform;


	void Dialogue_Box::d_initialize(int id, const char *imageFile, const char *matFile) {

		d_textureUnit = 2 * id;
		d_matUnit = d_textureUnit + 1;
		glGenTextures(1, &d_textureName);
		d_textureName = LoadTexture(imageFile, d_textureUnit);
		glGenTextures(1, &d_matName);
		d_matName = LoadTexture(matFile, d_matUnit);
	}

	void Dialogue_Box::D_SetTransform(mat4 m) {
		d_transform = m;
	}

	void Dialogue_Box::D_Display() {
		const char *vShader = R"(
    #version 330
    uniform mat4 view;
    out vec2 uv;
    void main() {
      vec2 pts[] = vec2[4](vec2(-1,-1), vec2(-1,-0.5), vec2(1, -0.5), vec2(1, -1) );
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

		if (!d_shader)
			d_shader = LinkProgramViaCode(&vShader, &pShader);
		glUseProgram(d_shader);
		glActiveTexture(GL_TEXTURE0 + d_textureUnit);
		glBindTexture(GL_TEXTURE_2D, d_textureName);
		SetUniform(d_shader, "textureImage", d_textureUnit);
		SetUniform(d_shader, "textureMat", d_matUnit);
		SetUniform(d_shader, "useMat", d_matName > 0 ? 1 : 0);

		glActiveTexture(GL_TEXTURE0 + d_matUnit);
		glBindTexture(GL_TEXTURE_2D, d_matName);

		SetUniform(d_shader, "view", d_transform);
		glDrawArrays(GL_QUADS, 0, 4);
	}
