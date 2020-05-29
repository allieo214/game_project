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
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

Background background;
Dialogue_Box dialogue;
CharacterGirl characterG;

struct conversationNode {

	std::string statement;
	std::string optionY;
	std::string optionN;
	vec3 color; // 000 initalization
	conversationNode* y;
	conversationNode* n;
	
	conversationNode(std::string main, std::string yes, std::string no, conversationNode* yesNode = NULL, conversationNode* noNode = NULL) {
		statement = main;
		optionY = yes;
		optionN = no;
		color = { 0, 0 ,0 };
		y = yesNode;
		n = noNode;
	}
};

struct conversationNode* newNode(std::string dialogue, std::string dialogue_y = "", std::string dialogue_n = "", conversationNode* yes = NULL, conversationNode* no = NULL)
{
	// Allocate memory for new node  
	struct conversationNode* node = (struct conversationNode*)malloc(sizeof(struct conversationNode));

	// Assign data to this node 
	node->statement = dialogue;
	node->optionY = dialogue_y;
	node->optionN = dialogue_n;

	node->color = { 0, 0 ,0 };

	// Initialize left and right children as NULL 
	node->y = yes;
	node->n = no;
	return(node);
}

conversationNode *create(std::ifstream &f)
{
	conversationNode *p;
	std::string main;
	std::string yes;
	std::string no;
	std::string delimiter = "\\";
	std::stack<std::string> s;
	
	std::string str;
	std::getline(f, str);

	std::cout << str << std::endl;

	int index = str.length() - 1;
	if (str.at(index) == ';') {
		p = new conversationNode(main, "", "", NULL, NULL);
		return p;
	}

	//p = (conversationNode*)malloc(sizeof(conversationNode));
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		s.push(token);
		str.erase(0, pos + delimiter.length());
	}
	
	no = s.top();
	//std::cout << no << std::endl;
	s.pop();
	yes = s.top();
	//std::cout << yes << std::endl;
	s.pop();
	main = s.top();
	s.pop();
	//std::cout << main << std::endl;

	conversationNode* yesNode = create(f);
	conversationNode* noNode = create(f);
	p = new conversationNode(main, yes, no, yesNode, noNode);
	//f.close();
	/*p->statement = main;
	p->optionN = no;
	p->optionY = yes;*/

	//p->y = create(f);
	//p->n = create(f);

	return p;
}
void loadDialogue(std::string fn) {
	std::string line;
	std::ifstream file(fn);
	
	conversationNode *root;
	if(file.is_open()){
		root = create(file);
	
	}
}

// display
int winW = 1920, winH = 1080;

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

int main() {

	std::string end;
	loadDialogue("example.txt");
	std::cin >> end;
}