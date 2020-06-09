#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "VecMat.h"
#include "Background.h"
#include "Dialogue_Box.h"
#include "CharacterGirl.h"
#include "Dialogue_Choice1.h"
#include "Dialogue_Choice2.h"
#include "Text.h"
#include <ft2build.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <conio.h>


Background background;
Dialogue_Box dialogue;
Dialogue_Choice1 choice1;
Dialogue_Choice2 choice2;
CharacterGirl characterG;
CharacterGirl upset;
CharacterGirl blush;

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

struct conversationNode* newNode(std::string dialogue, std::string dialogue_y, std::string dialogue_n, conversationNode* yes = NULL, conversationNode* no = NULL)
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
	if (str == "") {
		return new conversationNode("", "","", NULL, NULL);
	}

	if (str.at(index) == ';') {
		p = new conversationNode(str, "", "", NULL, NULL);
		return p;
	}

	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		s.push(token);
		str.erase(0, pos + delimiter.length());
	}
	
	no = s.top();
	s.pop();
	yes = s.top();
	s.pop();
	main = s.top();
	s.pop();

	conversationNode* yesNode = create(f);
	conversationNode* noNode = create(f);
	p = new conversationNode(main, yes, no, yesNode, noNode);

	return p;
}
conversationNode* loadDialogue(std::string fn) {
	std::string line;
	std::ifstream file(fn);
	
	conversationNode *root;
	if(file.is_open()){
		root = create(file);
	
	}
	else {
		root = NULL;
	}

	return root;
}

// display
int winW = 1280, winH = 720;

// interaction
float objectX = 0, objectY = 0, objectScale = 1;
float xDown = 0, yDown = 0, oldMouseX = 0, oldMouseY = 0;



void DisplayText(const char* fontName, const char* text, int y, int x) {
	SetFont(fontName, 16, 60);  // exact affect of charRes, pixelRes unclear
	Text(x, y, vec3(0, 0, 0), 10, text);
}

// Mouse

void SetDialogueTransform() {
	mat4 trans = Translate(objectX, objectY, 0);
	mat4 scale = Scale(objectScale);
	dialogue.D_SetTransform(trans*scale);
};

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
	
	conversationNode* yes;
	conversationNode* no;
	conversationNode* curr = loadDialogue("intro.txt");
	std::cout << curr->statement << std::endl;
	std::cout << curr->optionY << std::endl;
	std::cout << curr->optionN << std::endl;
	no = curr->n;
	yes = curr->y;
	std::cout << "YES BRANCH" << std::endl;
	std::cout << yes->statement << std::endl;
	std::cout << yes->optionY << std::endl;
	std::cout << yes->y->statement << std::endl;
	std::cout << yes->y->optionY<< std::endl;
	std::cout << yes->y->y->statement << std::endl;
	std::cout << yes->y->optionN << std::endl;
	std::cout << yes->y->n->statement << std::endl;
	std::cout << yes->optionN << std::endl;
	std::cout << yes->n->statement << std::endl;
	std::cout << yes->n->optionY << std::endl;
	std::cout << yes->n->y->statement << std::endl;
	std::cout << yes->n->optionN << std::endl;
	std::cout << yes->n->n->statement << std::endl;
	std::cout << "NO BRANCH" << std::endl;
	std::cout << no->statement << std::endl;
	
	glfwInit();
	GLFWwindow *w = glfwCreateWindow(winW, winH, "Dating Sim!", NULL, NULL);
	glfwSetWindowPos(w, 0, 0);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	background.initialize(0, "sci_fi_background2.tga");

	dialogue.d_initialize(1, "dialogue_TEST.tga", "dialogue_testMAT.tga");
	characterG.initialize(2, "charactergirl1_neutral.tga", "character1_MAT.tga");
	upset.initialize(3, "charactergirl1_upset.tga", "character1_MAT.tga");
	blush.initialize(4, "charactergirl1_blushing.tga", "character1_MAT.tga");
	choice1.c1_initialize(5, "dialogue_TEST.tga", "dialogue_testMat.tga");
	choice2.c2_initialize(6, "dialogue_TEST.tga", "dialogue_testMAT.tga");


	glfwSwapInterval(1);

	bool interaction = false;
	char ch = 'a'; 
	char playerChoice = 'y';
	CharacterGirl c = characterG;
	while (!glfwWindowShouldClose(w)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		background.Display();

		/*if (playerChoice == 'Y' || playerChoice == 'y')
			blush.Display();
		else if (playerChoice == 'N' || playerChoice == 'n')
			upset.Display();
		else {
			characterG.Display();
		}*/
		
		c.Display();
		dialogue.D_Display();
		DisplayText("C:/Fonts/OpenSans/OpenSans-Bold.ttf", curr->statement.c_str(), 80, 30);


		if (interaction == true){
			if (curr->y != NULL){
				//display choices
				std::cout << "Entered Loop" << std::endl;
				choice1.c1_Display();
				choice2.c2_Display();

				
				DisplayText("C:/Fonts/OpenSans/OpenSans-Bold.ttf", curr->optionY.c_str(), 360, 310);//yes option
				DisplayText("C:/Fonts/OpenSans/OpenSans-Bold.ttf", curr->optionN.c_str(), 360, 830);//no option

				glFlush();
				glfwSwapBuffers(w);
				glfwPollEvents();

				playerChoice = _getch();
				if (playerChoice == 'y' || playerChoice == 'Y') {
					std::cout << "Entered Y" << std::endl;
					c = blush;
					curr = curr->y;
				}
				else if (playerChoice == 'n' || playerChoice == 'N') {
					std::cout << "Entered N" << std::endl;
					c = upset;
					curr = curr->n;
				}
			}
		}
		else {
			glFlush();
			glfwSwapBuffers(w);
			glfwPollEvents();
		}

		if (ch != 'e' && (playerChoice == 'y' || playerChoice == 'Y' || playerChoice == 'n' || playerChoice == 'N')){
			ch = _getch();

			if (ch == 'e') {
				interaction = true;
			}
		}
		else {
			interaction = false;
			ch = 's';
		}

		std::cout << playerChoice << " - " << interaction << std::endl;

	}
	//terminate
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	const GLuint names[] = { background.textureName, dialogue.d_textureName, characterG.c_textureName, choice1.c1_textureName, choice2.c2_textureName };
	glfwDestroyWindow(w);
	glfwTerminate();

}