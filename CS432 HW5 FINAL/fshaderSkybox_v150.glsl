#version 150
in vec3 textureCoord;  //get the interpolated texture location from the vertex shader
uniform samplerCube cubeMap;  //the texture unit to sample from

out vec4 fColor;


void main() 
{
    fColor = texture(cubeMap, textureCoord);  //apply the uniform color to the fragment
    fColor.a = 1.0; //get the texture color at location textureCoord
   
} 

