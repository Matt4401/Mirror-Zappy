#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 matProjection;
uniform mat4 matView;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;

void main()
{
    // Calculate fragment position based on vertex (local) position
    fragPosition = vertexPosition;

    // Remove translation from the view matrix so the skybox is always centered
    mat4 rotView = matView;
    rotView[3][0] = 0.0;
    rotView[3][1] = 0.0;
    rotView[3][2] = 0.0;

    // Calculate final vertex position
    vec4 clipPos = matProjection * rotView * vec4(vertexPosition, 1.0);

    // Force the depth to be the maximum possible value (1.0)
    gl_Position = clipPos.xyww;
}
