#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;

void main()
{
    // Calculate fragment position based on vertex (local) position
    // Apply matModel to rotate the skybox texture
    fragPosition = (matModel * vec4(vertexPosition, 1.0)).xyz;

    // Remove translation from the view matrix so the skybox is always centered
    mat4 rotView = matView;
    rotView[3][0] = 0.0;
    rotView[3][1] = 0.0;
    rotView[3][2] = 0.0;

    // Calculate final vertex position without matModel so the box geometry stays static
    vec4 clipPos = matProjection * rotView * vec4(vertexPosition, 1.0);

    // Force the depth to be the maximum possible value (1.0)
    gl_Position = clipPos.xyww;
}
