#version 150

in vec4 vPosition;	

uniform mat4 proj_matrix;
uniform mat4 view_matrix;

out vec3 textureCoord; 

void main() 
{ 	
	textureCoord = normalize(vPosition.xyz);
	
  	gl_Position = proj_matrix*view_matrix*vPosition;  //assign the vertex position to the vPosition attribute multiplied by the model matrix  	
}