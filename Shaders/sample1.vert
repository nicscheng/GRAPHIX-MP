#version 330 core

layout(location = 0) in vec3 aPos; // for position access

// transformation matrix
uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0); // makes vec3 into a vec4
}

 