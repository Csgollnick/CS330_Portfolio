///*Name: Chris Gollnick
//* Class: CS330-T4210
//* Date: 18 April 2021
//* Assignment: Module7 Final Milestone
//*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "cylinder.h"
#include "cone.h"
#include "openCylinder.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 1.2f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");
	Shader textureShader("shaderfiles / 7.3.camera.vs", "shaderfiles / 7.3.camera.fs");

	
	unsigned int VBO2, VAO2;	//Kemper Back
	unsigned int VBO3, VAO3;	//Kemper Left, Front
	unsigned int VBO4, VAO4;	//Kemper Right, Bottom
	unsigned int VBO5, VAO5;	 //Glass
	unsigned int VBO6, VAO6;	 //Sheet Music Pages
	unsigned int VBO7, VAO7;	//SheetMusic Sides
	unsigned int VBO8, VAO8;	//Floor
	unsigned int VBO9, VAO9;	// Legs
	unsigned int VBO10, VAO10; //Kemper Top
	unsigned int VBO11, VAO11; //Kemper Top Panels
	unsigned int VBO12, VAO12; //FootRest Top
	unsigned int VBO13, VAO13; //Foot sides
	unsigned int VBO14, VAO14; //Bottle Base
	unsigned int VBO15, VAO15; //Bootle Cone
	unsigned int VBO16, VAO16; //Bottle Neck
	unsigned int VBO17, VAO17; //Bottle Cork
	unsigned int VBO18, VAO18; //FootRest Arms
	unsigned int VBO19, VAO19; //PencilBody
	unsigned int VBO20, VAO20; //Pencil Tip
	unsigned int VBO21, VAO21; //Walls
	unsigned int VBO22, VAO22;
	unsigned int VBO23, VAO23, VAO23a, VAO23b, VAO23c, VAO23d; //Guitar Body Q1
	unsigned int VBO24, VAO24, VAO24a, VAO24b, VAO24c, VAO24d; //Guitar Body Q2
	unsigned int VBO25, VAO25, VAO25a, VAO25b, VAO25c, VAO25d, VAO25e; //Guitar Body Q3
	unsigned int VBO26, VAO26, VAO26a, VAO26b, VAO26c, VAO26d; //GuitarBodyq4
	unsigned int VBO27, VAO27, VAO27a, VAO27b, VAO27c;//GuutarNeck
	unsigned int VBO28, VAO28, VAO28a; //Headstock
	unsigned int VBO29, VAO29, VAO29a, VAO29b; //Covrers
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions            // normals           // texture coords
		-3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f,
		-3.0f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f,
		3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f,

		3.0f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f,
		3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f,
		-3.0f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f,

	};
	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  -0.01f,  0.0f),
		
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  8.9f,  0.0f),
		glm::vec3(-8.0f,  8.9f,  0.0f),
		glm::vec3(8.0f,  8.9f,  0.0f)
	};
	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float verticeKemperBack[] = {
				-0.77f, 0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f,//0TL
				-0.77f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //1BL
				0.77f, 0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f,//3TR

				0.77, 0.0f, -1.0f,			0.0f, 2.0f, 0.0f,	0.0f, 0.0f,//2BR
				-0.77f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f,//1BL
				0.77f, 0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f//3TR
	};
	glm::vec3 kbPositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
	};
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeKemperBack), verticeKemperBack, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	float verticeKemperSides[] = {
				//Left Side
				-0.77f, 0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f,//0TL
				-0.77f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //1BL
				-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //4 Top Left Upper Panel Corner
		
				-0.77f, 0.0f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //1BL
				-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //4 Top Left Upper Panel Corner
				-0.77f, 0.0f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //5 Bottom Left Bottom Back Corner
		
				-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //4 Top Left Upper Panel Corner
				-0.77f, 0.0f, -0.86f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //5 Bottom Left Bottom Back Corner
				-0.77f, 0.0f,  -0.3f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
		
				-0.77f, 0.0f,  -0.3f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
				-0.77f, 0.14f, -0.3f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //9 Top Front Left Upper Panel Corner
				-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //4 Top Left Upper Panel Corner
		
				-0.77f, 0.14f,  -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //7 Top Front Left top Corner
				-0.77f, 0.0f,  -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
				-0.77f, 0.14f, -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //9 Top Front Left Upper Panel Corner
		
				-0.77f, 0.0f,  -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //6 Bottom Front Left Bottom Corner
				-0.77f, 0.14f,  -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //7 Top Front Left top Corner
				-0.77f, 0.0f,  -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //8 Bottom Front Left Bottom Panel Corner
		
				//Front
				0.77f, 0.0f,  -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
				0.77f, 0.14f, -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //13 Top Front Right Corner
			   -0.77f, 0.14f, -0.16f,	    0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //7 Top Front Left top Corner
		
				-0.77f, 0.14f, -0.16f,	    0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //7 Top Front Left top Corner
				-0.77f, 0.0f,  -0.16f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //6 Bottom Front Left Bottom Corner
				 0.77f, 0.0f,  -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
		
			};
	glm::vec3 ksPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeKemperSides), verticeKemperSides, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertKemperRight[] = {
				
		//Right Side
		0.77f, 0.28f, -1.0f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f,//3TR
		0.77f, 0.0f, -1.0f,				0.0f, 2.0f, 0.0f,		1.0f, 0.0f,//2BR
		0.77f, 0.0f, -0.86f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //11 Bottom Back right Bottom Panel Corner
	
		0.77f, 0.28f, -0.86f,			0.0f, 2.0f, 0.0f,		0.0f, 1.0f,	//10 Top Back right Upper Panel Corner
		0.77f, 0.0f, -0.86f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f,	 //11 Bottom Back right Bottom Panel Corner
		0.77f, 0.28f, -1.0f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //3TR
	
		0.77f, 0.28f, -0.86f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f,  //10 Top Back right Upper Panel Corner
		0.77f, 0.0f, -0.86f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //11 Bottom Back right Bottom Panel Corner
		0.77f, 0.0f,  -0.16f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
		
		0.77f, 0.14f, -0.3f,			0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //15 Top Front Right Upper Panel Corner
		0.77f, 0.28f, -0.86f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //10 Top Back right Upper Panel Corner
		0.77f, 0.0f,  -0.3f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //14 Bottom Front Right Bottom Panel Corner
		
		0.77f, 0.0f,  -0.16f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
		0.77f, 0.0f,  -0.3f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //14 Bottom Front Right Bottom Panel Corner
		0.77f, 0.14f, -0.3f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //15 Top Front Right Upper Panel Corner
		
		0.77f, 0.0f,   -0.16f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
		0.77f, 0.14f,  -0.16f,			0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //13 Top Front Right Corner
		0.77f, 0.14f,  -0.3f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //15 Top Front Right Upper Panel Corner
		
		//Bottom
		-0.77f, 0.0f, -1.0f,			0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //1BL
		-0.77f, 0.0f,  -0.16f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //6 Bottom Front Left Bottom Corner
		0.77f,  0.0f, -1.0f,			0.0f, 2.0f, 0.0f,		0.0f, 1.0f,//2BR
		-0.77f, 0.0f,  -0.16f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //6 Bottom Front Left Bottom Corner
		0.77f,  0.0f, -1.0f,			0.0f, 2.0f, 0.0f,		0.0f, 1.0f,//2BR
		0.77f,  0.0f,  -0.16f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //12 Bottom Front Right Bottom Corner
				
	};
	glm::vec3 krPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertKemperRight), vertKemperRight, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	
	//Glass 5
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glm::vec3 glassPositions[] = {
	glm::vec3(-1.35f,  0.92f,  -0.4f)
	};

	//Sheet Music 6
	float vertMusicPages[] = {
		0.0f, 0.025f, -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //TL Corner
		-0.2f, 0.025f, 0.76f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //BL Corner
		1.22f, 0.025f, 0.92f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //BR Corner

		0.0f, 0.025f, -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //TL Corner
		1.22f, 0.025f, 0.92f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //BR Corner
		1.42f, 0.025f, 0.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f //TR Corner

	};
	glm::vec3 smPositions[] = {
	glm::vec3(-0.2f,  0.0f,  -0.3f)
	};
	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glBindVertexArray(VAO6);
	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertMusicPages), vertMusicPages, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	//SheetMusic Sides7 
	float vertMusicSides[] = {
		//Left
		 0.0f, 0.025f, -0.16f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //TLU Corner
		 0.0f, 0.0f, -0.16f,			0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //TLL Corner
		-0.2f, 0.025f, 0.76f,			0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //BLU Corner

		-0.2f, 0.025f, 0.76f,			0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //BLU Corner
		-0.2f, 0.0f, 0.76f,				0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //BLL Corner
		 0.0f, 0.0f, -0.16f,			0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //TLL Corner

		//Top
		 0.0f, 0.025f, -0.16f,			0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //TLU Corner
		 0.0f, 0.0f, -0.16f,			0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //TLL Corner
		 1.42f, 0.025f, 0.0f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //TRU Corner

		 1.42f, 0.0f, 0.0f,				0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //TRL Corner
		 0.0f, 0.0f, -0.16f,			0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //TLL Corner
		 1.42f, 0.025f, 0.0f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //TRU Corner

		//Bottom
		-0.2f, 0.025f, 0.76f,			0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //BLU Corner
		-0.2f, 0.0f, 0.76f,				0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //BLL Corner
		 1.22f, 0.025f, 0.92f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //BRU Corner

		 1.22f, 0.0f, 0.92f,			0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //BRL Corner
		 -0.2f, 0.0f, 0.76f,			0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //BLL Corner
		 1.22f, 0.025f, 0.92f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //BRU Corner

		//Right
		1.22f, 0.025f, 0.92f,			0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //BRU Corner
		1.42f, 0.025f, 0.0f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //TRU Corner
		1.22f, 0.0f, 0.92f,				0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //BRL Corner

		1.42f, 0.0f, 0.0f,				0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //TRL Corner
		1.42f, 0.025f, 0.0f,			0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //TRU Corner
		1.22f, 0.0f, 0.92f,				0.0f, 2.0f, 0.0f,	0.0f, 0.0f //BRL Corner

	
	};
	glm::vec3 smsPositions[] = {
	glm::vec3(-0.2f,  0.0f,  -0.3f)
	};
	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glBindVertexArray(VAO7);
	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertMusicSides), vertMusicSides, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// 
	//Floor 8
	float vertFloor[] = {
	 15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //UR
	 15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //LR
	-15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //UL
	
	-15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //LL
	 15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //LR
	-15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f //UL
	}; 
	glm::vec3 floorPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glBindVertexArray(VAO8);
	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertFloor), vertFloor, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	 
	//Table LEgs 9
	float vertLegs[] = {
	//Front Left Leg
		//Top Piece
	-3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Top
	-2.92f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Top
	-3.0f, 0.0f, 0.92f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Top
	-2.92f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner TOP
	-3.0f, 0.0f, 0.92f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Top
	-2.92f, 0.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner TOP
		//Front face
	-3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Top
	-3.0f, -3.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Bottom
	-2.92f, -3.0f, 1.0f,	0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Bottom
	-3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Top
	-2.92f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Top
	-2.92f, -3.0f, 1.0f,	0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Bottom
		//Left face
	-3.0f, 0.0f, 0.92f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Top
	-3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Top	
	-3.0f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Bottom
	-3.0f, -3.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Bottom
	-3.0f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Bottom
	-3.0f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //FL Corner Top	
		//BackFace
	-3.0f, 0.0f, 0.92f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Top
	-2.92f, 0.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner TOP
	-2.92f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner TBottom
	-3.0f, 0.0f, 0.92f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Top  
	-3.0f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //BL Corner Top  
	-2.92f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner TBottom
			//RightFace
	-2.92f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner Bottom
	-2.92f, 0.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner TOP
	-2.92f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Top
	-2.92f, -3.0f, 1.0f,	0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Bottom
	-2.92f, 0.0f, 1.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //FR Corner Top
	-2.92f, -3.0f, 0.92f,	0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //BR Corner Bottom

	};
	glm::vec3 legsPositions[] = {
	glm::vec3(0.0f,  -0.01f,  0.0f),
	glm::vec3(5.92f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f,  -1.92f),
	glm::vec3(-5.92f,  0.0f,  -0.0f)
	};
	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glBindVertexArray(VAO9);
	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertLegs), vertLegs, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	//Kemper Top 10
	float vertKemperTop[] = {
		0.77f, 0.14f,  -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //15 Top Front Right Upper Panel Corner
		-0.77f, 0.14f, -0.3f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //9 Top Front Left Upper Panel Corner
		-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //4 Top Left Upper Panel Corner
	
		0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f,	//10 Top Back right Upper Panel Corner
		0.77f, 0.14f,  -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //15 Top Front Right Upper Panel Corner
		-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //4 Top Left Upper Panel Corner
	};
	glm::vec3 ktPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	
	glGenVertexArrays(1, &VAO10);
	glGenBuffers(1, &VBO10);
	glBindVertexArray(VAO10);
	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertKemperTop), vertKemperTop, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	
	//Kemper Panles 11
	float vertPanels[] = {
		//Upper
		-0.77f, 0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f,//0TL
		0.77f,  0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f,	//10 Top Back right Upper Panel Corner
		-0.77f, 0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //4 Top Left Upper Panel Corner
	
		0.77f,  0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f,//3TR
		-0.77f, 0.28f, -1.0f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f,//0TL
		0.77f,  0.28f, -0.86f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f,	//10 Top Back right Upper Panel Corner
	
		//Lower
		0.77f,  0.14f,  -0.3f,		0.0f, 2.0f, 0.0f,	0.0f, 1.0f, //15 Top Front Right Upper Panel Corner
		0.77f,  0.14f,  -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //13 Top Front Right Corner
		-0.77f, 0.14f,  -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //9 Top Front Left Upper Panel Corner
	
		-0.77f, 0.14f,  -0.16f,		0.0f, 2.0f, 0.0f,	1.0f, 0.0f, //7 Top Front Left top Corner
		0.77f,  0.14,   -0.16f,		0.0f, 2.0f, 0.0f,	0.0f, 0.0f, //13 Top Front Right Corner
		-0.77f, 0.14f,  -0.3f,		0.0f, 2.0f, 0.0f,	1.0f, 1.0f, //9 Top Front Left Upper Panel Corner
	};
	glm::vec3 panelsPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	
	glGenVertexArrays(1, &VAO11);
	glGenBuffers(1, &VBO11);
	glBindVertexArray(VAO11);
	glBindBuffer(GL_ARRAY_BUFFER, VBO11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertPanels), vertPanels, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	
	//Foot Rest Top 12
	float vertsFootRestTop[] =
	{
	-1.5f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //TL
	-1.21f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TR
	-1.5f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BL
	-1.21f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TR
	-1.5f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BL
	-1.21f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f //BR
	};
	glm::vec3 footTopPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};

	glGenVertexArrays(1, &VAO12);
	glGenBuffers(1, &VBO12);
	glBindVertexArray(VAO12);
	glBindBuffer(GL_ARRAY_BUFFER, VBO12);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsFootRestTop), vertsFootRestTop, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Foot Rest Arms & Sides 13
	float vertsFootRestSides[] = {
				//Left
		-1.5f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TLU
		-1.5f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //BLU
		-1.5f, 0.75f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TLL
		-1.5f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //BLU
		-1.5f, 0.75f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TLL
		-1.5f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //BLL
				//Right
		-1.21f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BRU
		-1.21f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BRL
		-1.21f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //TRU
		-1.21f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BRL
		-1.21f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //TRU
		-1.21f, 0.75f, -0.75f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //TRL
				//Back
		-1.5f, 0.75f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TLL
		-1.5f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TLU
		-1.21f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //TRU
		-1.21f, 0.75f, -0.75f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //TRL
		-1.21f, 0.792f, -0.75f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //TRU
		-1.5f, 0.75f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //TLL
				//Front
		-1.5f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //BLL
		-1.5f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //BLU
		-1.21f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BRU
		-1.21f, 0.792f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BRU
		-1.21f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //BRL
		-1.5f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f //BLL

	};
	glm::vec3 sidesPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};

	glGenVertexArrays(1, &VAO13);
	glGenBuffers(1, &VBO13);
	glBindVertexArray(VAO13);
	glBindBuffer(GL_ARRAY_BUFFER, VBO13);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsFootRestSides), vertsFootRestSides, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	//Bottle Base 14
	glGenVertexArrays(1, &VAO14);
	glGenBuffers(1, &VBO14);
	glBindVertexArray(VAO14);
	glBindBuffer(GL_ARRAY_BUFFER, VBO14);
	glm::vec3 bbPositions[] = {
	glm::vec3(1.5f,  0.25f,  -0.2f)
	};
	//Bootle Cone 15
	glGenVertexArrays(1, &VAO15);
	glGenBuffers(1, &VBO15);
	glBindVertexArray(VAO15);
	glBindBuffer(GL_ARRAY_BUFFER, VBO15);
	glm::vec3 bcnPositions[] = {
	glm::vec3(1.5f,  0.58f,  -0.2f)
	};
	//Bottle Neck 16
	glGenVertexArrays(1, &VAO16);
	glGenBuffers(1, &VBO16);
	glBindVertexArray(VAO16);
	glBindBuffer(GL_ARRAY_BUFFER, VBO16);
	glm::vec3 bnPositions[] = {
	glm::vec3(1.5f,  0.8f,  -0.2f)
	};
	
	//Bottle Cork 17
	glGenVertexArrays(1, &VAO17);
	glGenBuffers(1, &VBO17);
	glBindVertexArray(VAO17);
	glBindBuffer(GL_ARRAY_BUFFER, VBO17);
	glm::vec3 bcPositions[] = {
	glm::vec3(1.5f,  0.85f,  -0.2f)
	};

	//Foot Rest Arms 18
	float vertsFootRestArms[] = {
		//Arms
	-1.5f, 0.75, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f,
	-1.5f, 0.75f, -0.7f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.5f, 0.0f, 0.08f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f,

	-1.5f, 0.0f, 0.08f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f,
	-1.5f, 0.75f, -0.7f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.5f, 0.0f, 0.03f,			0.0f, 2.0f, 0.0f,		0.0f, 0.0f,

	-1.5f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.5f, 0.75f, 0.03f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f,
	-1.5f, 0.0f, -0.7f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f,

	-1.5f, 0.0f, -0.7f,			0.0f, 2.0f, 0.0f,		1.0f, 0.0f,
	-1.5f, 0.75f, 0.03f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f,
	-1.5f, 0.0f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f,

	-1.21f, 0.75, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f,
	-1.21f, 0.75f, -0.7f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.21f, 0.0f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f,

	-1.21f, 0.0f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f,
	-1.21f, 0.75f, -0.7f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.21f, 0.0f, 0.03f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f,

	-1.21f, 0.75f, 0.08f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f,
	-1.21f, 0.75f, 0.03f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.21f, 0.0f, -0.7f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f,

	-1.21f, 0.0f, -0.7f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f,
	-1.21f, 0.75f, 0.03f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f,
	-1.21f, 0.0f, -0.75f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f
	};
	glm::vec3 armsPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};

	glGenVertexArrays(1, &VAO18);
	glGenBuffers(1, &VBO18);
	glBindVertexArray(VAO18);
	glBindBuffer(GL_ARRAY_BUFFER, VBO18);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsFootRestArms), vertsFootRestArms, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Pencil Shaft 19
	glGenVertexArrays(1, &VAO19);
	glGenBuffers(1, &VBO19);
	glBindVertexArray(VAO19);
	glBindBuffer(GL_ARRAY_BUFFER, VBO19);
	glm::vec3 psPositions[] = {
	glm::vec3(-1.35f,  0.93f,  -0.4f)
	};

	//Pencil Tip 20
	glGenVertexArrays(1, &VAO20);
	glGenBuffers(1, &VBO20);
	glBindVertexArray(VAO20);
	glBindBuffer(GL_ARRAY_BUFFER, VBO20);
	glm::vec3 ptPositions[] = {
	glm::vec3(-1.2813f,  1.093f,  -0.51f)
	};

	//Walls 21
	float vertsWalls[] = {
			//Front
		 15.0f, 9.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //0
		 15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //1
		-15.0f, 9.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //3

		 15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //1
		-15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //2	
		-15.0f, 9.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //3

		//Back
		 15.0f, 9.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //4
		 15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //5
		-15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //6

		-15.0f, 9.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //7
		-15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //6
		 15.0f, 9.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //4

		 //Right
		 15.0f, 9.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //0
		 15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //1
		 15.0f, 9.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //4

		 15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //5
		 15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //1
		 15.0f, 9.0f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //4

		 //Left
		-15.0f, -3.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //2	
		-15.0f, 9.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //3
		-15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //6

		-15.0f, 9.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 1.0f, //7
		-15.0f, 9.0f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 1.0f, //3
		-15.0f, -3.0f, -15.0f,		0.0f, 2.0f, 0.0f,		1.0f, 0.0f, //6

	};
	glm::vec3 wallsPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};

	glGenVertexArrays(1, &VAO21);
	glGenBuffers(1, &VBO21);
	glBindVertexArray(VAO21);
	glBindBuffer(GL_ARRAY_BUFFER, VBO21);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsWalls), vertsWalls, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//Cieling 22
	float vertsCieling[] = {
	15.0f, 9.01f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //0
	15.0f, 9.01f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //1
	-15.0f, 9.01f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //2
	
	15.0f, 9.01f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //1
	-15.0f, 9.01f, -15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //2
	-15.0f, 9.01f, 15.0f,		0.0f, 2.0f, 0.0f,		0.0f, 0.0f, //3

	};
	glm::vec3 cielingPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	

	glGenVertexArrays(1, &VAO22);
	glGenBuffers(1, &VBO22);
	glBindVertexArray(VAO22);
	glBindBuffer(GL_ARRAY_BUFFER, VBO22);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsCieling), vertsCieling, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//Guitar Body q1
	float vertsGBodyq1[] = {
		 0.0f, 0.0f, 0.0f,   0.0f, 2.0f, 0.0f,  1.0f,0.0f,  // Body Apex 0
		 0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f,  0.0f, 0.0f,
		 .0042f, .025f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .0042f, .025f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,   //.05
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0083f, .0417f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .0083f, .0417f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//.1
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0167f, .0625f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .0167f, .0625f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//.2
		 0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f,  0.0f, 0.0f,
		 0.025f, 0.0833f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .025f, .0833f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//.3
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0292f, .104f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		 .0292f, .104f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//.35
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0333f, 0.125f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .0333f, .125f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//.4
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .042f, .146f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .042f, .146f, 0.0f,0.0f, 2.0f, 0.0f,1.0f, 0.0f,//.5
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .05f,.1667f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .05f, .1667f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//.6
		 0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		 .0542f, .1875f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		 .0542f, .1875f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//.65
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0625f, .2083f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		 .0625f, .2083f, 0.0f,0.0f, 2.0f, 0.0f,1.0f, 0.0f,//.75
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0666f, .2167f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,
	};
	
	glm::vec3 q1Positions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f)
	};
		
	glGenVertexArrays(1, &VAO23);
	glGenBuffers(1, &VBO23);
	glBindVertexArray(VAO23);
	glBindBuffer(GL_ARRAY_BUFFER, VBO23);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq1), vertsGBodyq1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q1a
	float vertsGBodyq1a[] = {

		 .0666f, .2167f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//.8
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .075f, .2292f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .075f, .2292f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//.9+
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .08333f, .25f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .08333f, .25f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//1
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .0958f, .271f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		 .0958f, .271f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//1.15
		 0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f,  0.0f, 0.0f,
		 .1f, .2792f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .1f, .2792f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//1.2
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1083f, .2917f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.00f,

		 .1083f, .2917f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//1.3
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1167f, .3f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		 .1167f, .3f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//1.4
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .125f, .3125f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .125f, .3125f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//1.5
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1375f, .3333f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .1375f, .3333f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//1.65
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1417f, .3417f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .1417f, .3417f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//1.7
		 0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f,  0.0f, 0.0f,
		 .15f, .35f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .15f, .35f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//1.8
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1583f, .3542f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .1583f, .3542f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//1.9
		 0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		 .1625f, .375f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 q1aPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO23a);
	glGenBuffers(1, &VBO23);
	glBindVertexArray(VAO23a);
	glBindBuffer(GL_ARRAY_BUFFER, VBO23);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq1a), vertsGBodyq1a, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q1b
	float vertsGBodyq1b[] = {

		.1625f, .375f, 0.0f, 0.0f, 2.0f, 0.0f,1.0f, 0.0f,//1.95
		 0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		 .1667f, .3792f, 0.0,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,


		  .1667f, .3792f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//2
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		.175f, .3833f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		  .175f, .3833f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//2.1
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1833f, .3958f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .1833f, .3958f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//2.2
		  0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .1917f, .4f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		  .1917f, .4f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//2.3
		  0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		  .2f, .4083f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .2f, .4083f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,   //2.4
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .2083f, .4167f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .2083f, .4167f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,///2.5
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .2167f, .425f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .2167f, .425f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //2.6
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .225f, .4333f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .225f, .4333f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//2.7
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .2333f, .4375f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		 .2333f, .4375f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//2.8
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.2417f, .4458f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.2417f, .4458f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //2.9
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.25f, .45f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

	};

	glm::vec3 q1bPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO23b);
	glGenBuffers(1, &VBO23);
	glBindVertexArray(VAO23b);
	glBindBuffer(GL_ARRAY_BUFFER, VBO23);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq1b), vertsGBodyq1b, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q1b
	float vertsGBodyq1c[] = {

		  .25f, .45f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//3.0
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.2583f, .4583f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.2583f, .4583f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//3.1
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.2667f, .4625f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.2667f, .4625f, 0.0f, 0.0f, 2.0f, 0.0f, 1.00f, 0.0f, //3.2
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.275f, .4667f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.275f, .4667f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //3.3
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.2833f, .4708f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.2833f, .4708f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //3.4
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.2917f, .475f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.2917f, .475f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //3.5
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.3f, .4792f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.3f, .4792f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//3.6
		 0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.3083f, .4833f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.3083f, .4833f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//3.7
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.31667f, .485f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.31667f, .485f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //3.8
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.325f, .4867f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		.325f, .4867f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, //3.9
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.3333f, 0.4883f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,
	};

	glm::vec3 q1cPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO23c);
	glGenBuffers(1, &VBO23);
	glBindVertexArray(VAO23c);
	glBindBuffer(GL_ARRAY_BUFFER, VBO23);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq1c), vertsGBodyq1c, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q1d
	float vertsGBodyq1d[] = {

	0.3333f, 0.4883f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//4
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.3417f, 0.49f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.3417f, 0.49f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.1
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.35f, .4917f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.35f,0.4917f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.2
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.3583f, .4933f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.3583f, 0.4933f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.3
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.3667f, .495f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.3667f, 0.495f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.4
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.375f, .4967f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.375f, 0.4967f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.5
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
	   .3833f, .49708f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.3833f, 0.49708f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.6
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.3917f, .4975f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.3917f, 0.4975f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.7
	   0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.4f, .4983f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.4f, 0.4983f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.8
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		 .4083f, .4992f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.4083f, 0.4992f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,//4.9
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		.4167f, .5f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f //5

	};

	glm::vec3 q1dPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO23d);
	glGenBuffers(1, &VBO23);
	glBindVertexArray(VAO23d);
	glBindBuffer(GL_ARRAY_BUFFER, VBO23);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq1d), vertsGBodyq1d, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//Guitar Body q2
	float vertsGBodyq2[] = {
		0.00f,0.00f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//0
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0042f,-0.0208f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,


		0.0042f,-0.0208f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.05
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		0.0083f,-0.0417f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0083f,-0.0417f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.1
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.01f,-0.0625f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0100f,-0.0625f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.12
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0125f,-0.0833f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.0125f,-0.0833f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.15
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0175f,-0.1042f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0175f,-0.1042f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.21
		0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f,  0.0f, 0.0f,
		0.025f,-0.125f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.0250f,-0.1250f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.3
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0292f,-0.1458f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.0292f,-0.1458f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.35
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0333f,-0.1542f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0333f,-0.1542f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.4
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0417f,-0.1667f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.0417f,-0.1667f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.5
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0467f,-0.1875f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0467f,-0.1875f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.56
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		0.05f,-0.2083f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

	};

	glm::vec3 q2Positions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO24);
	glGenBuffers(1, &VBO24);
	glBindVertexArray(VAO24);
	glBindBuffer(GL_ARRAY_BUFFER, VBO24);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq2), vertsGBodyq2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q2a
	float vertsGBodyq2a[] = {
  0.0500f,-0.2083f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.6
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0542f,-0.2292f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0542f,-0.2292f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.65
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.0625f,-0.25f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.0625f,-0.2500f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.75
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.06667f,-0.2708f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0667f,-0.2708f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.8
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		0.075f,-0.2833f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.0750f,-0.2833f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//.9
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		0.0833f,-0.2917f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0833f,-0.2917f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		0.09167f,-0.3125f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.0917f,-0.3125f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.1
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.1f,-0.3333f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.1000f,-0.3333f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.2
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.1083f,-0.34167f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.1083f,-0.3417f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.3
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.1167f,-0.35f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.1167f,-0.3500f,0.0f,1.0f,0.0f,//1.4
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.125f,-0.3542f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		
		
	};

	glm::vec3 q2aPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO24a);
	glGenBuffers(1, &VBO24);
	glBindVertexArray(VAO24a);
	glBindBuffer(GL_ARRAY_BUFFER, VBO24);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq2a), vertsGBodyq2a, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q2b
	float vertsGBodyq2b[] = {
		
	
		0.1250f,-0.3542f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.5
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
		0.1333f,-0.375f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.1333f,-0.3750f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.6
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
		0.1417f,-0.3833f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,


		0.1417f,-0.3833f,0.0f, 0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.7
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.15f, -0.3958f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		0.1500f,-0.3958f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.8
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.1583f, -0.4042f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.1583f,-0.4042f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//1.9
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.1667f, -0.4167f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.1667f,-0.4167f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.175f, -0.425f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.1750f,-0.4250f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.1
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.1833f, -0.4292f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

		0.1833f,-0.4292f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.2
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.1917f, -0.4333f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,


		

	

	};

	glm::vec3 q2bPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO24b);
	glGenBuffers(1, &VBO24);
	glBindVertexArray(VAO24b);
	glBindBuffer(GL_ARRAY_BUFFER, VBO24);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq2b), vertsGBodyq2b, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q2c
	float vertsGBodyq2c[] = {

		0.1917f,-0.4333f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.3
		0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.2f, -0.4375f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		0.2000f,-0.4375f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.4
		0.3375f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.2083f, -0.4417f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			0.2083f,-0.4417f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.5
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.2167f,-0.4458f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.2167f,-0.4458f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.6
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.225f, -0.45f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		    0.2250f,-0.4500f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.7
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.2333f, -0.4583f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.2333f,-0.4583f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.8
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.2417f, -0.46f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.2417f,-0.4600f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//2.9
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.25f, -0.4617f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.2500f,-0.4617f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.2583f, -0.4633f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

		
			


	};

	glm::vec3 q2cPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO24c);
	glGenBuffers(1, &VBO24);
	glBindVertexArray(VAO24c);
	glBindBuffer(GL_ARRAY_BUFFER, VBO24);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq2c), vertsGBodyq2c, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Guitar Body q2d
	float vertsGBodyq2d[] = {

			0.2583f,-0.4633f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.1
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.2667f, -0.465f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.2667f,-0.4650f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.2
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.275f, -0.4667f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.2750f,-0.4667f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.3
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.2833f, -0.4683f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.2833f,-0.4683f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.4
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.2917f, -0.47f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.2917f,-0.4700f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.5
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.3f, -0.4717f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.3000f,-0.4717f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.6
			0.3375f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 0.0f,
			0.3083f, -0.4733f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

			0.3083f,-0.4733f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.7
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.31667f, -0.475f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 1.0f,
			0.3167f,-0.4750f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.8
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.325f, -0.4767f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.3250f,-0.4767f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//3.9
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.333f, -0.4792f, 0.0f, 0.0f, 2.0f, 0.0f,0.0f, 1.0f,

			0.3333f,-0.4792f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//4.0
			0.3375f, 0.0f, 0.0f,0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
			0.3375f,-0.5000f,0.0f,0.0f, 2.0f, 0.0f,1.0f,0.0f,//4.05

		

	};

	glm::vec3 q2dPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO24d);
	glGenBuffers(1, &VBO24);
	glBindVertexArray(VAO24d);
	glBindBuffer(GL_ARRAY_BUFFER, VBO24);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq2d), vertsGBodyq2d, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//GuitarBodyq3
	float vertsGBodyq3[] = {

		 0.4167f,0.5f, 0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f, //5
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.4583f,0.4983f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.4583f,0.4983f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//5.5
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.5f,0.4958f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.5f,0.4958f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//6
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.5417f,0.4792f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.5417f,0.4792f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f, //6.5
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.5833f,0.458f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.5833f,0.458f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//7
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.625f,0.4167f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.625f,0.4167f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//7.5
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.6667f,0.4f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.6667f,0.4f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//8
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.7083f,0.3667f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		0.7083f,0.3667f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//8.5
			0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.75f,0.35f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		0.75f,0.35f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//9
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		0.7917f,0.3333f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		0.7917f,0.3333f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//9.5
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		0.8333f,0.3292f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

	};

	glm::vec3 q3Positions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO25);
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq3), vertsGBodyq3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	float vertsGBodyq3a[] = {
		0.8333f,0.3292f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//10
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		0.875f,0.3208f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.875f,0.3208f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//10.5
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		0.9167f,0.325f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		0.9167f,0.325f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//11
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		0.9583f,0.3292f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		0.9583f,0.3292f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//11.5
		.75f, 0.0f,  0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.0f,0.3333f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		1.0f,0.3333f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//12
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.0417f,0.35f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		1.0417f,0.35f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//12.5
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.083f,0.3583f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//13


		1.083f,0.3583f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//13
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.125f,0.383f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//13.5


		1.125f,0.383f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//13.5
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.1667f,0.4f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14

	};

	glm::vec3 q3aPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO25a);
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25a);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq3a), vertsGBodyq3a, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGBodyq3b[] = {
		

		1.1667f,0.4f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.2083f,0.4083f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		1.2083f,0.4083f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14.5
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.25f,0.4208f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		 1.25f, 0.4208f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//15
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.292f,0.425f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		1.292f, 0.425f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//15.5
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.333f,0.42917f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		1.333f, 0.42917f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//16
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.375f, 0.4167f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//16.5

	};

	glm::vec3 q3bPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO25b);
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25b);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq3b), vertsGBodyq3b, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGBodyq3c[] = {



		
		1.375f, 0.4167f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//16.5
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.4167f,0.4083f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,

		1.4167f, 0.4083f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//17
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.4375f,0.39583f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,

		1.4375f, 0.39583f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//17.25
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.4583f,0.375f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//17.5

		1.4583f, 0.375f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//17.5
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.47917f,0.35417f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//17.75

		1.47917f, 0.35417f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//17.75
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.5f,0.3333f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//18

		1.5f, 0.3333f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//18
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.5f,0.29167f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//18L
	};

	glm::vec3 q3cPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO25c);
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25c);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq3c), vertsGBodyq3c, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGBodyq3d[] = {

		1.5f,0.29167f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//18L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.4583f, 0.2833f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//17.5L
		
		1.4583f, 0.2833f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//17.5L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.4167f, 0.2667f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//17L
		
		1.4167f, 0.2667f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//17L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.375f, 0.2625f, 0.0f, 0.0f,2.0f, 0.0f, 1.0f, 0.0f,//16.5L

		1.375f, 0.2625f, 0.0f, 0.0f,2.0f, 0.0f, 1.0f, 0.0f,//16.5L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.3333f, 0.2583f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//16L

		1.3333f, 0.2583f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//16L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.2917f, 0.2417f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//15.5L

		1.2917f, 0.2417f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//15.5L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.25f, 0.2292f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//15L

		1.25f, 0.2292f, 0.0f,0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//15L
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.2083f,0.2f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//14.5l

		1.2083f,0.2f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//14.5l
		1.1667f, 0.3f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.1667f,0.1792f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14l
		
	};

	glm::vec3 q3dPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO25d);
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25d);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq3d), vertsGBodyq3d, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGBodyq3e[] = {

	
		1.1667f,0.1792f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14l
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.125f,0.133f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//13.5L

		1.125f,0.133f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f,//13.5L
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.083f,0.083f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f, //13L

		1.083f,0.083f,0.0f, 0.0f,2.0f,0.0f,1.0f,0.0f, //13L
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.075f,0.0625f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//12.9

		1.075f,0.0625f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//12.9
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.0625f,0.0f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//12.75

		1.1667f,0.1792f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14l
		.75f, 0.0f, 0.0f,0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.1667f,0.4f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//14

		0.7917f,0.3333f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//9.5
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.0625f,0.0f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//12.75

		1.0417f,-0.0333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.5
		.75f, 0.0f, 0.0f, 0.00f, 2.0f, 0.0f, 1.0f, 1.0f,//Pivot
		1.0625f,0.0f, 0.0f,0.0f,2.0f,0.0f,1.0f,0.0f,//12.75


	};

	glm::vec3 q3ePositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO25e);
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25e);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq3e), vertsGBodyq3e, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	float vertsGBodyq4[] = {
		0.3333f,-0.5f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//4
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.375f,-0.4917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//4.5

		0.375f,-0.4917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//4.5
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.4167f,-0.5f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//5

		0.4167f,-0.5f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//5
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.4583f,-0.4917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//5.5

		0.4583f,-0.4917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//5.5
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.5f,-0.4833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//6

		0.5f,-0.4833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//6
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.5417f,-0.4667f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//6.5

		0.5417f,-0.4667f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//6.5
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.5833f,-0.4458f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//7

		0.5833f,-0.4458f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//7
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.6167f,-0.4333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//7.4

		0.6167f,-0.4333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//7.4
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.6667f,-0.4f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8
		
	};

	glm::vec3 q4Positions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO26);
	glGenBuffers(1, &VBO26);
	glBindVertexArray(VAO26);
	glBindBuffer(GL_ARRAY_BUFFER, VBO26);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq4), vertsGBodyq4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	float vertsGBodyq4a[] = {
	0.7083f,-0.375f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.5
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.725f,-0.3542f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.7

	0.725f,-0.3542f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.7
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.75f,-0.3417f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//9

	0.75f,-0.3417f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//9
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.7917f,-0.3333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//9.5

	0.7917f,-0.3333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//9.5
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.8333f,-0.325f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//10

	0.8333f,-0.325f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//10
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.875f,-0.3333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//10.5

	0.875f,-0.3333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//10.5
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.9167f,-0.3417f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//11

	0.9167f,-0.3417f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//11
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	0.9625f,-0.3583f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//11.55

	0.9625f,-0.3583f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//11.55
	0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
	1.0f,-0.375f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12


	};

	glm::vec3 q4aPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO26a);
	glGenBuffers(1, &VBO26);
	glBindVertexArray(VAO26a);
	glBindBuffer(GL_ARRAY_BUFFER, VBO26);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq4a), vertsGBodyq4a, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGBodyq4b[] = {
	
		0.6667f,-0.4f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.675f,-0.4f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.1

		0.675f,-0.4f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.1
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.6833f,-0.3917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.2

		0.6833f,-0.3917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.2
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.6917f,-0.3833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.3

		0.6917f,-0.3833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.3
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.7f,-0.3792f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.4

		0.7f,-0.3792f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.4
		0.3333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		0.7083f,-0.375f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//8.5

		1.0f,-0.375f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12
		0.5333f,0.0f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0417f,-0.3917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.5
						
		1.0417f,-0.3917f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.5
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0833f,-0.3833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13

		1.0833f,-0.3833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.1042f,-0.375f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13.25


	};

	glm::vec3 q4bPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO26b);
	glGenBuffers(1, &VBO26);
	glBindVertexArray(VAO26b);
	glBindBuffer(GL_ARRAY_BUFFER, VBO26);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq4b), vertsGBodyq4b, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGBodyq4c[] = {

		1.0417f,-0.0333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.5
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0125f,-0.1167f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.15

		1.0125f,-0.1167f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.15
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0083f,-0.1458f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.1

		1.0083f,-0.1458f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.1
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0167f,-0.2f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.2

		1.0167f,-0.2f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.2
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0417f,-0.2458f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.5

		1.0417f,-0.2458f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.5
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.05f,-0.2833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.6

		1.05f,-0.2833f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//12.6
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.0833f,-0.3083f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13

		1.0833f,-0.3083f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.125f,-0.3417f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13.5

		1.125f,-0.3417f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13.5
		0.8333f,-0.1667f,0.0f,0.0f,2.0f,0.0f,1.0f,1.0f,//Pivot
		1.1042f,-0.3792f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,//13.25



	};

	glm::vec3 q4cPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.5f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO26c);
	glGenBuffers(1, &VBO26);
	glBindVertexArray(VAO26c);
	glBindBuffer(GL_ARRAY_BUFFER, VBO26);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGBodyq4c), vertsGBodyq4c, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	


	float vertsGneck[] = {
	1.5833f, 0.1667f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 
	1.5833f, 0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
	1.5833f, 0.1667f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.1667f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

	};

	glm::vec3 gNeckPositions[] = {
	glm::vec3(-1.0f,  0.4f,  0.51f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO27);
	glGenBuffers(1, &VBO27);
	glBindVertexArray(VAO27);
	glBindBuffer(GL_ARRAY_BUFFER, VBO27);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGneck), vertsGneck, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGneck2[] = {
	1.5833f, 0.125f, -0.0416667f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
	1.5833f, 0.0416f,-.0416667f, 0.0f,  2.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0416f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 0.0416f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
	1.5833f, 0.125f, -.0416667f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.125f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

	};

	glm::vec3 gNeck2Positions[] = {
	glm::vec3(-1.0f,  0.4f,  0.51f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO27a);
	glGenBuffers(1, &VBO27);
	glBindVertexArray(VAO27a);
	glBindBuffer(GL_ARRAY_BUFFER, VBO27);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGneck2), vertsGneck2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsGneck3[] = {
	1.5833f, 0.1667f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.1667f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.125f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,

	0.0f, 0.125f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f,
	1.5833f, 0.1667f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
	1.5833f, 0.125f, -.0416667f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,


	1.5833f, 0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0416f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,


	0.0f, 0.0416f, -.0416667f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f,
	1.5833f, 0.0416f,-.0416667f, 0.0f,  2.0f, 0.0f, 1.0f, 0.0f,
	1.5833f, 0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
	};

	glm::vec3 gNeck3Positions[] = {
	glm::vec3(-1.0f,  0.4f,  0.51f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO27b);
	glGenBuffers(1, &VBO27);
	glBindVertexArray(VAO27b);
	glBindBuffer(GL_ARRAY_BUFFER, VBO27);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsGneck3), vertsGneck3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	float vertsHead[] = {
	
	
	0.0f,0.2292f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.0f,0.0625f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0
	
	0.0f,0.0625f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.0667f,0.0125f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0.8
	
	0.0667f,0.0125f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0.8
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.15f,0.0042f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//1.8

	0.15f,0.0042f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//1.8
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.2167f,0.0167f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//2.6

	0.2167f,0.0167f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//2.6
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.325f,0.0417f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//3.9

	0.325f,0.0417f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//3.9
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.3458f,0.025f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//4.15

	0.3458f,0.025f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//4.15
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.3583f,0.0f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//4.3

	0.3583f,0.0f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//4.3
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.4167f,0.0125f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5

	0.4167f,0.0125f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.45f,0.0417f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.4

	0

	};

	glm::vec3 gHeadPositions[] = {
	glm::vec3(0.55f,  0.35f,  0.51f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO28);
	glGenBuffers(1, &VBO28);
	glBindVertexArray(VAO28);
	glBindBuffer(GL_ARRAY_BUFFER, VBO28);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsHead), vertsHead, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsHead2[] = {


	.45f,0.0417f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.4
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.4583f,0.0583f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.5

	0.4583f,0.0583f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.5
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.4542f,0.075f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.45

	0.4542f,0.075f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.45
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.45f,0.1042f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.4

	0.45f,0.1042f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.4
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.425f,0.1333f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.1

	0.425f,0.1333f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//5.1
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.3417f,0.1375f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//4.1

	0.3417f,0.1375f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//4.1
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.1083f,0.2417f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//1.3

	0.1083f,0.2417f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//1.3
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.0667f,0.25f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0.8

	0.0667f,0.25f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0.8
	.2292f, .125f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, //Pivot
	0.0f,0.2292f,0.0f,0.0f,2.0f,0.0f, 1.0f, 0.0f,//0



	};

	glm::vec3 gHead2Positions[] = {
	glm::vec3(0.55f,  0.35f,  0.51f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO28a);
	glGenBuffers(1, &VBO28);
	glBindVertexArray(VAO28a);
	glBindBuffer(GL_ARRAY_BUFFER, VBO28);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsHead2), vertsHead2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertsBodyTop[] = {

		0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		.4167f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		.4167f, .5000f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		.4167f, 0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		.4167f, -.5000f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		.4167f, 0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		.4167f, .5f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		.9167f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		.4167f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		.4167f, -.5f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		.9167f,  0.0f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		.75f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		.6250f, .4167f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		.7917f, .3333f,0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,

		1.0f, -.3333f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		.4167f, -.3333f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		.9167f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,



	};

	glm::vec3 gBodyTopPositions[] = {
	glm::vec3(-2.0f,  0.5f,  0.51f),
	glm::vec3(0.0f,  0.0f,  -0.083f)
	};

	glGenVertexArrays(1, &VAO29);
	glGenBuffers(1, &VBO29);
	glBindVertexArray(VAO29);
	glBindBuffer(GL_ARRAY_BUFFER, VBO29);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsBodyTop), vertsBodyTop, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap = loadTexture("images/LightWoodTableGrain.jpg");
	unsigned int specularMap = loadTexture("images/DaRKWoodTableGrain.jpg");
	unsigned int texture2 = loadTexture("images/kemperback.png");
	unsigned int texture3 = loadTexture("images/greenmetal.png");
	unsigned int texture4 = loadTexture("images/glasstexture.jpg");
	unsigned int texture5 = loadTexture("images/MusicScore.png");
	unsigned int texture6 = loadTexture("images/PageEdges.jpg");
	unsigned int texture7 = loadTexture("images/hardwoodfloor.jpg");
	unsigned int texture8 = loadTexture("images/KemperTop.png");
	unsigned int texture9 = loadTexture("images/OdinsSkull.png");
	unsigned int texture10 = loadTexture("images/whiteCeramic.jpg");
	unsigned int texture11 = loadTexture("images/cork.png");
	unsigned int texture12 = loadTexture("images/footresttop.png");
	unsigned int texture13 = loadTexture("images/blackmetal.png");
	unsigned int texture14 = loadTexture("images/blackplastic.jpg");
	unsigned int texture15 = loadTexture("images/shinymetal.jpg");
	unsigned int texture16 = loadTexture("images/ShipLap.png");
	unsigned int texture17 = loadTexture("images/AcousticCieling.png");
	unsigned int texture18 = loadTexture("images/BlueWood.jpg");
	//unsigned int texture19 = loadTexture("images/greenmetal.png");


	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// render containers
		glBindVertexArray(cubeVAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::translate(model, cubePositions[0]);
		float angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// also draw the lamp object(s)
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.6f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Render VAO2
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, kbPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Render VAO3
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO3);
		model = glm::mat4(1.0f);
		model = glm::translate(model, ksPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Render VAO4
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO4);
		model = glm::mat4(1.0f);
		model = glm::translate(model, krPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO5
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glassPositions[0]);
		lightingShader.setMat4("model", model);
		static_meshes_3D::OpenCylinder Glass(0.125, 14, 0.25, true, true, true);
		Glass.render();

		//Render VAO6
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glBindVertexArray(VAO6);
		model = glm::mat4(1.0f);
		model = glm::translate(model, smPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO7
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glBindVertexArray(VAO7);
		model = glm::mat4(1.0f);
		model = glm::translate(model, smsPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO8
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture7);
		glBindVertexArray(VAO8);
		model = glm::mat4(1.0f);
		model = glm::translate(model, floorPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO9
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glBindVertexArray(VAO9);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4(1.0f);
			model = glm::translate(model, legsPositions[i]);
			//float angle = 20.0f * i;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Render VAO10
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture8);
		glBindVertexArray(VAO10);
		model = glm::mat4(1.0f);
		model = glm::translate(model, ktPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO11
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO11);
		model = glm::mat4(1.0f);
		model = glm::translate(model, panelsPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO12
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture12);
		glBindVertexArray(VAO12);
		model = glm::mat4(1.0f);
		model = glm::translate(model, footTopPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO13
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture13);
		glBindVertexArray(VAO13);
		model = glm::mat4(1.0f);
		model = glm::translate(model, sidesPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO14
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture9);
		glBindVertexArray(VAO14);
		model = glm::mat4(1.0f);
		model = glm::translate(model, bbPositions[0]);
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder BottleBase(0.125, 14, 0.5, true, true, true);
		BottleBase.render();
		
		//Render VAO15
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO15);
		model = glm::mat4(1.0f);
		model = glm::translate(model, bcnPositions[0]);
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cone bottleCone(0.125, 14, 0.17, true, true, true);
		bottleCone.render();
		 
		
		//Render VAO16
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO16);
		model = glm::mat4(1.0f);
		model = glm::translate(model, bnPositions[0]);
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder BottleNeck(0.04, 14, 0.11, true, true, true);
		BottleNeck.render();
		
		//Render VAO17
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture11);
		glBindVertexArray(VAO17);
		model = glm::mat4(1.0f);
		model = glm::translate(model, bcPositions[0]);
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cylinder BottleCork(0.045, 14, 0.042, true, true, true);
		BottleCork.render();

		//Render VAO18
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture13);
		glBindVertexArray(VAO18);
		model = glm::mat4(1.0f);
		model = glm::translate(model, armsPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO19
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture14);
		glBindVertexArray(VAO19);
		for (unsigned int i = 0; i < 2; i++)
		{
			glm::mat4(1.0f);
			model = glm::translate(model, psPositions[i]);
			float angle = -40.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			static_meshes_3D::Cylinder PencilShaft(0.0098, 14, 0.42, true, true, true);
			PencilShaft.render();

		}
		
		//Render VAO20
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture15);
		glBindVertexArray(VAO20);
		model = glm::mat4(1.0f);
		model = glm::translate(model, ptPositions[0]);
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1, .3, .5));
		lightingShader.setMat4("model", model);
		static_meshes_3D::Cone PencilTip(0.0093, 14, 0.042, true, true, true);
		PencilTip.render();
		
		//Render VAO21
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture16);
		glBindVertexArray(VAO21);
		model = glm::mat4(1.0f);
		model = glm::translate(model, wallsPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Render VAO22
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO22);
		model = glm::mat4(1.0f);
		model = glm::translate(model, cielingPositions[0]);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		////Render VAO23
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO23);
		//for (unsigned int i = 0; i < 5; i++)
		//{
		//	glm::mat4(1.0f);
		//	model = glm::translate(model, q1Positions[i]);
		//	//float angle = 20.0f * i;
		//	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	lightingShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		///*model = glm::mat4(1.0f);
		//model = glm::translate(model, q1Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);*/

		////Render VAO23a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO23a);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q1aPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		////Render VAO23b
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO23b);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q1bPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO23c
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO23c);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q1cPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO23d
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO23d);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q1dPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO24
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO24);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q2Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO24a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO24a);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q2aPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO24b
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO24b);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q2bPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO24c
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO24c);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q2cPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO24d
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO24d);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q2dPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO25
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO25);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q3Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO25a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO25a);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q3aPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO25b
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO25b);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q3bPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO25c
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO25c);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q3cPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO25d
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO25d);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q3dPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO25e
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO25e);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q3ePositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		////Render VAO26
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO26);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q4Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO26a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO26a);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q4aPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO26b
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO26b);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q4bPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO26c
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO26c);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, q4cPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO27
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO27);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, gNeckPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO27a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO27a);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, gNeck2Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	
		////Render VAO27b
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO27b);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, gNeck3Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		////Render VAO28
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO28);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, gHeadPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO28a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO28a);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, gHead2Positions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render VAO28a
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 32.0f);
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("model", model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture18);
		//glBindVertexArray(VAO29);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, gBodyTopPositions[0]);
		//lightingShader.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);

	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);

	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);

	glDeleteVertexArrays(1, &VAO6);
	glDeleteBuffers(1, &VBO6);

	glDeleteVertexArrays(1, &VAO7);
	glDeleteBuffers(1, &VBO7);
	
	glDeleteVertexArrays(1, &VAO8);
	glDeleteBuffers(1, &VBO8);

	glDeleteVertexArrays(1, &VAO9);
	glDeleteBuffers(1, &VBO9);

	glDeleteVertexArrays(1, &VAO10);
	glDeleteBuffers(1, &VBO10);
	
	glDeleteVertexArrays(1, &VAO11);
	glDeleteBuffers(1, &VBO11);

	glDeleteVertexArrays(1, &VAO12);
	glDeleteBuffers(1, &VBO12);

	glDeleteVertexArrays(1, &VAO13);
	glDeleteBuffers(1, &VBO13);

	glDeleteVertexArrays(1, &VAO14);
	glDeleteBuffers(1, &VBO14);

	glDeleteVertexArrays(1, &VAO15);
	glDeleteBuffers(1, &VBO15);

	glDeleteVertexArrays(1, &VAO16);
	glDeleteBuffers(1, &VBO16);

	glDeleteVertexArrays(1, &VAO17);
	glDeleteBuffers(1, &VBO17);

	glDeleteVertexArrays(1, &VAO18);
	glDeleteBuffers(1, &VBO18);

	glDeleteVertexArrays(1, &VAO19);
	glDeleteBuffers(1, &VBO19);

	glDeleteVertexArrays(1, &VAO20);
	glDeleteBuffers(1, &VBO20);

	glDeleteVertexArrays(1, &VAO21);
	glDeleteBuffers(1, &VBO21);

	glDeleteVertexArrays(1, &VAO22);
	glDeleteBuffers(1, &VBO22);

	glDeleteVertexArrays(1, &VAO23);
	glDeleteVertexArrays(1, &VAO23a);
	glDeleteVertexArrays(1, &VAO23b);
	glDeleteVertexArrays(1, &VAO23c);
	glDeleteVertexArrays(1, &VAO23d);
	glDeleteBuffers(1, &VBO23);

	glDeleteVertexArrays(1, &VAO24);
	glDeleteVertexArrays(1, &VAO24a);
	glDeleteVertexArrays(1, &VAO24b);
	glDeleteVertexArrays(1, &VAO24c);
	glDeleteVertexArrays(1, &VAO24d);
	glDeleteBuffers(1, &VBO24);

	glDeleteVertexArrays(1, &VAO25);
	glDeleteVertexArrays(1, &VAO25a);
	glDeleteVertexArrays(1, &VAO25b);
	glDeleteVertexArrays(1, &VAO25c);
	glDeleteVertexArrays(1, &VAO25d);
	glDeleteVertexArrays(1, &VAO25e);
	glDeleteBuffers(1, &VBO25);

	glDeleteVertexArrays(1, &VAO26);
	glDeleteVertexArrays(1, &VAO26a);
	glDeleteVertexArrays(1, &VAO26b);
	glDeleteVertexArrays(1, &VAO26c);
	glDeleteBuffers(1, &VBO26);

	glDeleteVertexArrays(1, &VAO27);
	glDeleteVertexArrays(1, &VAO27a);
	glDeleteVertexArrays(1, &VAO27b);
	glDeleteBuffers(1, &VBO27);

	glDeleteVertexArrays(1, &VAO28);
	glDeleteVertexArrays(1, &VAO28a);
	glDeleteBuffers(1, &VBO28);

	glDeleteVertexArrays(1, &VAO29);
	glDeleteBuffers(1, &VBO29);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrComponents;

	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	flipImageVertically(data, width, height, nrComponents);

	if (data)
	{
		
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}