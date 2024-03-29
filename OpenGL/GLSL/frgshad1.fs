#version 330 core
in vec4 color;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    vec4 color1 = texture(texture1, texCoord);
    vec4 color2 = texture(texture2, texCoord);
    FragColor = mix(color1, color2, 0.5);
}