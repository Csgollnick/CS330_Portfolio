///*Name: Chris Gollnick
//* Class: CS330-T4210
//* Date: 10 April 2021
//* Assignment: Module 6-5 Milestone
//*/
//
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>     // GLFW library
//
////Load Image
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//// GLM Math Header inclusions
//#include <glm/glm.hpp>
//#include <glm/gtx/transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "shader.h"
//#include "cylinder.h"
//#include <iostream>         // cout, cerr
//#include <cstdlib>          // EXIT_FAILURE
//
//
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//unsigned int loadTexture(const char* path);
//
////Window Settings
//const unsigned int SCR_WIDTH = 1920;
//const unsigned int SCR_HEIGHT = 1080;
//
////Camera
//glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 10.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 1.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::mat4 GetViewMatrix()
//{
//	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//}
//float MovementSpeed;
//
//bool firstMouse = true;
//float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//float pitch = 0.0f;
//float speed = 2.5f;
//float lastX = 1920.0 / 2.0;
//float lastY = 1080.0 / 2.0;
//float fov = 45.0f;
//float Zoom = 45.0f;
//
//// timing
//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;
//
////Lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//
//int main()
//{
//	// glfw: initialize and configure
//	// ------------------------------
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//	// glfw window creation
//	// --------------------
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Module 6-5 Milestone", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetScrollCallback(window, scroll_callback);
//
//	// tell GLFW to capture our mouse
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//
//	// glad: load all OpenGL function pointers
//	// ---------------------------------------
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//	// configure global opengl state
//	// -----------------------------
//	glEnable(GL_DEPTH_TEST);
//
//	// build and compile our shader zprogram
//	// ------------------------------------
//	//Shader ourShader("shaderfiles/7.3.camera.vs", "shaderfiles/7.3.camera.fs");
//	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
//	Shader lampShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");
//
//
//	unsigned int VBO, VAO;    //Table
//	unsigned int VBO2, VAO2; //Kemper Back
//	unsigned int VBO3, VAO3; //Kemper Left, Front
//	unsigned int VBO4, VAO4; //Kemper Right, Bottom
//	unsigned int VBO5, VAO5; //Glass
//	unsigned int VBO6, VAO6; //Sheet Music Left
//	unsigned int VBO7, VAO7; //SheetMusic Right
//	unsigned int VBO8, VAO8; //SheetMusic Spiral
//	unsigned int VBO9, VAO9; // Legs
//	unsigned int VBO10, VAO10; //Kemper Top
//	unsigned int VBO11, VAO11; //Kemper Top Panels
//	unsigned int VBO12, VAO12; //FootRest Top
//	unsigned int VBO13, VAO13; //Foot Rest Arms
//	unsigned int VBO14, VAO14; //Bottle Base
//	unsigned int VBO15, VAO15; //Bootle Cone
//	unsigned int VBO16, VAO16; //Bottle Neck
//	unsigned int VBO17, VAO17; //Bottle Cork
//
//	unsigned int lightVAO;
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	// ------------------------------------------------------------------
//	float vertices[] = {
//
//		//Coordinates				//Normals				//TExtures
//		//Table
//		-3.0f, -1.01f, -1.0f,		0.0f, 5.0f, 0.0f,		1.0f, 1.0f, //UL
//		-3.0f, -1.01f, 1.0f,		0.0f, 5.0f, 0.0f,		1.0f, 0.0f, //LL
//		3.0f, -1.01f, -1.0f,		0.0f, 5.0f, 0.0f,		0.0f, 1.0f, //UR
//
//		-3.0f, -1.01f, 1.0f,		0.0f, 5.0f, 0.0f,		1.0f, 0.0f, //LL
//		3.0f, -1.01f, -1.0f,		0.0f, 5.0f, 0.0f,		0.0f, 1.0f, //UR
//		3.0f, -1.01f, 1.0f,			0.0f, 5.0f, 0.0f,		0.0f, 0.0f //LR
//
//	};
//
//	glm::vec3 tablePosition[] = {
//		glm::vec3(0.0f, 0.0f, 0.0f)
//	};
//	glm::vec3 tableLightPosition[] = {
//		glm::vec3(0.0f, 5.0f, 0.0f)
//	};
//
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);// position attribute
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //light attribute
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));// texture coord attribute
//	glEnableVertexAttribArray(1);
//
//	//Light Configure VAO
//	glGenVertexArrays(1, &lightVAO);
//	glBindVertexArray(lightVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	
//	// load textures (we now use a utility function to keep the code more organized)
//	// -----------------------------------------------------------------------------
//	unsigned int diffuseMap = loadTexture("images/LightWoodTableGrain.jpg");
//	unsigned int specularMap = loadTexture("images/DarkWoodTableGrain.jpg");
//	
//	// shader configuration
//   // --------------------
//	lightingShader.use();
//	lightingShader.setInt("material.diffuse", 0);
//	lightingShader.setInt("material.specular", 1);
//
//	float verticeKemperBack[] = {
//		-1.0f, -0.5f, -1.5f,		1.0f, 1.0f,//0TL
//		-1.0f, -1.0f, -1.5f,		1.0f, 0.0f, //1BL
//		1.0f, -0.5f, -1.5f,			0.0f, 1.0f,//3TR
//
//		1.0f, -1.0f, -1.5f,			0.0f, 0.0f,//2BR
//		-1.0f, -1.0f, -1.5f,		1.0f, 0.0f,//1BL
//		1.0f, -0.5f, -1.5f,			0.0f, 1.0f//3TR
//	};
//	glGenVertexArrays(1, &VAO2);
//	glGenBuffers(1, &VBO2);
//	glBindVertexArray(VAO2);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeKemperBack), verticeKemperBack, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	float verticeKemperSides[] = {
//		//Left Side
//		-1.0f, -0.5f, -1.5f,		1.0f, 0.0f,//0TL
//		-1.0f, -1.0f, -1.5f,		1.0f, 1.0f, //1BL
//		-1.0f, -0.5f, -1.25f,		0.0f, 0.0f, //4 Top Left Upper Panel Corner
//
//		-1.0f, -1.0f, -1.5f,		1.0f, 1.0f, //1BL
//		-1.0f, -0.5f, -1.25f,		0.0f, 0.0f, //4 Top Left Upper Panel Corner
//		-1.0f, -1.0f, -1.25f,		0.0f, 1.0f, //5 Bottom Left Bottom Back Corner
//
//		-1.0f, -0.5f, -1.25f,		1.0f, 1.0f, //4 Top Left Upper Panel Corner
//		-1.0f, -1.0f, -1.25f,		1.0f, 0.0f, //5 Bottom Left Bottom Back Corner
//		-1.0f, -1.0f,  -0.25f,		0.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
//
//		-1.0f, -1.0f,  -0.25f,		0.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
//		-1.0f, -0.75f,  -0.25f,		0.0f, 1.0f, //9 Top Front Left Upper Panel Corner
//		-1.0f, -0.5f, -1.25f,		1.0f, 1.0f, //4 Top Left Upper Panel Corner
//
//		-1.0f, -0.75f,  0.0f,		0.0f, 1.0f, //7 Top Front Left top Corner
//		-1.0f, -1.0f,  -0.25f,		1.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
//		-1.0f, -0.75f,  -0.25f,		1.0f, 1.0f, //9 Top Front Left Upper Panel Corner
//
//		-1.0f, -1.0f,  0.0f,		0.0f, 0.0f, //6 Bottom Front Left Bottom Corner
//		-1.0f, -0.75f,  0.0f,		0.0f, 1.0f, //7 Top Front Left top Corner
//		-1.0f, -1.0f,  -0.25f,		1.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
//
//		//Front
//		1.0f, -1.0f,  0.0f,			0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
//		1.0f, -0.75f,  0.0f,		0.0f, 1.0f, //13 Top Front Right Corner
//		-1.0f, -0.75f,  0.0f,		1.0f, 1.0f, //7 Top Front Left top Corner
//
//		-1.0f, -0.75f,  0.0f,		1.0f, 1.0f, //7 Top Front Left top Corner
//		-1.0f, -1.0f,  0.0f,		1.0f, 0.0f, //6 Bottom Front Left Bottom Corner
//		1.0f, -1.0f,  0.0f,			0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
//
//	};
//	glGenVertexArrays(1, &VAO3);
//	glGenBuffers(1, &VBO3);
//	glBindVertexArray(VAO3);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeKemperSides), verticeKemperSides, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	float vertKemperRight[] = {
//
//		//Right Side
//		1.0f, -0.5f, -1.5f,			1.0f, 1.0f,//3TR
//		1.0f, -1.0f, -1.5f,			1.0f, 0.0f,//2BR
//		1.0f, -1.0f, -1.25f,		0.0f, 0.0f, //11 Bottom Back right Bottom Panel Corner
//
//		1.0f, -0.5f, -1.25f,		0.0f, 1.0f,	//10 Top Back right Upper Panel Corner
//		1.0f, -1.0f, -1.25f,		0.0f, 0.0f,	 //11 Bottom Back right Bottom Panel Corner
//		1.0f, -0.5f, -1.5f,			1.0f, 1.0f, //3TR
//
//		1.0f, -0.5f, -1.25f,		1.0f, 1.0f,  //10 Top Back right Upper Panel Corner
//		1.0f, -1.0f, -1.25f,		1.0f, 0.0f, //11 Bottom Back right Bottom Panel Corner
//		1.0f, -1.0f,  0.0f,			0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
//
//		1.0f, -0.75f,  -0.25f,		0.0f, 1.0f, //15 Top Front Right Upper Panel Corner
//		1.0f, -0.5f, -1.25f,		1.0f, 1.0f, //10 Top Back right Upper Panel Corner
//		1.0f, -1.0f,  -0.25f,		0.0f, 0.0f, //14 Bottom Front Right Bottom Panel Corner
//
//		1.0f, -1.0f,  0.0f,			0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
//		1.0f, -1.0f,  -0.25f,		1.0f, 0.0f, //14 Bottom Front Right Bottom Panel Corner
//		1.0f, -0.75f,  -0.25f,		1.0f, 1.0f, //15 Top Front Right Upper Panel Corner
//
//		1.0f, -1.0f,  0.0f,			0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
//		1.0f, -0.75f,  0.0f,		0.0f, 1.0f, //13 Top Front Right Corner
//		1.0f, -0.75f,  -0.25f,		1.0f, 1.0f, //15 Top Front Right Upper Panel Corner
//
//		//Bottom
//		-1.0f, -1.0f, -1.5f,		1.0f, 1.0f, //1BL
//		-1.0f, -1.0f,  0.0f,		1.0f, 0.0f, //6 Bottom Front Left Bottom Corner
//		1.0f, -1.0f, -1.5f,			0.0f, 1.0f,//2BR
//		-1.0f, -1.0f,  0.0f,		1.0f, 0.0f, //6 Bottom Front Left Bottom Corner
//		1.0f, -1.0f, -1.5f,			0.0f, 1.0f,//2BR
//		1.0f, -1.0f,  0.0f,			0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
//
//	};
//	glGenVertexArrays(1, &VAO4);
//	glGenBuffers(1, &VBO4);
//	glBindVertexArray(VAO4);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertKemperRight), vertKemperRight, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	//Glass 5
//	glGenVertexArrays(1, &VAO5);
//	glGenBuffers(1, &VBO5);
//	glBindVertexArray(VAO5);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
//
//	//Sheet Music Left 6
//	float vertMusicLeft[] = {
//	0.0f, -1.0f, 0.0f,			1.0f, 1.0f, //0 Top Left Corner
//	0.0f, -1.0f, 1.0f, 		1.0f, 0.0f, //1 Bottom Left Corner
//	1.0f, -1.0f, 1.0f,			0.0f, 0.0f, //2 Bottom Right
//
//	0.0f, -1.0f, 0.0f,			1.0f, 1.0f, //0 Top Left Corner
//	1.0f, -1.0f, 1.0f,			0.0f, 0.0f, //2 Bottom Right
//	1.0f, -1.0f, 0.0f,			0.0f, 1.0f  //3 Top Right
//	};
//	glGenVertexArrays(1, &VAO6);
//	glGenBuffers(1, &VBO6);
//	glBindVertexArray(VAO6);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertMusicLeft), vertMusicLeft, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	//SheetMusic Right7 
//	float vertMusicRight[] = {
//		1.1f, -1.0f, 0.0f,			1.0f, 1.0f, //0 Top Left
//		1.1f, -1.0f, 1.0f,			1.0f, 0.0f, //2 BOttom Left
//		2.1f, -1.0f, 1.0f,			0.0f, 0.0f, //3 Bottom Right
//
//	   2.1f, -1.0f, 0.0f,			0.0f, 1.0f, //1 Top Right
//	   1.1f, -1.0f, 0.0f,			1.0f, 1.0f, //0 Top Left
//	   2.1f, -1.0f, 1.0f,			0.0f, 0.0f //3 Bottom Right
//
//
//	};
//	glGenVertexArrays(1, &VAO7);
//	glGenBuffers(1, &VBO7);
//	glBindVertexArray(VAO7);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertMusicRight), vertMusicRight, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//	// 
//	//Sheet Music Spiral 8
//	float vertSpiral[] = {
//		1.0f, -1.0f, 0.0f,			1.0f, 1.0f, //0top Left 
//		1.0f, -1.0f, 1.0f,			1.0f, 0.0f, //2bottom left
//		1.1f, -1.0f, 1.0f,			0.0f, 0.0f, //3bottom right
//
//		1.0f, -1.0f, 0.0f,			1.0f, 1.0f, //0top Left 
//		1.1f, -1.0f, 0.0f,			0.0f, 1.0f, //1top Right
//		1.1f, -1.0f, 1.0f,			0.0f, 0.0f //3bottom right
//	};
//	glGenVertexArrays(1, &VAO8);
//	glGenBuffers(1, &VBO8);
//	glBindVertexArray(VAO8);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertSpiral), vertSpiral, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//	// 
//	////Table LEgs 9
//	//float vertLegs[] = {
//
//	//};
//
//	//Kemper Top 10
//	float vertKemperTop[] = {
//		1.0f, -0.75f,  -0.25f,		1.0f, 0.0f, //15 Top Front Right Upper Panel Corner
//		-1.0f, -0.75f,  -0.25f,		0.0f, 0.0f, //9 Top Front Left Upper Panel Corner
//		-1.0f, -0.5f, -1.25f,		0.0f, 1.0f, //4 Top Left Upper Panel Corner
//
//		1.0f, -0.5f, -1.25f,		1.0f, 1.0f,	//10 Top Back right Upper Panel Corner
//		1.0f, -0.75f,  -0.25f,		1.0f, 0.0f, //15 Top Front Right Upper Panel Corner
//		-1.0f, -0.5f, -1.25f,		0.0f, 1.0f, //4 Top Left Upper Panel Corner
//	};
//
//	glGenVertexArrays(1, &VAO10);
//	glGenBuffers(1, &VBO10);
//	glBindVertexArray(VAO10);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertKemperTop), vertKemperTop, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//
//	//Kemper Panles 11
//	float vertPanels[] = {
//		//Upper
//		-1.0f, -0.5f, -1.5f,		1.0f, 1.0f,//0TL
//		1.0f, -0.5f, -1.25f,		0.0f, 0.0f,	//10 Top Back right Upper Panel Corner
//		-1.0f, -0.5f, -1.25f,		1.0f, 0.0f, //4 Top Left Upper Panel Corner
//
//		1.0f, -0.5f, -1.5f,			0.0f, 1.0f,//3TR
//		-1.0f, -0.5f, -1.5f,		1.0f, 1.0f,//0TL
//		1.0f, -0.5f, -1.25f,		0.0f, 0.0f,	//10 Top Back right Upper Panel Corner
//
//		//Lower
//		1.0f, -0.75f,  -0.25f,		0.0f, 1.0f, //15 Top Front Right Upper Panel Corner
//		1.0f, -0.75f,  0.0f,		0.0f, 0.0f, //13 Top Front Right Corner
//		-1.0f, -0.75f,  -0.25f,		1.0f, 0.0f, //9 Top Front Left Upper Panel Corner
//
//		-1.0f, -0.75f,  0.0f,		1.0f, 0.0f, //7 Top Front Left top Corner
//		1.0f, -0.75f,  0.0f,		0.0f, 0.0f, //13 Top Front Right Corner
//		-1.0f, -0.75f,  -0.25f,		1.0f, 1.0f, //9 Top Front Left Upper Panel Corner
//	};
//
//	glGenVertexArrays(1, &VAO11);
//	glGenBuffers(1, &VBO11);
//	glBindVertexArray(VAO11);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO11);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertPanels), vertPanels, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//
//	//Foot Rest Top 12 
//
//	//Foot Rest Arms & Sides 13
//
//	//Bottle Base 14
//	glGenVertexArrays(1, &VAO14);
//	glGenBuffers(1, &VBO14);
//	glBindVertexArray(VAO14);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO14);
//	//Bootle Cone 15
//
//	//Bottle Neck 16
//	glGenVertexArrays(1, &VAO16);
//	glGenBuffers(1, &VBO16);
//	glBindVertexArray(VAO16);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO16);
//
//	//Bottle Cork 17
//	glGenVertexArrays(1, &VAO17);
//	glGenBuffers(1, &VBO17);
//	glBindVertexArray(VAO17);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO17);
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	// load and create a texture 
//	// -------------------------
//	unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9, texture10, texture11, texture12,
//		texture13, texture14, texture15, texture16, texture17, texture18, texture19;
//	int width, height, nrChannels;
//
//	////// texture 1
//	//// ---------
//	//glGenTextures(1, &texture1);
//	//glBindTexture(GL_TEXTURE_2D, texture1);
//	//// set the texture wrapping parameters
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	//// set texture filtering parameters
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//// load image, create texture and generate mipmaps
//	//
//	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	//unsigned char* data = stbi_load("images/blackwoodgrain.jpg", &width, &height, &nrChannels, 0);
//	//if (data)
//	//{
//	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//	//	glGenerateMipmap(GL_TEXTURE_2D);
//	//}
//	//else
//	//{
//	//	std::cout << "Failed to load texture" << std::endl;
//	//}
//	//stbi_image_free(data);
//	//
//	// texture 2
//	// ---------
//	glGenTextures(1, &texture2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	unsigned char* data2 = stbi_load("images/KemperBack.png", &width, &height, &nrChannels, 0);
//	if (data2)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data2);
//
//	// texture 3
//// ---------
//	glGenTextures(1, &texture3);
//	glBindTexture(GL_TEXTURE_2D, texture3);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data3 = stbi_load("images/GreenMetal.png", &width, &height, &nrChannels, 0);
//	if (data3)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data3);
//
//	// texture 4
//// ---------
//	glGenTextures(1, &texture4);
//	glBindTexture(GL_TEXTURE_2D, texture4);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data4 = stbi_load("images/Glass.png", &width, &height, &nrChannels, 0);
//	if (data4)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data4);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data4);
//
//	// texture 5
//// ---------
//	glGenTextures(1, &texture5);
//	glBindTexture(GL_TEXTURE_2D, texture5);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data5 = stbi_load("images/PageLeft.png", &width, &height, &nrChannels, 0);
//	if (data5)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data5);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data5);
//
//	// texture 6
//// ---------
//	glGenTextures(1, &texture6);
//	glBindTexture(GL_TEXTURE_2D, texture6);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data6 = stbi_load("images/PageRight.png", &width, &height, &nrChannels, 0);
//	if (data6)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data6);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data6);
//
//	// texture 7
//// ---------
//	glGenTextures(1, &texture7);
//	glBindTexture(GL_TEXTURE_2D, texture7);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data7 = stbi_load("images/Spiral.png", &width, &height, &nrChannels, 0);
//	if (data7)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data7);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data7);
//
//
//
//	// texture 11
//// ---------
//	glGenTextures(1, &texture11);
//	glBindTexture(GL_TEXTURE_2D, texture11);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data11 = stbi_load("images/KemperTop.png", &width, &height, &nrChannels, 0);
//	if (data11)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data11);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data11);
//
//	// texture 14
//// ---------
//	glGenTextures(1, &texture14);
//	glBindTexture(GL_TEXTURE_2D, texture14);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data14 = stbi_load("images/OdinsSkull.png", &width, &height, &nrChannels, 0);
//	if (data14)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data14);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data14);
//
//	// texture 15
//// ---------
//	glGenTextures(1, &texture15);
//	glBindTexture(GL_TEXTURE_2D, texture15);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data15 = stbi_load("images/WhiteCeramic.jpg", &width, &height, &nrChannels, 0);
//	if (data15)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data15);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data15);
//
//	// texture 16
//// ---------
//	glGenTextures(1, &texture16);
//	glBindTexture(GL_TEXTURE_2D, texture16);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char* data16 = stbi_load("images/Cork.png", &width, &height, &nrChannels, 0);
//	if (data16)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data16);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data16);
//
//
//	 //tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
//	------------------------------------------------------------------------------------------ -
//		ourShader.use();
//	//ourShader.setInt("texture1", 0);
//	ourShader.setInt("texture2", 1);
//	ourShader.setInt("texture3", 2);
//	ourShader.setInt("texture4", 3);
//	ourShader.setInt("texture5", 4);
//	ourShader.setInt("texture6", 5);
//	ourShader.setInt("texture7", 6);
//	ourShader.setInt("texture8", 7);
//	ourShader.setInt("texture9", 8);
//	ourShader.setInt("texture10", 9);
//	ourShader.setInt("texture11", 10);
//	ourShader.setInt("texture12", 11);
//	ourShader.setInt("texture13", 12);
//	ourShader.setInt("texture14", 13);
//	ourShader.setInt("texture15", 14);
//	ourShader.setInt("texture16", 15);
//	ourShader.setInt("texture17", 16);
//	ourShader.setInt("texture18", 17);
//	ourShader.setInt("texture19", 18);
//
//	//glm::mat4 model;
//	//float angle;
//
//	// render loop
//	// -----------
//	while (!glfwWindowShouldClose(window))
//	{
//		// per-frame time logic
//		// --------------------
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		// input
//		// -----
//		processInput(window);
//
//		// render
//		// ------
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		//activate shader when setting uniforms/drawing objects
//		lightingShader.use();
//		lightingShader.setVec3("viewPos", cameraPos);
//		lightingShader.setFloat("material.shininess", 32.0f);
//
//		// directional light
//		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
//		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
//		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
//		// point light 1
//		lightingShader.setVec3("pointLights[0].position", tableLightPosition[0]);
//		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
//		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
//		lightingShader.setFloat("pointLights[0].constant", 1.0f);
//		lightingShader.setFloat("pointLights[0].linear", 0.09);
//		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
//		// spotLight
//		lightingShader.setVec3("spotLight.position", cameraPos);
//		lightingShader.setVec3("spotLight.direction", cameraFront);
//		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//		lightingShader.setFloat("spotLight.constant", 1.0f);
//		lightingShader.setFloat("spotLight.linear", 0.09);
//		lightingShader.setFloat("spotLight.quadratic", 0.032);
//		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
//		// view/projection transformations
//		glm::mat4 projection = glm::perspective(glm::radians(Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glm::mat4 view = GetViewMatrix();
//		lightingShader.setMat4("projection", projection);
//		lightingShader.setMat4("view", view);
//		// world transformation
//		glm::mat4 model;
//		lightingShader.setMat4("model", model);
//		// bind diffuse map
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, diffuseMap);
//		// bind specular map
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, specularMap);
//
//		// render containers
//		glBindVertexArray(VAO);
//		for (unsigned int i = 0; i < 10; i++)
//		{
//			// calculate the model matrix for each object and pass it to shader before drawing
//			glm::mat4 model;
//			model = glm::translate(model, tablePosition[i]);
//			float angle = 20.0f * i;
//			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//			lightingShader.setMat4("model", model);
//
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//		}
//
//		// also draw the lamp object(s)
//		lampShader.use();
//		lampShader.setMat4("projection", projection);
//		lampShader.setMat4("view", view);
//
//		//draw as many light bulbs as we have point lights.
//		glBindVertexArray(lightVAO);
//		for (unsigned int i = 0; i < 4; i++)
//		{
//			model = glm::mat4();
//			model = glm::translate(model, tableLightPosition[i]);
//			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
//			lampShader.setMat4("model", model);
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//		}
//
//
//		
//		//// bind textures on corresponding texture units
//		//glActiveTexture(GL_TEXTURE0);
//		//glBindTexture(GL_TEXTURE_2D, texture1);
//		// activate shader
//		//ourShader.use();
//		//// pass projection matrix to shader (note that in this case it could change every frame)
//		//glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		//ourShader.setMat4("projection", projection);
//		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);	// camera/view transformation
//		//ourShader.setMat4("view", view);
//		//glBindVertexArray(VAO);
//		//		// calculate the model matrix for each object and pass it to shader before drawing
//		//model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		//angle = 0.0f;
//		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//		//ourShader.setMat4("model", model);
//		//glDrawArrays(GL_TRIANGLES, 0, 36);
//
//
//		//texture for vao2
//		ourShader.use();
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture2);
//		glBindVertexArray(VAO2);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao3
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture3);
//		glBindVertexArray(VAO3);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao4
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture3);
//		glBindVertexArray(VAO4);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao5
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture4);
//		glBindVertexArray(VAO5);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -0.5f));
//		ourShader.setMat4("model", model);
//		static_meshes_3D::Cylinder Glass(0.2, 14, 0.5, true, true, true);
//		Glass.render();
//
//		//texture for vao6
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture5);
//		glBindVertexArray(VAO6);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		angle = 15.0f;
//		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao7
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture6);
//		glBindVertexArray(VAO7);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		angle = 15.0f;
//		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao8
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture7);
//		glBindVertexArray(VAO8);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		angle = 15.0f;
//		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//
//		//texture for vao10
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture11);
//		glBindVertexArray(VAO10);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao11
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture3);
//		glBindVertexArray(VAO11);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		ourShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		//texture for vao14
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture15);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture14);
//		glBindVertexArray(VAO14);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
//		model = glm::translate(model, glm::vec3(1.5f, 0.0f, -0.5f));
//		ourShader.setMat4("model", model);
//		static_meshes_3D::Cylinder BottleBase(0.2, 14, 0.8, true, true, true);
//		BottleBase.render();
//
//		/*
//		//texture for vao16
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture15);
//		glBindVertexArray(VAO16);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//		model = glm::translate(model, glm::vec3(1.5f, 5.2f, -0.5f));
//		ourShader.setMat4("model", model);
//		static_meshes_3D::Cylinder BottleNeck(0.04, 14, 0.2, true, true, true);
//		BottleNeck.render();
//
//		//texture for vao17
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture16);
//		glBindVertexArray(VAO17);
//		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//		model = glm::translate(model, glm::vec3(1.5f, 5.31f, -0.5f));
//		ourShader.setMat4("model", model);
//		static_meshes_3D::Cylinder BottleCork(0.05, 14, 0.05, true, true, true);
//		BottleCork.render();
//		*/
//
//
//		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//		// -------------------------------------------------------------------------------
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	// optional: de-allocate all resources once they've outlived their purpose:
//	// ------------------------------------------------------------------------
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteVertexArrays(1, &lightVAO);
//	glDeleteBuffers(1, &VBO);
//
//	glDeleteVertexArrays(1, &VAO2);
//	glDeleteBuffers(1, &VBO2);
//
//	glDeleteVertexArrays(1, &VAO3);
//	glDeleteBuffers(1, &VBO3);
//
//	glDeleteVertexArrays(1, &VAO4);
//	glDeleteBuffers(1, &VBO4);
//
//	glDeleteVertexArrays(1, &VAO5);
//	glDeleteBuffers(1, &VBO5);
//
//	glDeleteVertexArrays(1, &VAO6);
//	glDeleteBuffers(1, &VBO6);
//
//	glDeleteVertexArrays(1, &VAO7);
//	glDeleteBuffers(1, &VBO7);
//
//	glDeleteVertexArrays(1, &VAO8);
//	glDeleteBuffers(1, &VBO8);
//
//	glDeleteVertexArrays(1, &VAO10);
//	glDeleteBuffers(1, &VBO10);
//
//	glDeleteVertexArrays(1, &VAO11);
//	glDeleteBuffers(1, &VBO11);
//
//	glDeleteVertexArrays(1, &VAO14);
//	glDeleteBuffers(1, &VBO14);
//
//	glDeleteVertexArrays(1, &VAO16);
//	glDeleteBuffers(1, &VBO16);
//
//	glDeleteVertexArrays(1, &VAO17);
//	glDeleteBuffers(1, &VBO17);
//
//	// glfw: terminate, clearing all previously allocated GLFW resources.
//	// ------------------------------------------------------------------
//	glfwTerminate();
//	return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	float cameraSpeed = 2.5 * deltaTime;
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		cameraPos += cameraSpeed * cameraFront;
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		cameraPos -= cameraSpeed * cameraFront;
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//		cameraPos += cameraSpeed * cameraUp;
//	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//		cameraPos -= cameraSpeed * cameraUp;
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//	lastX = xpos;
//	lastY = ypos;
//
//	float sensitivity = 0.1f; // change this value to your liking
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += xoffset;
//	pitch += yoffset;
//
//	// make sure that when pitch is out of bounds, screen doesn't get flipped
//	if (pitch > 89.0f)
//		pitch = 89.0f;
//	if (pitch < -89.0f)
//		pitch = -89.0f;
//
//	glm::vec3 front;
//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front.y = sin(glm::radians(pitch));
//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	cameraFront = glm::normalize(front);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//
//	MovementSpeed -= (float)yoffset;
//	if (MovementSpeed < 1.0f)
//		MovementSpeed = 1.0f;
//	if (MovementSpeed > 45.0f)
//		MovementSpeed = 45.0f;
//}
//
//unsigned int loadTexture(char const* path)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}