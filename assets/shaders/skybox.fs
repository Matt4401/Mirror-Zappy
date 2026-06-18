#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform samplerCube environmentMap;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Fetch color from the cubemap texture
    // We use the 3D position vector directly as the texture coordinate
    finalColor = texture(environmentMap, fragPosition);
}
