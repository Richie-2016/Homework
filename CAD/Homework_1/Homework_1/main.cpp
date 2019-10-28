#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

const int WIDTH = 1680, HEIGHT = 1050;
const int V = 1009, F = 2022;
const float camSpdRat = 1.5;
float instance = 0.0f, prvFrame = 0.0f, camSpd = 0.0f;

glm::vec4 lineColor(1.0f, 1.0f, 1.0f, 1.0f);
GLfloat faceColorful[F * 3], faceColor[F * 3];
GLfloat* faceColorptr = faceColor;
bool faceIsColorful = GL_FALSE;

bool isPressed[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void changeLineColor(int redMode, int greenMode, int blueMode);
void changeFaceColor(int redMode, int greenMode, int blueMode);
void genColorfulFace();

int renderMode = 0;
void render(int renderMode, GLuint* VBO, GLuint* VAO, Shader& myShader, GLfloat* allFaceColor, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec4& lineColor);

Camera myCam(glm::vec3(0.0f, 0.0f, 3.0f));

int main() {
	/* Read eight.uniform.obj */
	GLfloat v[V * 3], f[F * 9];
	int curv = 0, curf = 0;
	std::ifstream data("eight.uniform.obj");
	while (!data.eof()) {
		std::stringstream buff;
		std::string str, tmp;
		std::getline(data, str);
		buff << str;
		if (str[0] == 'v') {
			/* This line is describing a vertice */
			
			std::getline(buff, tmp, ' ');
			buff >> v[curv++] >> v[curv++] >> v[curv++];
		}
		else if (str[0] == 'f') {
			/* This line is describing a face */
			
			std::getline(buff, tmp, ' ');
			int tmpv[3];
			buff >> tmpv[0] >> tmpv[1] >> tmpv[2];
			for (int i = 0; i != 3; i++) {
				for (int j = 0; j != 3; j++) {
					f[curf++] = v[(tmpv[i] - 1) * 3 + j];
				}
			}
		}
		buff.clear();
	}
	/* glfwInit */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/*Create the window*/
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Homework_1 by Richie 2016011846", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/* gladInit */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	/* Create myShader */
	Shader myShader("main.vert.glsl", "main.frag.glsl");
	GLuint VBO, VAO;

	/* Bind VBO & VAO */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f), f, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	for (int i = 0; i != F * 3; i++) {
		faceColor[i] = 1.0f;
	}

	/* Loop until the window closed */
	while (!glfwWindowShouldClose(window)) {
		
		GLfloat curFrame = (GLfloat)glfwGetTime();
		instance = curFrame - prvFrame;
		prvFrame = curFrame;
		camSpd = camSpdRat * instance;
		
		/* Poll for and process events */
		glfwPollEvents();
		
		glEnable(GL_DEPTH_TEST);

		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1), view(1), projection(1);
		view = myCam.GetViewMatrix();
		projection = glm::perspective(glm::radians(myCam.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 10.0f);
		
		if (isPressed[GLFW_KEY_R]) changeLineColor(1, 0, 0);
		if (isPressed[GLFW_KEY_G]) changeLineColor(0, 1, 0);
		if (isPressed[GLFW_KEY_B]) changeLineColor(0, 0, 1);
		if (isPressed[GLFW_KEY_E]) changeLineColor(-1, 0, 0);
		if (isPressed[GLFW_KEY_F]) changeLineColor(0, -1, 0);
		if (isPressed[GLFW_KEY_V]) changeLineColor(0, 0, -1);
		if (isPressed[GLFW_KEY_T]) changeFaceColor(1, 0, 0);
		if (isPressed[GLFW_KEY_H]) changeFaceColor(0, 1, 0);
		if (isPressed[GLFW_KEY_N]) changeFaceColor(0, 0, 1);
		if (isPressed[GLFW_KEY_Y]) changeFaceColor(-1, 0, 0);
		if (isPressed[GLFW_KEY_J]) changeFaceColor(0, -1, 0);
		if (isPressed[GLFW_KEY_M]) changeFaceColor(0, 0, -1);
		if (isPressed[GLFW_KEY_W]) myCam.ProcessKeyboard(PITCH_DOWN, camSpd);
		if (isPressed[GLFW_KEY_A]) myCam.ProcessKeyboard(ROTATE_COUNTERCLOCKWISE, camSpd);
		if (isPressed[GLFW_KEY_S]) myCam.ProcessKeyboard(PITCH_UP, camSpd);
		if (isPressed[GLFW_KEY_D]) myCam.ProcessKeyboard(ROTATE_CLOCKWISE, camSpd);
		if (isPressed[GLFW_KEY_UP]) myCam.ProcessKeyboard(DOWN, camSpd);
		if (isPressed[GLFW_KEY_DOWN]) myCam.ProcessKeyboard(UP, camSpd);
		if (isPressed[GLFW_KEY_LEFT]) myCam.ProcessKeyboard(RIGHT, camSpd);
		if (isPressed[GLFW_KEY_RIGHT]) myCam.ProcessKeyboard(LEFT, camSpd);
		if (isPressed[GLFW_KEY_EQUAL]) myCam.ProcessKeyboard(FORWARD, camSpd);
		if (isPressed[GLFW_KEY_MINUS]) myCam.ProcessKeyboard(BACKWARD, camSpd);
		render(renderMode, &VBO, &VAO, myShader, faceColorptr, model, view, projection, lineColor);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE); break;
		case GLFW_KEY_C:
			renderMode = (renderMode + 1) % 4; break;
		case GLFW_KEY_Q:
			if (faceIsColorful) {
				faceIsColorful = GL_FALSE;
				faceColorptr = faceColor;
			}
			else if (!faceIsColorful) {
				faceIsColorful = GL_TRUE;
				genColorfulFace();
				faceColorptr = faceColorful;
			}
			break;
		default:
			isPressed[key] = GL_TRUE; break;
		}
	}
	if (action == GLFW_RELEASE)
		isPressed[key] = GL_FALSE;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	myCam.ProcessMouseScroll((float)yoffset);
}

void render(int renderMode, GLuint* VBO, GLuint* VAO, Shader& myShader, GLfloat* allFaceColor, glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::vec4& lineColor) {
	glBindVertexArray(*VAO);
	GLint modelLoc = glGetUniformLocation(myShader.Program, "model"), viewLoc = glGetUniformLocation(myShader.Program, "view"), projLoc = glGetUniformLocation(myShader.Program, "projection"), fragmentColorLocation;
	switch (renderMode) {
	case 0:
		glPointSize(1.5f);
		myShader.Use();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		for (int i = 0; i < V; i++)
		{
			fragmentColorLocation = glGetUniformLocation(myShader.Program, "faceColor");
			myShader.Use();
			glUniform4f(fragmentColorLocation, lineColor.x, lineColor.y, lineColor.z, lineColor.w);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDrawArrays(GL_TRIANGLES, 3 * i, 3);
		}
		break;

	case 1:
		myShader.Use();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		fragmentColorLocation = glGetUniformLocation(myShader.Program, "faceColor");
		myShader.Use();
		glUniform4f(fragmentColorLocation, lineColor.x, lineColor.y, lineColor.z, lineColor.w);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3 * F);
		break;

	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (int i = 0; i < F; i++)
		{
			GLfloat red = allFaceColor[3 * i], green = allFaceColor[3 * i + 1], blue = allFaceColor[3 * i + 2];
			fragmentColorLocation = glGetUniformLocation(myShader.Program, "faceColor");
			modelLoc = glGetUniformLocation(myShader.Program, "model");
			viewLoc = glGetUniformLocation(myShader.Program, "view");
			projLoc = glGetUniformLocation(myShader.Program, "projection");
			myShader.Use();
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform4f(fragmentColorLocation, red, green, blue, 1.0f);
			glDrawArrays(GL_TRIANGLES, 3 * i, 3);
		}
		break;

	case 3:
		fragmentColorLocation = glGetUniformLocation(myShader.Program, "faceColor");
		myShader.Use();
		glUniform4f(fragmentColorLocation, lineColor.x, lineColor.y, lineColor.z, lineColor.w);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3 * F);
		for (int i = 0; i < F; i++)
		{
			GLfloat red = allFaceColor[3 * i], green = allFaceColor[3 * i + 1], blue = allFaceColor[3 * i + 2];
			fragmentColorLocation = glGetUniformLocation(myShader.Program, "faceColor");
			modelLoc = glGetUniformLocation(myShader.Program, "model"), viewLoc = glGetUniformLocation(myShader.Program, "view"), projLoc = glGetUniformLocation(myShader.Program, "projection");
			myShader.Use();
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform4f(fragmentColorLocation, red, green, blue, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 3 * i, 3);
		}
		break;
	}
	glBindVertexArray(0);
}

void genColorfulFace() {
	srand((unsigned)time(0));
	for (int i = 0; i != F * 3; i++) faceColorful[i] = (rand() % 15 + 6) / (GLfloat)20;
}

void changeFaceColor(int redMode, int greenMode, int blueMode) {
	for (int i = 0; i != F; i++) {
		faceColor[i * 3] += redMode * 0.0005f; faceColor[i * 3 + 1] += greenMode * 0.0005f; faceColor[i * 3 + 2] += blueMode * 0.0005f;
		if (faceColor[i * 3] > 1.0f) faceColor[i * 3] = 1.0f; if (faceColor[i * 3] < 0.0f) faceColor[i * 3] = 0.0f;
		if (faceColor[i * 3 + 1] > 1.0f) faceColor[i * 3 + 1] = 1.0f; if (faceColor[i * 3 + 1] < 0.0f) faceColor[i * 3 + 1] = 0.0f;
		if (faceColor[i * 3 + 2] > 1.0f) faceColor[i * 3 + 2] = 1.0f; if (faceColor[i * 3 + 2] < 0.0f) faceColor[i * 3 + 2] = 0.0f;
	}
}

void changeLineColor(int redMode, int greenMode, int blueMode) {
	lineColor.x += redMode * 0.0005f; lineColor.y += greenMode * 0.0005f; lineColor.z += blueMode * 0.0005f;
	if (lineColor.x > 1.0f) lineColor.x = 1.0f; if (lineColor.x < 0.0f) lineColor.x = 0.0f;
	if (lineColor.y > 1.0f) lineColor.y = 1.0f; if (lineColor.y < 0.0f) lineColor.y = 0.0f;
	if (lineColor.z > 1.0f) lineColor.z = 1.0f; if (lineColor.z < 0.0f) lineColor.z = 0.0f;
}
/*flag*/