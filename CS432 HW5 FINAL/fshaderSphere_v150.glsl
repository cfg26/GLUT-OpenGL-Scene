#version 150
in vec4 color;	//get the interpolated color from the vertex shader
in vec3 textureCoord;  //get the interpolated texture location from the vertex shader

uniform samplerCube cubeMap;  //the texture unit to sample from

out vec4 fColor;


void main() 
{
    vec4 texColor = texture(cubeMap,textureCoord);  //get the texture color at location textureCoord
    fColor = color*texColor;  //apply the uniform color to the fragment
} 

