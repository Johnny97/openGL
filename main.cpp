//Lab 1
//modified from http://learnopengl.com/

#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\glm\glm.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "objloader.h"
#include "shaloader.h"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
//camera setup
glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 direction = glm::vec3(0, 0, -1);
glm::vec3 up = glm::vec3(0, 1, 0);
//scale
glm::mat4 scale_matrixUP = glm::scale(glm::mat4(1.0f), glm::vec3(1.1, 1.1, 1.1));
glm::mat4 scale_matrixDOWN = glm::scale(glm::mat4(1.0f), glm::vec3(0.9, 0.9, 0.9));
//rotate
glm::mat4 rotat_matrixX = glm::rotate(glm::mat4(1.f), glm::radians((float)5), glm::vec3(1, 0, 0));
glm::mat4 rotat_matrixY = glm::rotate(glm::mat4(1.f), glm::radians((float)5), glm::vec3(0, 0, 1));
glm::mat4 rotat_matrixZ = glm::rotate(glm::mat4(1.f), glm::radians((float)5), glm::vec3(0, 1, 0));
//translate
glm::mat4 trans_matrixADDx = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0));
glm::mat4 trans_matrixMINx = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0, 0));
glm::mat4 trans_matrixADDy = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.5, 0));
glm::mat4 trans_matrixMINy = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, 0));
glm::mat4 trans_matrixADDz = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.5));
glm::mat4 trans_matrixMINz = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.5));
//model
glm::mat4 model_matrix = glm::mat4(1.0f);
glm::mat4 plane_model_matrix = glm::mat4(1.0f);
glm::vec3 objectcolor = glm::vec3(1.0f, 1.0f, 1.0f);
//glm::vec3 lightcolor = glm::vec3(0.8f, 0.8f, 0.8f);

bool on_red = false;
bool on_green = false;
bool on_blue = false;
bool on_all = false;
bool no_light = false;
bool no_normal = false;
glm::vec3 effectbool = glm::vec3(1,0,0);
void renderQuad();
unsigned int planeVAO;
//meshes

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		position += direction;
		std::cout << "left mouse button pressed" << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_2)
	{
		position -= direction;
		std::cout << "right mouse button pressed" << std::endl;
	}

}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W) {
		position += direction;
	}

	if (key == GLFW_KEY_S) {
		position -= direction;
	}

	if (key == GLFW_KEY_A) {
		position -= glm::cross(direction, up);
	}

	if (key == GLFW_KEY_D) {
		position += glm::cross(direction, up);
	}

	if (key == GLFW_KEY_UP) {
		direction += up * 0.01f;
	}

	if (key == GLFW_KEY_DOWN) {
		direction -= up * 0.01f;
	}

	if (key == GLFW_KEY_LEFT) {
		direction -= glm::cross(direction, up)*0.01f;
	}

	if (key == GLFW_KEY_RIGHT) {
		direction += glm::cross(direction, up)*0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		model_matrix = rotat_matrixX * model_matrix;
		//view_matrix = rotat_matrix * view_matrix;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		model_matrix = rotat_matrixY * model_matrix;
		//view_matrix = rotat_matrix * view_matrix;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		model_matrix = rotat_matrixZ * model_matrix;
		//view_matrix = rotat_matrix * view_matrix;
	}
	if (key == GLFW_KEY_J) {
		model_matrix = trans_matrixADDx * model_matrix;
	}
	if (key == GLFW_KEY_L) {
		model_matrix = trans_matrixMINx * model_matrix;
	}
	if (key == GLFW_KEY_I) {
		model_matrix = trans_matrixADDy * model_matrix;
	}
	if (key == GLFW_KEY_K) {
		model_matrix = trans_matrixMINy * model_matrix;
	}

	if (key == GLFW_KEY_PAGE_UP) {
		model_matrix = trans_matrixADDz * model_matrix;
	}
	if (key == GLFW_KEY_PAGE_DOWN) {
		model_matrix = trans_matrixMINz * model_matrix;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		model_matrix = scale_matrixUP * model_matrix;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		model_matrix = scale_matrixDOWN * model_matrix;
	}
	//toggle red
	if (key == GLFW_KEY_1 && action == GLFW_PRESS && on_red == false)
	{
		objectcolor = glm::vec3(1, 0, 0);
		on_red = true;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS && on_red == true)
	{
		objectcolor = glm::vec3(1, 0.5, 0.31);
		on_red = false;
	}
	//toggle green
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS && on_green == false)
	{
		objectcolor = glm::vec3(0, 1, 0);
		on_green = true;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS && on_green == true)
	{
		objectcolor = glm::vec3(1, 0.5, 0.31);
		on_green = false;
	}
	//toggle blue
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS && on_blue == false)
	{
		objectcolor = glm::vec3(0, 0, 1);
		on_blue = true;
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS && on_blue == true)
	{
		objectcolor = glm::vec3(1, 0.5, 0.31);
		on_blue = false;
	}
	//turn on all light
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		objectcolor = glm::vec3(1, 1, 1);
	}
	//toggle no light and on light
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS && no_light == false)
	{
		effectbool = glm::vec3(1,1,0);
		no_light == true;
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS && no_light == true)
	{
		effectbool = glm::vec3(1, 0, 0);
		no_light == false;
	}
	//normal light
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS && no_normal == false)
	{
		effectbool = glm::vec3(1,1,1);
		no_normal == true;
	}
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS && no_normal == true)
	{
		effectbool = glm::vec3(1, 0, 0);
		no_normal == false;
	}
	
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment2", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	// Build and compile our shader program
	// Vertex shader
	GLuint shader = loadSHADER("vertex.shader", "fragment.shader");
	GLuint Gouraudshader = loadSHADER("GouraudVertex.shader", "GouraudFragment.shader");
	GLuint depthProgramID = loadSHADER("DepthVS.shader","DepthFS.shader");

	//get a handle for our 'MVP' uniform
	GLuint depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

	//glUseProgram(shader);
	//glUseProgram(Gouraudshader);

	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec3> normals;
	//std::vector<glm::vec2> uvs;

	//// *load values from triangle.obj
	//loadOBJ("../sphere.obj", vertices, normals, uvs);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(10.0f, 15.0f, 5.0f),
		glm::vec3(-10.0f, 15.0f, 5.0f),
		glm::vec3(0.0f, 15.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 25.0f)
	};
	//color of each point light
	glm::vec3 pointLightColor[] = {
		glm::vec3(0.2f, 0.05f, 0.05f),
		glm::vec3(0.05f, 0.2f, 0.05f),
		glm::vec3(0.05f, 0.05f, 0.2f),
		glm::vec3(0.05f, 0.05f, 0.05f)
	};

	float planeVertices[] = {
		// positions            // normals         // texcoords
		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//The frame buffer
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	GLuint DepthBiasID = glGetUniformLocation(shader, "DepthBiasMVP");
	GLuint ShadowMapID = glGetUniformLocation(shader, "shadowMap");
	GLuint m_loc = glGetUniformLocation(shader, "model");
	GLuint v_loc = glGetUniformLocation(shader, "view");
	GLuint p_loc = glGetUniformLocation(shader, "pers");
	GLuint object_color_loc = glGetUniformLocation(shader, "object_color");
	GLuint light_effect_loc = glGetUniformLocation(shader, "light_effect");
	GLuint object_color_Gouraud_loc = glGetUniformLocation(Gouraudshader, "object_color");
	GLuint Shader_loc = glGetUniformLocation(shader, "lightSpaceMatrix");
	GLuint shadow_loc = glGetUniformLocation(shader, "shadowMap");


	//perspective
	glm::mat4 pers_matrix = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 1000.f);
	glPointSize(5.0f);
	
	bool on_gouraud = false;

	// Game loop
	int c = 0;
	while (!glfwWindowShouldClose(window))
	{
		
		//toggle shader
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && on_gouraud == false)
		{
			glUseProgram(Gouraudshader);
			on_gouraud = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && on_gouraud == true)
		{
			glUseProgram(shader);
			on_gouraud = false;
		}

		c++;
		//view matrix

		glm::mat4 view_matrix = glm::lookAt(position, position + direction, up);
		//shader
		glUseProgram(depthProgramID);
		glm::vec3 lightInvDir = glm::vec3(0.0, 20.0, 10.0);

		//compute the MVP matrix from the light's point of view
		//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		glm::vec3 lightPos(5, 20, 20);
		glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * model_matrix;
	
		glUseProgram(shader);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		
		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
		glUniform1i(ShadowMapID, 1);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		glUniformMatrix4fv(m_loc, 1, GL_FALSE, &model_matrix[0][0]);
		
		glUniformMatrix4fv(v_loc, 1, GL_FALSE, &view_matrix[0][0]);
		glUniformMatrix4fv(p_loc, 1, GL_FALSE, &pers_matrix[0][0]);
		glUniform3fv(object_color_loc, 1, reinterpret_cast<GLfloat *>(&objectcolor[0]));
		glUniform3fv(object_color_Gouraud_loc, 1, reinterpret_cast<GLfloat *>(&objectcolor[0]));
		glUniform3fv(light_effect_loc, 1, reinterpret_cast<GLfloat *>(&effectbool[0]));

		

		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glUniformMatrix4fv(m_loc, 1, GL_FALSE, &plane_model_matrix[0][0]);
		
		//renderQuad();
		

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
		
		

		glBindVertexArray(0);

		//render quad
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;

}

GLuint quadVAO = 0;
GLuint quadVBO;

void renderQuad()
{

	if (quadVAO == 0)
	{
		
		float quadVertices[] = {
			// positions        // texture Coords
			-3.0f,  -2.0f, -3.0f, 0.0f, 1.0f,
			-3.0f, -2.0f, 3.0f, 0.0f, 0.0f,
			3.0f,  -2.0f, -3.0f, 1.0f, 1.0f,
			3.0f, -2.0f, 3.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}



