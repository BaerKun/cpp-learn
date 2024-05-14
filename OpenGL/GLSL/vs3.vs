#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;

void main(){
    float x = aPos[1];
    float y = aPos[0];
    float z =
y                                                               ;
    gl_Position = transform * vec4(y, z, x, 1.0);
}