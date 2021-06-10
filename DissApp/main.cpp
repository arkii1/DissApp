// Allows us to make use of stb
#define STB_IMAGE_IMPLEMENTATION

// Libraries
#include <stdio.h>
#include <string.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Our headers from classes we have created
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"
#include "Camera.h"

Camera camera;

// Variable for our window
Window mainWindow;

// Texture variable for brick png
Texture brickTexture;

// Variables for material
Material shinyMaterial;
Material dullMaterial;

// Variable for our light
Light directionalLight;

// Shader paths
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

// Buffer objects
GLuint VAO, VBO, IBO;

// Float to convert degrees to radians
const float toRadians = 3.14159265f / 180.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void CalculateAverageNormals(unsigned int * indicies, unsigned int indicieCount, GLfloat * vertices, unsigned verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indicieCount; i += 3)
	{
		// Get indicies and create a vector from them. Then find the normal using cross product
		unsigned int in0 = indicies[i] * vLength;
		unsigned int in1 = indicies[i + 1] * vLength;
		unsigned int in2 = indicies[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);

		normal = glm::normalize(normal);

		// Now jump to normals in our matrix then set those normals to calculated values
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	// Normalize normals
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

// Void which creates our triangle
void CreateTriangle()
{
	// Indicies define the order of our index draw. E.g. first line we go vertex 0 -> 3 -> 1 in the vertices array below
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Define world positions (x,y,z) and texture coordinates (u,v) and normals in x, y, and z plane. We keep these as 0 then calculate them in CalculateAverageNormal()
	// x	  y	     z			 u		v			nx	  ny	nz
	GLfloat vertices[] = {
	  -1.0f, -1.0f, 0.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
	  0.0f, -1.0f, 1.0f,		0.5f, 0.0f,			0.0f, 0.0f, 0.0f,
	  1.0f,  -1.0f, 0.0f,		1.0f, 0.0f,			0.0f, 0.0f, 0.0f,
	  0.0f,   1.0f, 0.0f,		0.5f, 1.0f,			0.0f, 0.0f, 0.0f,
	};

 	CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	// Generate 1 VAO and assign to our VAO variable
	glGenVertexArrays(1, &VAO);
	// Bind our VAO so we can now code with it. Important step as otherwise our GPU doesn't know which VAO to use
	glBindVertexArray(VAO);

	// Generate and bind IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Generate the IBO data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Same with VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Attach our vertices to our bound VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * 32, vertices, GL_STATIC_DRAW);

	// Define the format of the data of our VBO & enable it. So handles world position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);

	// Handles formatting of u,v values in our VAO
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	// Handles formatting of normals & enables
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	// Unbind VBO & VBO & IBO for clean slate next frame
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// When executed, main() is ran. Our entire program begins and ends here
int main()
{
	// Instantiate and initialise a window
	mainWindow = Window();
	mainWindow.Initialise();

	// Create a triangle and a shader program
	CreateTriangle();
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);

	// Create camera controller
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.25f);

	// Create and load texture
	brickTexture = Texture((char*)("Textures/brick.png"));
	brickTexture.LoadTexture();

	// Create material
	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(1.0f, 0);

	// Create light
	directionalLight = Light(1.0f, 1.0f, 1.0f, 0.25f,
		2.0f, -1.0f, -2.0f, 1.0f);

	// Create local uniform variable to use on shader
	GLuint uniformModel = 0;
	GLuint uniformProjection = 0;
	//Light & material variables
	GLuint uniformAmbientColour = 0;
	GLuint uniformAmbientIntensity = 0;
	GLuint uniformDirection = 0;
	GLuint uniformDiffuseIntensity = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;

	GLuint uniformView = 0;
	GLuint uniformEyePosition = 0;

	// Calculate our projection matrix - left, right, top and down plane are defined by our fov defined in first parameter. Manually define near and far plane in last two parameters
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.GetAspectRatio(), 0.1f, 100.0f);

	float yaw = 0.0f;
	float rotateSpeed = 5.0f;

	// Loop until window should be closed. Will be when user exits the window
	while (!mainWindow.GetShouldWindowClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events. Will tell us if we have clicked on exit window
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		// Clear window. Use every frame because we want to draw on a blank frame every time. We choose black as our background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clear colour buffer and depth buffer on our GPU
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		shader1->UseShader();

		// Get shader variables and set to local variables
		uniformModel = shader1->GetUniformModel();
		uniformProjection = shader1->GetUniformProjection();

		// Get light uniform variable
		uniformAmbientColour = shader1->GetUniformAmbientColourLocation();
		uniformAmbientIntensity = shader1->GetUniformAmbientIntesityLocation();
		uniformDirection = shader1->GetUniformDirectionLocation(); //>>>
		uniformDiffuseIntensity = shader1->GetUniformDiffuseIntensityLocation(); //>>>

		uniformView = shader1->GetUniformViewLocation();
		uniformEyePosition = shader1->GetUniformEyePositionLocation();

		// Get material uniform variables
		uniformSpecularIntensity = shader1->GetUniformSpecularIntensityLocation();
		uniformShininess = shader1->GetUniformShininessLocation();

		//Use our light 
		directionalLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

		//Create a matrix which will store the transforms we want to apply
		glm::mat4 model = glm::mat4(1.0f);

		yaw += deltaTime * rotateSpeed;

		// Method to translate our model. Comment it out if you don't want to apply it
		model = glm::translate(model, glm::vec3(0.0, 0.0, -5.0f));
		// Rotate our model. Note second parameter is amount in radians, and the vec3 is the axis we want to rotate around
		model = glm::rotate(model, yaw * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Method to scale model
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.0f));

		// This applies the model transform we calculated to our uniform model on the shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		
		// Use our texture
		brickTexture.UseTexture();

		// Use our material
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// Binds our VAO so we can draw a triangle using info stored on vertices from it
		glBindVertexArray(VAO);

		//Bind IBO, draw tetrahedron using indexed draws, then unbind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Unbind VAO
		glBindVertexArray(0);

		// Use no shader for 'clean slate' on next frame
		glUseProgram(0);

		// Swap buffers. A computer uses two frames: one to draw which is not viewable, and an already drawn frame which the user can see. When the drawing frame has finished, we swap the frames. 
		mainWindow.SwapBuffers();
	}
	return 0;
}
