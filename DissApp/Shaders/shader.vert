// Declare we are using OpenGL version 3.30
#version 330

// Vertex shader is inputted with a vertex position
layout (location = 0) in vec3 pos;
// Also inputted with a tex coordinate, or uv value
layout (location = 1) in vec2 tex;
// Inputted with a normal direction
layout (location = 2) in vec3 norm;

// Uniform variables
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

// Output texture coordinate to our fragment shader
out vec2 TexCoord;
// Also output normal & a fragment position
out vec3 Normal;
out vec3 FragPos;

// Shader program execution begins and ends with main()
void main()
{
	// Include projection now, same as we did with model
	gl_Position = projection * view * model * vec4(pos, 1.0);
	// Set TexCoord to tex
	TexCoord = tex;
	//Calculate normal
	Normal = mat3(transpose(inverse(model))) * norm;
	//Calculate frag pos
	FragPos = (model * vec4(pos, 1.0)).xyz; 
}