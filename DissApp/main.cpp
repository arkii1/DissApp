#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"
#include "Camera.h"

Camera camera;

Window mainWindow;

Texture brickTexture;

Material shinyMaterial;

Light directionalLight;

// Shader paths
static const char* vertexShader = "Shaders/shader.vert";
static const char* fragmentShader = "Shaders/shader.frag";

// Buffer objects
GLuint VAO, VBO, IBO;

// Mutiply to convert degrees to radians
const float toRadians = 3.14159265f / 180.0f;

// Used for time between frames
GLfloat deltaTime = 0.0f;
GLfloat timeLastFrame = 0.0f;

void CalculateAverageNormals(unsigned int * indicies, unsigned int indicieCount, GLfloat * vertices, unsigned verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indicieCount; i += 3)
	{
		// Gets one triangle from our model, gets 2 vectors from the plane, and uses cross product to get the normal of that plane
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

void CreateTriangle()
{
	// Indicies define the order of our indexed draw. E.g. first line we go vertex 0 -> 3 -> 1 in the vertices array below
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Define vertex prooperties (world position (xyz), texel coordinates (uv) and normal vector (nx,ny,nz))
	// x	  y	     z			 u		v			nx	  ny	nz
	GLfloat vertices[] = {
	  -1.0f, -1.0f, 0.0f,		0.0f, 0.0f,			0.0f, 0.0f, 0.0f,
	  0.0f, -1.0f, 1.0f,		0.5f, 0.0f,			0.0f, 0.0f, 0.0f,
	  1.0f,  -1.0f, 0.0f,		1.0f, 0.0f,			0.0f, 0.0f, 0.0f,
	  0.0f,   1.0f, 0.0f,		0.5f, 1.0f,			0.0f, 0.0f, 0.0f,
	};

 	CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * 32, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


int main()
{
	mainWindow = Window();
	mainWindow.Initialise();

	CreateTriangle();
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vertexShader, fragmentShader);

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.25f);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.GetAspectRatio(), 0.1f, 100.0f);

	brickTexture = Texture((char*)("Textures/brick.png"));
	brickTexture.LoadTexture();

	directionalLight = Light(1.0f, 1.0f, 1.0f, 0.25f,
		2.0f, -1.0f, -2.0f, 1.0f);

	shinyMaterial = Material(1.0f, 32);

	GLuint uniformModelTransform = 0;
	GLuint uniformProjection = 0;
	GLuint uniformAmbientColour = 0;
	GLuint uniformAmbientIntensity = 0;
	GLuint uniformDirection = 0;
	GLuint uniformDiffuseIntensity = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;
	GLuint uniformView = 0;
	GLuint uniformEyePosition = 0;

	float currentYaw = 0.0f;
	float tetrahedronRotateSpeed = 5.0f;

	while (!mainWindow.GetShouldWindowClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - timeLastFrame;
		timeLastFrame = now;

		glfwPollEvents();

		camera.HandleKeyMovement(mainWindow.GetHasKeyBeenPressed(), deltaTime);
		camera.HandleMouseMovement(mainWindow.GetXChange(), mainWindow.GetYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader1->UseShader();

		uniformModelTransform = shader1->GetUniformModelTransformLocation();
		uniformProjection = shader1->GetUniformProjection();
		uniformAmbientColour = shader1->GetUniformAmbientColourLocation();
		uniformAmbientIntensity = shader1->GetUniformAmbientIntesityLocation();
		uniformDirection = shader1->GetUniformDirectionLocation();
		uniformDiffuseIntensity = shader1->GetUniformDiffuseIntensityLocation();
		uniformView = shader1->GetUniformViewLocation();
		uniformEyePosition = shader1->GetUniformEyePositionLocation();
		uniformSpecularIntensity = shader1->GetUniformSpecularIntensityLocation();
		uniformShininess = shader1->GetUniformShininessLocation();

		directionalLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

		glm::mat4 transformationMatrix = glm::mat4(1.0f);

		currentYaw += deltaTime * tetrahedronRotateSpeed;

		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.0, 0.0, -5.0f));
		transformationMatrix = glm::rotate(transformationMatrix, currentYaw * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.4f, 0.4f, 0.0f));

		glUniformMatrix4fv(uniformModelTransform, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		
		brickTexture.UseTexture();

		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}
	return 0;
}
