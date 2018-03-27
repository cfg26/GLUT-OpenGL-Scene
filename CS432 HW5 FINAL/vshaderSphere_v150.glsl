#version 150

in vec4 vPosition;	//vertex attribute for vertex position
in vec3 vNormal;
in vec2 vTexture;

out vec4 color;  //to send to the fragment shader, interpolated along the way
out vec3 textureCoord;

uniform mat4 model_matrix;  //the model matrix to transpose vertices from object coord to world coord
uniform mat4 proj_matrix;
uniform float toggle;
uniform float toggle2;
uniform mat4 view_matrix;
uniform float cspot;

//lighting stuff...
uniform vec4 lightPos;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular;
uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;



void main() 
{ 
	vec3 posInCam = (view_matrix*model_matrix*vPosition).xyz;
	vec3 lightInCam = (view_matrix*lightPos).xyz;
	
	vec3 L = normalize(lightInCam-posInCam); 
	vec3 l = normalize(posInCam-lightInCam); 
	vec3 ls = vec3(0,0,-1);

	float cspot = max(dot(l,ls),0.0);	
	float theta = acos(dot(l,ls));
	float thetamax = 0.1f;


	if(theta > thetamax){
		cspot = 0;
	}

	if(toggle > 0){
		cspot = 0;	
	}

	vec3 V = normalize(-posInCam);
	vec3 H = normalize(L+V);
	vec3 N = normalize((view_matrix*model_matrix*vec4(vNormal,0)).xyz);
	
	vec4 ambient = lightAmbient*matAmbient;
	
	float Kd = max(dot(L,N),0.0);
	vec4 diffuse = Kd*lightDiffuse*matDiffuse;
	
	vec4 specular;
	if(dot(L,N)<0.0)
		specular = vec4(1,0,0,1);
	else{
		float Ks = pow(max(dot(N,H),0.0),matAlpha);
		specular = Ks*lightSpecular*matSpecular;
	}
	
	//if(toggle2 > 1){
    //cspot = max(dot(l,ls),0.0);
	//}
	
	cspot = max(dot(l,ls),0.0);		
	color = cspot*(ambient + diffuse + specular);
	color.a = 1.0;
	
	
  	gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;  //assign the vertex position to the vPosition attribute multiplied by the model matrix
	textureCoord = normalize(vPosition.xyz); //pass through texture location to fragment shader

}