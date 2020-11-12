#version 330

uniform mat4 projection, modelview, model;

in vec2 position;

void main()
{
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * model * vec4(position, 0.0, 1.0);
}
