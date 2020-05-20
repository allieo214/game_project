#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"
#include "Background.h"
#include "Dialogue_Box.h"
#include "Character.h"

Background background;
Dialogue_Box dialogue;
Character character;
// display
int winW = 3000, winH = 1500;
//vec2 back_pts[4] = { vec2(-1,-1), vec2(-1,1), vec2(1,1), vec2(1,-1) };
//vec2 points[4] = { vec2(-1,-1), vec2(-1,1), vec2(-0.5, 1), vec2(-0.5, -1) };
//vec2 char_pts[4] = { vec2(-3, -3), vec2(-3,3), vec2(3, 3), vec2(3,-3) };

// interaction
float objectX = 0, objectY = 0, objectScale = 1;
float xDown = 0, yDown = 0, oldMouseX = 0, oldMouseY = 0;

// Mouse

void SetDialogueTransform() {
	mat4 trans = Translate(objectX, objectY, 0);
	mat4 scale = Scale(objectScale);
	dialogue.D_SetTransform(trans*scale);
};

/*void SetCharacterTransform() {
	mat4 trans = Translate(objectX, objectY, 0);
	mat4 scale = Scale(objectScale);
	character.SetTransform(trans*scale);
}
*/
int WindowHeight(GLFWwindow *w) {
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	return height;
}

void MouseWheel(GLFWwindow *w, double xoffset, double yoffset) {
	objectScale += .1f*(float)yoffset;
	SetDialogueTransform();
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
		SetDialogueTransform();
	}
}

// Application

void Resize(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, winW = width, winH = height);
}

int main(int ac, char **av) {
	//init app window and GL context;
	glfwInit();
	GLFWwindow *w = glfwCreateWindow(winW, winH, "background", NULL, NULL);
	glfwSetWindowPos(w, 100, 100);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	background.initialize(0, "C:/Users/allis/Graphics/Textures/sci_fi_background2.tga");
	//background.SetPoints();
	dialogue.d_initialize(1, "C:/Users/allis/Graphics/Textures/dialogue_test.tga", "C:/Users/allis/Graphics/Textures/dialogue_testMAT.tga");
	character.initialize(2, "C:/Users/allis/Graphics/Textures/character1.tga", "C:/Users/allis/Graphics/Textures/character1_MAT.tga");

	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		background.Display();
		character.Display();
		dialogue.D_Display();
		glFlush();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	// terminate
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	const GLuint names[] = { background.textureName, dialogue.d_textureName, character.c_textureName};
	glfwDestroyWindow(w);
	glfwTerminate();

}
