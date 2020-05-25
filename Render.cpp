#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"
#include "Background.h"
#include "Dialogue_Box.h"
#include "CharacterGirl.h"
#include "Text.h"
#include <ft2build.h>


Background background;
Dialogue_Box dialogue;
CharacterGirl characterG;

struct conversationNode {

	std::string statement;
	std::string optionY;
	std::string optionN;
	vec3 color;
	conversationNode* y;
	conversationNode* n;
};

// display
int winW = 1920, winH = 1080;
//vec2 back_pts[4] = { vec2(-1,-1), vec2(-1,1), vec2(1,1), vec2(1,-1) };
//vec2 points[4] = { vec2(-1,-1), vec2(-1,1), vec2(-0.5, 1), vec2(-0.5, -1) };
//vec2 char_pts[4] = { vec2(-3, -3), vec2(-3,3), vec2(3, 3), vec2(3,-3) };

//Char Struct
//struct Character {
//	unsigned int TextureID;  // ID handle of the glyph texture
//	glm::ivec2   Size;       // Size of glyph
//	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
//	unsigned int Advance;    // Offset to advance to next glyph
//};


// interaction
float objectX = 0, objectY = 0, objectScale = 1;
float xDown = 0, yDown = 0, oldMouseX = 0, oldMouseY = 0;



void DisplayText(const char* fontName, const char* text, int y) {
	SetFont(fontName, 16, 60);  // exact affect of charRes, pixelRes unclear
	Text(30, y, vec3(0, 0, 0), 10, text);
}

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
	GLFWwindow *w = glfwCreateWindow(winW, winH, "Dating Sim!", NULL, NULL);
	glfwSetWindowPos(w, 0, 0);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	background.initialize(0, "sci_fi_background2.tga");
	//background.SetPoints();
	dialogue.d_initialize(1, "dialogue_TEST.tga", "dialogue_testMAT.tga");
	characterG.initialize(2, "character1.tga", "character1_MAT.tga");


	//FONT
	//FT_Library ft;
	//if (FT_Init_FreeType(&ft))
	//	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	//FT_Face face;
	//if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	//	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//FT_Set_Pixel_Sizes(face, 0, 48);

	//if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	//	std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;


	//std::map<char, Character> Characters;

	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		background.Display();
		characterG.Display();
		dialogue.D_Display();
		DisplayText("C:/Fonts/OpenSans/OpenSans-Bold.ttf", "Hi there! How are you doing?", 80);
		glFlush();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	//terminate
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	const GLuint names[] = { background.textureName, dialogue.d_textureName, characterG.c_textureName};
	glfwDestroyWindow(w);
	glfwTerminate();

}
