// Declare we are using OpenGL version 3.30
#version 330

// Gets texture coordinates from vertex shader
in vec2 TexCoord;
// Also gets normal and fragpos 
in vec3 Normal;
in vec3 FragPos;

// Outputs a colour
out vec4 colour;

// A struct is a lightweight class - we use this to store variables for our light
struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

// Struct for our material 
struct Material
{
	float specularIntensity;
	float shininess;
};

// Used to get a texel of our texture from the texture coordinates
uniform sampler2D theTexture;

// Create uniform variables from our structs
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

// Program execution begins and ends with main()
void main()
{
	// Caluclate ambient colour
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	// Calculate diffuse factor
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	// Calculate diffuse colour
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	// Calculate specular colour
	vec4 specularColour = vec4(0, 0, 0, 0);

	// Essentially if not facing away from the light source, we calculate the specular factor and colour
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	// Calculate the colour
	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);
}
