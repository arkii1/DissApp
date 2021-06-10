#include "Shader.h"

Shader::Shader()
{
	// Set ID to 0, so we start with a new shader
	shaderID = 0;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	// Gets the code of vertex and fragment shader and converts to string variables
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	// Converts string to a const char* so we can compile it
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	// We will be returning content so initialize it here
	std::string content;
	// Initiate file stream
	std::ifstream fileStream(fileLocation, std::ios::in);

	// If file stream doesn't open, returns nothing and prints an error
	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist. \n", fileLocation);
		return "";
	}

	// This loops through our code until we are at the "end of file" (hence eof). We put the code into a varaible and add our content varaible
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	// Close the file strean and return content
	fileStream.close();
	return content;
}

// Here we create a program and add our vertex and fragment shaders. We then validate and link it so it is set up
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	// Use glCreateProgram to create a new shader program. Will return an id we assign to shaderID so we can access it
	shaderID = glCreateProgram();

	// If no ID then glCreateProgram failed, we print and error and return
	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	// Add the vertex and fragment shader to our program
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	// We use result to check that our program is both linked and validated. We will use eLog to print any errors if they occur
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Link our program. Print an error and return if linking fails
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program : '%s'\n", eLog);
		return;
	}

	// Validate our program. Print an error and return if validating fails
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program : '&s'\n", eLog);
		return;
	}

	// Assign shader uniform variables to private variables
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformAmbientColour = glGetUniformLocation(shaderID, "directionalLight.colour");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	// Create a shader of the type that was passed as a parameter
	GLuint theShader = glCreateShader(shaderType);

	// Create an array of the shader code
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	// Get the length of the shader code
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	// This replaces the source code in a shader object
	glShaderSource(theShader, 1, theCode, codeLength);
	// Compile the shader
	glCompileShader(theShader);

	// We use result to check that our shaders compiled. We will use eLog to print any errors if they occur
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Make sure shader is compiled. Print an error and return if not
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		printf("Error compiling the %d shader: '&s'\n", shaderType, eLog);
		if (shaderType == GL_VERTEX_SHADER)
			printf("Vertex shader");
		else if (shaderType == GL_FRAGMENT_SHADER)
			printf("Fragment shader");
		return;
	}

	// Attach shader to the program
	glAttachShader(theProgram, theShader);
}

void Shader::UseShader()
{
	// Tells our GPU to use our shader
	glUseProgram(shaderID);
}

// Called to clear shader
void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
}

// Clears shader. Main reason a destructor is here when clear shader does the same is here is simply because it is good practice
Shader::~Shader()
{
	ClearShader();
}