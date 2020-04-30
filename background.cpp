#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"


// display
GLuint textureNames[3] = { 0, 1, 2 };
int textureUnits[3] = { 0, 1, 2 };
int winW = 3000;
int winH = 1500;

//shaders
GLuint backShader = 0;
GLuint foreShader = 0;
GLuint vBuffer = 0;
vec2 points[] = { vec2(-1,-1.5), vec2(-1,1.5), vec2(1,1.5), vec2(1,-1.5) };
vec2 uvs[] = { vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,1) };

// interactions

float objectX = 0, objectY = 0, objectScale = 1;
float xDown = 0, yDown = 0, oldMouseX = 0, oldMouseY = 0;

void DisplayBackground() {

	const char *vShader = R"(
        #version 330
        out vec2 uv;
        void main() {
            vec2 pts[] = vec2[4](vec2(-1,-1), vec2(-1,1), vec2(1,1), vec2(1,-1));
            uv = (pts[2]+pts[gl_VertexID])/2;
            gl_Position = vec4(pts[gl_VertexID], 0, 1);
        }
    )";

	const char *pShader = R"(
        #version 330
        in vec2 uv;
        out vec4 pColor;
        uniform sampler2D textureImage;
        void main() {
            pColor = texture(textureImage, uv);
        }
    )";

	if (!backShader)
		backShader = LinkProgramViaCode(&vShader, &pShader);
	glUseProgram(backShader);
	glActiveTexture(GL_TEXTURE0 + textureUnits[0]);
	glBindTexture(GL_TEXTURE_2D, textureNames[0]);
	SetUniform(backShader, "textureImage", textureUnits[0]);
	glDrawArrays(GL_QUADS, 0, 4);
}


void DisplayForeground() {

	const char *vShader = R"(
        #version 330
		in vec2 point;
		in vec2 uv;
		uniform mat4 view;
        out vec2 vUv;
        void main() {
            vUv = uv;
            gl_Position = view*vec4(point, 1, 1.25);
        }
    )";
	const char *pShader = R"(
        #version 330
        in vec2 vUv;
        out vec4 pColor;
        uniform sampler2D textureImage;
        uniform sampler2D textureMat;
        void main() {
            pColor = texture(textureImage, vUv);
			pColor.a = texture(textureMat, vUv).r;
        }
    )";

	if (!foreShader)
		foreShader = LinkProgramViaCode(&vShader, &pShader);
	glUseProgram(foreShader);
	glActiveTexture(GL_TEXTURE0 + textureUnits[1]);
	glBindTexture(GL_TEXTURE_2D, textureNames[1]);
	SetUniform(foreShader, "textureImage", textureUnits[1]);
	glActiveTexture(GL_TEXTURE0 + textureUnits[2]);
	glBindTexture(GL_TEXTURE_2D, textureNames[2]);
	SetUniform(foreShader, "textureMat", textureUnits[2]);
	VertexAttribPointer(foreShader, "point", 2, 0, (void *)0);
	VertexAttribPointer(foreShader, "uv", 2, 0, (void *) sizeof(points));
	mat4 trans = Translate(objectX, objectY, 0);
	mat4 rot = RotateZ(90);
	mat4 scale = Scale(objectScale);
	SetUniform(foreShader, "view", trans*rot*scale);

	glDrawArrays(GL_QUADS, 0, 4);
}

// Mouse

int WindowHeight(GLFWwindow *w) {
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	return height;
}

void MouseWheel(GLFWwindow *w, double xoffset, double yoffset) {
	objectScale += .1f*(float)yoffset;
}

void MouseButton(GLFWwindow *w, int butn, int action, int mods) {
	double x, y;
	glfwGetCursorPos(w, &x, &y);
	y = WindowHeight(w) - y; // invert y for upward-increasing screen space
	if (action == GLFW_PRESS) {
		xDown = (float)x;
		yDown = (float)y;
	}
	else {
		oldMouseX = objectX;
		oldMouseY = objectY;
	}
}

void MouseMove(GLFWwindow *w, double x, double y) {
	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { // drag
		y = WindowHeight(w) - y;
		objectX = oldMouseX + ((float)x - xDown) / winW;
		objectY = oldMouseY + ((float)y - yDown) / winH;
	}
}

// Application

void Resize(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, winW = width, winH = height);
}

void InitVertexBuffer() {
	// define GPU buffer, make it active
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// allocate and fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(uvs), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(uvs), uvs);
}

int main(int ac, char **av) {
	// init app window and GL context
	glfwInit();
	GLFWwindow *w = glfwCreateWindow(winW, winH, "Background", NULL, NULL);
	glfwSetWindowPos(w, 100, 100);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// read background, foreground, and mat textures
	glGenTextures(1, textureNames);
	textureNames[0] = LoadTexture("C:/Users/allis/Graphics/Textures/background1.tga", textureUnits[0]);
	textureNames[1] = LoadTexture("C:/Users/allis/Graphics/Textures/dialogue_box3.tga", textureUnits[1]);
	textureNames[2] = LoadTexture("C:/Users/allis/Graphics/Textures/dialogue_box3.tga", textureUnits[2]);
	InitVertexBuffer();
	/*
	textureNames[0] = LoadTexture("C:/Users/jules/SeattleUniversity/Exe/Earth.tga", textureUnits[0]);
	textureNames[1] = LoadTexture("C:/Users/jules/SeattleUniversity/Exe/Lily.tga", textureUnits[1]);
	textureNames[2] = LoadTexture("C:/Users/jules/SeattleUniversity/Exe/Mat.tga", textureUnits[2]);
	InitVertexBuffer();*/

	// callbacks
	/*glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetCursorPosCallback(w, MouseMove);
	glfwSetScrollCallback(w, MouseWheel);
	glfwSetWindowSizeCallback(w, Resize);*/

	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DisplayBackground();
		DisplayForeground();
		glFlush();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(3, textureNames);
	glfwDestroyWindow(w);
	glfwTerminate();
}





/*float backCoords[] = {
	0.0f, 0.0f, //bottom left
	1.0f, 0.0f, //bottom right
	0.0f, 1.0f, //top left
	1.0f, 1.0f, //top right
}; 

vec2 texcoord;
vec3 Color;
vec2 Texcoord;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	Texcoord = texcoord;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(512, 512, "Background", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glViewport(0, 0, 512, 512);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//framebuffer_size_callback(window, 512, 512);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backCoords), backCoords, GL_STATIC_DRAW);



	int width, height;
	unsigned char* image = SOIL_load_image("background1.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	

	//unsigned char* image = SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//render stuff


		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}*/