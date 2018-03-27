#include "Shape.h"
Shape::Shape(int tempshape, char *texture,float x, float y, float z) {
	//load the shader and get the location of it's variables
	program = InitShader("vshaderLighting_v150.glsl", "fshaderLighting_v150.glsl");
	shapeType = tempshape;


	//vertices for cubes
	vertices2[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices2[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices2[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices2[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices2[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices2[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices2[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices2[7] = point4(0.5, -0.5, -0.5, 1.0);

	vertices3[0] = point4(-5.5, -5.5, 5.5, 1.0);
	vertices3[1] = point4(-5.5, 5.5, 5.5, 1.0);
	vertices3[2] = point4(5.5, 5.5, 5.5, 1.0);
	vertices3[3] = point4(5.5, -5.5, 5.5, 1.0);
	vertices3[4] = point4(-5.5, -5.5, -5.5, 1.0); //top 4 5 6 7
	vertices3[5] = point4(-5.5, 5.5, -5.5, 1.0);
	vertices3[6] = point4(5.5, 5.5, -5.5, 1.0);
	vertices3[7] = point4(5.5, -5.5, -5.5, 1.0);

	vertices5[0] = point4(-5.5, -5.5, -5.5, 1.0);
	vertices5[1] = point4(-5.5, 5.5, -5.5, 1.0);
	vertices5[2] = point4(5.5, 5.5, -5.5, 1.0);
	vertices5[3] = point4(5.5, -5.5, -5.5, 1.0);

	vertices4[0] = point4(-45.5, -45.5, 45.5, 1.0);
	vertices4[1] = point4(-45.5, 45.5, 45.5, 1.0);
	vertices4[2] = point4(45.5, 45.5, 45.5, 1.0);
	vertices4[3] = point4(45.5, -45.5, 45.5, 1.0);
	vertices4[4] = point4(-45.5, -45.5, -45.5, 1.0);
	vertices4[5] = point4(-45.5, 45.5, -45.5, 1.0);
	vertices4[6] = point4(45.5, 45.5, -45.5, 1.0);
	vertices4[7] = point4(45.5, -45.5, -45.5, 1.0);


	//particle system variables
	NUM_PARTICLES = 1000;
	particle particles[1000];
	vec4 particlePoints[1000];
	vec4 particleColors[1000];
	vec4 particleVelocities[1000];
	float last_time, present_time;
	float coef;


	//values passed in by constructor for cube location
	x2 = x;
	y2 = y;
	z2 = z;



	currentCubeIndex = 0;

	textureName = texture;
	build();
}

void Shape::build() {


	glGenBuffers(3, buffers);
	glGenVertexArrays(3, VAOs);



	////////////////////////////////////////////////////////////////SPHERE//////////////////////////////////////////
	if (shapeType == 1) {
		tetrahedron(NumTimesToSubdivide);

		//move the data onto the buffer
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		sizeof(normals), normals);

	//link the vertex attributes with the buffer
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

}
	////////////////////////////////////////////////////////////////CUBE//////////////////////////////////////////
	if (shapeType == 2) {
		
		buildCube(); 

						 
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube) + sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(normalsCube), normalsCube);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube), sizeof(vertexTextureLocations), vertexTextureLocations);

		//link the vertex attributes with the buffer
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube)));

		GLuint vTexture = glGetAttribLocation(program, "vTexture");
		glEnableVertexAttribArray(vTexture);
		glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube) + sizeof(normalsCube)));

		glGenTextures(1, &textures[0]);
		int imgsize = 512;
		data = ppmRead(textureName, &imgsize, &imgsize);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
		delete[] data;  //dont' need this data now that its on the GPU

						//set the texturing parameters
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
	}		
	////////////////////////////////////////////////////////////////GROUND PLANE//////////////////////////////////////////
	if (shapeType == 3) {
		buildGroundPlane();

		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube) + sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(normalsCube), normalsCube);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube), sizeof(vertexTextureLocations), vertexTextureLocations);

		//link the vertex attributes with the buffer
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube)));

		GLuint vTexture = glGetAttribLocation(program, "vTexture");
		glEnableVertexAttribArray(vTexture);
		glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexTextureLocations) + sizeof(normalsCube)));

		glGenTextures(1, &textures[0]);
		int imgsize = 512;
		data = ppmRead("grass256by256.ppm", &imgsize, &imgsize);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
		delete[] data;  //dont' need this data now that its on the GPU

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	////////////////////////////////////////////////////////////////SKYBOX//////////////////////////////////////////
	if (shapeType == 4) {

		buildCube();

		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube) + sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(normalsCube), normalsCube);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube), sizeof(vertexTextureLocations), vertexTextureLocations);

		//link the vertex attributes with the buffer
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glGenTextures(1, &textures[0]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);

		int imgsize = 512;

		GLubyte *skyTop = ppmRead("sky-top.ppm", &imgsize, &imgsize);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyTop);  //move the data onto the GPU

		GLubyte *skyBottom = ppmRead("sky-bottom.ppm", &imgsize, &imgsize);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBottom);  //move the data onto the GPU

		GLubyte *skyRight = ppmRead("sky-right.ppm", &imgsize, &imgsize);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyRight);  //move the data onto the GPU

		GLubyte *skyLeft = ppmRead("sky-left.ppm", &imgsize, &imgsize);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyLeft);  //move the data onto the GPU

		GLubyte *skyFront = ppmRead("sky-front.ppm", &imgsize, &imgsize);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyFront);  //move the data onto the GPU

		GLubyte *skyBack = ppmRead("sky-back.ppm", &imgsize, &imgsize);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBack);  //move the data onto the GPU

		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);

		//set the texturing parameters
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	////////////////////////////////////////////////////////////////PARTICLES//////////////////////////////////////////
	if (shapeType == 5) {
		initializeParticles();
	}

	////////////////////////////////////////////////////////////////NOT USED//////////////////////////////////////////
	if (shapeType == 6) {
			buildCube();


			glBindVertexArray(VAOs[1]);
			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube) + sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(normalsCube), normalsCube);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube), sizeof(vertexTextureLocations), vertexTextureLocations);

			//link the vertex attributes with the buffer
			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			GLuint vNormal = glGetAttribLocation(program, "vNormal");
			glEnableVertexAttribArray(vNormal);
			glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube)));

			GLuint vTexture = glGetAttribLocation(program, "vTexture");
			glEnableVertexAttribArray(vTexture);
			glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube) + sizeof(normalsCube)));

			glGenTextures(1, &textures[0]);
			int imgsize = 512;
			data = ppmRead(textureName, &imgsize, &imgsize);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
			delete[] data;  //dont' need this data now that its on the GPU

							//set the texturing parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		}

	////////////////////////////////////////////////////////////////DYNAMIC CUBE//////////////////////////////////////////
		if (shapeType == 7) {
			cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
			buildDynamicCube();

			


			glBindVertexArray(VAOs[1]);
			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube) + sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(normalsCube), normalsCube);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(normalsCube), sizeof(vertexTextureLocations), vertexTextureLocations);

			//link the vertex attributes with the buffer
			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			GLuint vNormal = glGetAttribLocation(program, "vNormal");
			glEnableVertexAttribArray(vNormal);
			glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube)));

			GLuint vTexture = glGetAttribLocation(program, "vTexture");
			glEnableVertexAttribArray(vTexture);
			glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube) + sizeof(normalsCube)));

			glGenTextures(1, &textures[0]);
			int imgsize = 512;
			data = ppmRead(textureName, &imgsize, &imgsize);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
			delete[] data;  //dont' need this data now that its on the GPU

							//set the texturing parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		}

}

void Shape::draw(Camera cam, vector<Light>lights){

	////////////////////////////////////////////////////////////////SPHERE//////////////////////////////////////////
	if (shapeType == 1) {
		//program = InitShader("vshaderSphere_v150.glsl", "fshaderSphere_v150.glsl");
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glUseProgram(program);

		//set the transformation matrices
		GLuint model_loc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);
		GLuint view_loc = glGetUniformLocation(program, "view_matrix");
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, cam.getViewMatrix());
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());
		GLint loc = glGetUniformLocation(program, "toggle");
		glUniform1f(loc, cam.getToggle());

		GLint loc2 = glGetUniformLocation(program, "toggle2");
		glUniform1f(loc2, cam.getToggle2());
		
		//set the materials
		GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
		glUniform4fv(ambient_loc, 1, matAmbient);
		GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
		glUniform4fv(diffuse_loc, 1, matDiffuse);
		GLuint specular_loc = glGetUniformLocation(program, "matSpecular");
		glUniform4fv(specular_loc, 1, matSpecular);
		GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
		glUniform1f(alpha_loc, shininess);


		//set up the lighting
		//light stuff
		GLuint light_loc = glGetUniformLocation(program, "lightPos");
		glUniform4fv(light_loc, 1, lights[0].getPosition());
		ambient_loc = glGetUniformLocation(program, "lightAmbient");
		glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
		diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
		glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
		specular_loc = glGetUniformLocation(program, "lightSpecular");
		glUniform4fv(specular_loc, 1, lights[0].getSpecular());

		glLineWidth(5.0);
		glDrawArrays(GL_TRIANGLES, 0,NumVertices);

	}

	////////////////////////////////////////////////////////////////CUBE//////////////////////////////////////////
	if (shapeType == 2) {
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);

		glUseProgram(program);

		//set the transformation matrices
		GLuint model_loc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);
		GLuint view_loc = glGetUniformLocation(program, "view_matrix");
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, cam.getViewMatrix());
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());
		GLint loc = glGetUniformLocation(program, "toggle");
		glUniform1f(loc, cam.getToggle());
		GLint loc2 = glGetUniformLocation(program, "toggle2");
		glUniform1f(loc2, cam.getToggle2());


		//set the materials
		GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
		glUniform4fv(ambient_loc, 1, matAmbient);
		GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
		glUniform4fv(diffuse_loc, 1, matDiffuse);
		GLuint specular_loc = glGetUniformLocation(program, "matSpecular");
		glUniform4fv(specular_loc, 1, matSpecular);
		GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
		glUniform1f(alpha_loc, shininess);


		//set up the lighting
		//light stuff
		GLuint light_loc = glGetUniformLocation(program, "lightPos");
		glUniform4fv(light_loc, 1, lights[0].getPosition());
		ambient_loc = glGetUniformLocation(program, "lightAmbient");
		glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
		diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
		glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
		specular_loc = glGetUniformLocation(program, "lightSpecular");
		glUniform4fv(specular_loc, 1, lights[0].getSpecular());

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(glGetUniformLocation(program, "textureID"), 0);

		//draw!
		glDrawArrays(GL_TRIANGLES, 0, NumVerticesCube);
	}

	////////////////////////////////////////////////////////////////GROUND PLANE//////////////////////////////////////////
	if (shapeType == 3) {
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

		glUseProgram(program);

		//set the transformation matrices
		GLuint model_loc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);
		GLuint view_loc = glGetUniformLocation(program, "view_matrix");
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, cam.getViewMatrix());
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());
		GLint loc = glGetUniformLocation(program, "toggle");
		glUniform1f(loc, cam.getToggle());
		GLint loc2 = glGetUniformLocation(program, "toggle2");
		glUniform1f(loc2, cam.getToggle2());

		//set the materials
		GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
		glUniform4fv(ambient_loc, 1, matAmbient);
		GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
		glUniform4fv(diffuse_loc, 1, matDiffuse);
		GLuint specular_loc = glGetUniformLocation(program, "matSpecular");
		glUniform4fv(specular_loc, 1, matSpecular);
		GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
		glUniform1f(alpha_loc, shininess);


		//set up the lighting
		//light stuff
		GLuint light_loc = glGetUniformLocation(program, "lightPos");
		glUniform4fv(light_loc, 1, lights[0].getPosition());
		ambient_loc = glGetUniformLocation(program, "lightAmbient");
		glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
		diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
		glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
		specular_loc = glGetUniformLocation(program, "lightSpecular");
		glUniform4fv(specular_loc, 1, lights[0].getSpecular());

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(glGetUniformLocation(program, "textureID"), 0);

		//draw!
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}

	////////////////////////////////////////////////////////////////SKYBOX//////////////////////////////////////////
	if (shapeType == 4) {
		program = InitShader("vshaderSkybox_v150.glsl", "fshaderSkybox_v150.glsl");

		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

		glUseProgram(program);

		GLuint view_loc = glGetUniformLocation(program, "view_matrix");
		mat4 view_Matrix = mat4(1.0);
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, view_Matrix);
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());

		glEnable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
		GLuint mapPos = glGetUniformLocation(program, "cubeMap");
		glUniform1i(mapPos, 0);



		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);



	}

	////////////////////////////////////////////////////////////////PARTICLES//////////////////////////////////////////
	if (shapeType == 5) {
	
		glBindVertexArray(VAOs[0]);
		glUseProgram(program);
		
		for (int i = 0; i < 1000; i++) {
			particlePoints[i] = particles[i].position;
			particlePoints[i] = particles[i].color;
		}

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePoints), particlePoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints), sizeof(particleColors), particleColors);
		
		GLuint model_loc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);
		GLuint camera_loc = glGetUniformLocation(program, "camera_matrix");
		glUniformMatrix4fv(camera_loc, 1, GL_TRUE, cam.getViewMatrix());
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());

		glPointSize(4.0);
		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

		glBindVertexArray(0);
	}
	////////////////////////////////////////////////////////////////NOT USED//////////////////////////////////////////
	if (shapeType == 6) {
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);

		glUseProgram(program);

		//set the transformation matrices
		GLuint model_loc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);
		GLuint view_loc = glGetUniformLocation(program, "view_matrix");
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, cam.getViewMatrix());
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());
		GLint loc = glGetUniformLocation(program, "toggle");
		glUniform1f(loc, cam.getToggle());
		GLint loc2 = glGetUniformLocation(program, "toggle2");
		glUniform1f(loc2, cam.getToggle2());


		//set the materials
		GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
		glUniform4fv(ambient_loc, 1, matAmbient);
		GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
		glUniform4fv(diffuse_loc, 1, matDiffuse);
		GLuint specular_loc = glGetUniformLocation(program, "matSpecular");
		glUniform4fv(specular_loc, 1, matSpecular);
		GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
		glUniform1f(alpha_loc, shininess);


		//set up the lighting
		//light stuff
		GLuint light_loc = glGetUniformLocation(program, "lightPos");
		glUniform4fv(light_loc, 1, lights[0].getPosition());
		ambient_loc = glGetUniformLocation(program, "lightAmbient");
		glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
		diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
		glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
		specular_loc = glGetUniformLocation(program, "lightSpecular");
		glUniform4fv(specular_loc, 1, lights[0].getSpecular());

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(glGetUniformLocation(program, "textureID"), 0);

		//draw!
		glDrawArrays(GL_TRIANGLES, 0, NumVerticesCube);


	}
	////////////////////////////////////////////////////////////////DYNAMIC CUBE//////////////////////////////////////////
	if (shapeType == 7) {
		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);

		glUseProgram(program);

		//set the transformation matrices
		GLuint model_loc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);
		GLuint view_loc = glGetUniformLocation(program, "view_matrix");
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, cam.getViewMatrix());
		GLuint proj_loc = glGetUniformLocation(program, "proj_matrix");
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, cam.getProjectionMatrix());
		GLint loc = glGetUniformLocation(program, "toggle");
		glUniform1f(loc, cam.getToggle());
		GLint loc2 = glGetUniformLocation(program, "toggle2");
		glUniform1f(loc2, cam.getToggle2());

		//set the materials
		GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
		glUniform4fv(ambient_loc, 1, matAmbient);
		GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
		glUniform4fv(diffuse_loc, 1, matDiffuse);
		GLuint specular_loc = glGetUniformLocation(program, "matSpecular");
		glUniform4fv(specular_loc, 1, matSpecular);
		GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
		glUniform1f(alpha_loc, shininess);


		//set up the lighting
		//light stuff
		GLuint light_loc = glGetUniformLocation(program, "lightPos");
		glUniform4fv(light_loc, 1, lights[0].getPosition());
		ambient_loc = glGetUniformLocation(program, "lightAmbient");
		glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
		diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
		glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
		specular_loc = glGetUniformLocation(program, "lightSpecular");
		glUniform4fv(specular_loc, 1, lights[0].getSpecular());

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(glGetUniformLocation(program, "textureID"), 0);

		//draw!
		glDrawArrays(GL_TRIANGLES, 0, NumVerticesCube);
	}

}


//Used for sphere subdivision
Shape::point4 Shape::unit(const point4& p)
{
	float len = p.x*p.x + p.y*p.y + p.z*p.z;
	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}
	return t;
}
//Used for sphere subdivision
void Shape::divide_triangle(const point4& a, const point4& b, const point4& c, int count)
{

	float r = ((double)rand() / (RAND_MAX));
	float g = ((double)rand() / (RAND_MAX));
	float h = ((double)rand() / (RAND_MAX));
	float z = ((double)rand() / (RAND_MAX));

	vec4 temp = vec4(r, g, h, z);

	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c, temp);
	}
}

//Used for sphere subdivision
void Shape::tetrahedron(int count)
{
	point4 v[4] = {
		vec4(0.0, 0.0, 1.0, 1.0),
		vec4(0.0, 0.942809, -0.333333, 1.0),
		vec4(-0.816497, -0.471405, -0.333333, 1.0),
		vec4(0.816497, -0.471405, -0.333333, 1.0)
	};


	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);
}

//Used for making cube
void Shape::makeQuad(int a, int b, int c, int d) {
	vec4 u = vertices2[b] - vertices2[a];
	vec4 v = vertices2[c] - vertices2[b];
	vec3 normal = normalize(cross(u, v));

	normalsCube[currentCubeIndex] = normal;	vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices2[a]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 0); pointsCube[currentCubeIndex] = vertices2[b]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1,0); pointsCube[currentCubeIndex] = vertices2[c]; currentCubeIndex++;

	u = vertices2[c] - vertices2[a];
	v = vertices2[d] - vertices2[a];
	normal = normalize(cross(u, v));

	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1, 0); pointsCube[currentCubeIndex] = vertices2[c]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1, 1); pointsCube[currentCubeIndex] = vertices2[d]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices2[a]; currentCubeIndex++;

}
//Used for making triangle for sphere subdivision
void Shape::triangle(const point4& a, const point4& b, const point4& c, vec4 color1)
{
	vec3 normal = normalize(cross(b - a, c - b));

	colors[Index] = color1;
	normals[Index] = normal;
	points[Index] = a;
	Index++;

	colors[Index] = color1;
	normals[Index] = normal;
	points[Index] = b;
	Index++;

	colors[Index] = color1;
	normals[Index] = normal;
	points[Index] = c;
	Index++;
}

//Used for making cube
void Shape::buildCube() {
	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left
}

//Not used
void Shape::buildDog() {
	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left
}

//Used for making dynamic cube
void Shape::dynamicQuad(int a, int b, int c, int d)
{
	vertices2[0] = point4(x2-0.5, y2-0.5, z2+0.5, 1.0);
	vertices2[1] = point4(x2-0.5, y2+0.5, z2 + 0.5, 1.0);
	vertices2[2] = point4(x2+0.5, y2+0.5, z2 + 0.5, 1.0);
	vertices2[3] = point4(x2+0.5, y2-0.5, z2 + 0.5, 1.0);
	vertices2[4] = point4(x2-0.5, y2-0.5, z2-0.5, 1.0);
	vertices2[5] = point4(x2-0.5, y2+0.5, z2-0.5, 1.0);
	vertices2[6] = point4(x2+0.5, y2+0.5, z2-0.5, 1.0);
	vertices2[7] = point4(x2+0.5, y2-0.5, z2-0.5, 1.0);


	vec4 u = vertices2[b] - vertices2[a];
	vec4 v = vertices2[c] - vertices2[b];
	vec3 normal = normalize(cross(u, v));

	normalsCube[currentCubeIndex] = normal;	vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices2[a]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 0); pointsCube[currentCubeIndex] = vertices2[b]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1, 0); pointsCube[currentCubeIndex] = vertices2[c]; currentCubeIndex++;

	u = vertices2[c] - vertices2[a];
	v = vertices2[d] - vertices2[a];
	normal = normalize(cross(u, v));

	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1, 0); pointsCube[currentCubeIndex] = vertices2[c]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1, 1); pointsCube[currentCubeIndex] = vertices2[d]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices2[a]; currentCubeIndex++;
}

//Used for making dynamic cube
void Shape::buildDynamicCube()
{


	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
	cout << x2;
	cout << y2;
	cout << z2;
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

	dynamicQuad(1, 0, 3, 2);  //front
	dynamicQuad(2, 3, 7, 6);  //right
	dynamicQuad(3, 0, 4, 7);  //bottom
	dynamicQuad(6, 5, 1, 2);  //top
	dynamicQuad(4, 5, 6, 7);  //back
	dynamicQuad(5, 4, 0, 1);  //left
}

//Used for making ground plane
void Shape::makePlaneQuad(int a, int b, int c, int d) {
	vec4 u = vertices3[b] - vertices3[a];
	vec4 v = vertices3[c] - vertices3[b];
	vec3 normal = normalize(cross(u, v));

	normalsCube[currentCubeIndex] = normal;		vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices3[a]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal;  vertexTextureLocations[currentCubeIndex] = vec2(0, 0); pointsCube[currentCubeIndex] = vertices3[b]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal;  vertexTextureLocations[currentCubeIndex] = vec2(1, 0); pointsCube[currentCubeIndex] = vertices3[c]; currentCubeIndex++;

	u = vertices2[c] - vertices2[a];
	v = vertices2[d] - vertices2[a];
	normal = normalize(cross(u, v));
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(1, 0); pointsCube[currentCubeIndex] = vertices3[a]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal;  vertexTextureLocations[currentCubeIndex] = vec2(1, 1); pointsCube[currentCubeIndex] = vertices3[c]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices3[d]; currentCubeIndex++;

}

//Used for making ground plane
void Shape::buildGroundPlane() {
	makePlaneQuad(1, 0, 3, 2);  //bottom
	makePlaneQuad(2, 3, 7, 6);  //left
	makePlaneQuad(3, 0, 4, 7);  //bottom
	makePlaneQuad(6, 5, 1, 2);  //front
	makePlaneQuad(4, 5, 6, 7);  //top
	makePlaneQuad(5, 4, 0, 1);  //right

}

///////////////////////////////BEGIN SKYBOX////////////////////////////////////////////////////////////////////////
void Shape::makeSkyBoxQuad(int a, int b, int c, int d) {
	vec4 u = vertices4[b] - vertices4[a];
	vec4 v = vertices4[c] - vertices4[b];
	vec3 normal = normalize(cross(u, v));

	normalsCube[currentCubeIndex] = normal;		vertexTextureLocations[currentCubeIndex] = vec2(0, 0); pointsCube[currentCubeIndex] = vertices4[a]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal;  vertexTextureLocations[currentCubeIndex] = vec2(1, 0); pointsCube[currentCubeIndex] = vertices4[b]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal;  vertexTextureLocations[currentCubeIndex] = vec2(1, 1); pointsCube[currentCubeIndex] = vertices4[c]; currentCubeIndex++;

	u = vertices4[c] - vertices3[a];
	v = vertices4[d] - vertices3[a];
	normal = normalize(cross(u, v));
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 0); pointsCube[currentCubeIndex] = vertices4[a]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal;  vertexTextureLocations[currentCubeIndex] = vec2(1, 1); pointsCube[currentCubeIndex] = vertices4[c]; currentCubeIndex++;
	normalsCube[currentCubeIndex] = normal; vertexTextureLocations[currentCubeIndex] = vec2(0, 1); pointsCube[currentCubeIndex] = vertices4[d]; currentCubeIndex++;

}

void Shape::buildSkyBox()
{
	makeSkyBoxQuad(1, 0, 3, 2);  //front
	makeSkyBoxQuad(2, 3, 7, 6);  //right
	makeSkyBoxQuad(3, 0, 4, 7);  //bottom
	makeSkyBoxQuad(6, 5, 1, 2);  //top
	makeSkyBoxQuad(4, 5, 6, 7);  //back
	makeSkyBoxQuad(5, 4, 0, 1);  //left
}

///////////////////////////////END SKYBOX////////////////////////////////////////////////////////////////////////

void Shape::changeTextureToCrate()
{
	textureName = "crate_texture.ppm";
}

void Shape::changeTextureToBricks()
{
	textureName = "brks.ppm";
}

//on destroy, delete the buffer (cleanup)
Shape::~Shape() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

//set the materials
void Shape::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse = d;
	matAmbient = a;
	matSpecular = s;
	shininess = sh;
}



void Shape::buildTriangle() {
	//set up the vertices
	vertexLocations[0] = vec4(-1, 0, 0, 1);
	vertexLocations[1] = vec4(1, 0, 0, 1);
	vertexLocations[2] = vec4(0, 0, -1, 1);

	//compute two of the edges
	vec4 e1 = vertexLocations[1] - vertexLocations[0];
	vec4 e2 = vertexLocations[2] - vertexLocations[0];

	//use the cross product to get the direction of the triangle's normal
	vec3 n = cross(vec3(e1.x, e1.y, e1.z), vec3(e2.x, e2.y, e2.z));
	n = normalize(n);
	//use computed normal for each vertex of the triangle
	vertexNormals[0] = n;
	vertexNormals[1] = n;
	vertexNormals[2] = n;

}
//initializes particle system
void Shape::initializeParticles()
{
	for (int i = 0; i < 1000; i++) {
		particles[i].mass = 1;
		for (int j = 0; j < 3; j++) {
			particles[i].color = vec4( 0., 1., 0., 1. );
			particles[i].position[j] = 20.0*((float)rand() / RAND_MAX) - 1.0;
			particles[i].velocity[j] = 1.0*((float)rand() / RAND_MAX) - 1.0;
		}
		particles[i].color.w = 1.0;
		particles[i].position.w = 1.0;
		particles[i].velocity.w = 0.0;
	}

	for (int i = 0; i <1000; i++) {
		particlePoints[i] = particles[i].position;
		particleColors[i] = particles[i].color;
		particleVelocities[i] = particles[i].velocity;
	}

	program = InitShader("vshaderParticle_v150.glsl", "fshaderParticle_v150.glsl");
	glBindVertexArray(VAOs[0]);
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlePoints) + sizeof(particleColors) + sizeof(particleVelocities), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePoints), particlePoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints), sizeof(particleColors), particleColors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints) + sizeof(particleColors), sizeof(particleVelocities), particleVelocities);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint color_loc = glGetAttribLocation(program, "colorIn");
	glEnableVertexAttribArray(color_loc);
	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(particlePoints)));

	GLuint velocity_loc = glGetAttribLocation(program, "vVelocity");
	glEnableVertexAttribArray(velocity_loc);
	glVertexAttribPointer(velocity_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(particlePoints) + sizeof(particleColors)));





}
//updates particles
void Shape::updateParticles()
{
	float elapsed_time = 0.001*glutGet(GLUT_ELAPSED_TIME);
	program = InitShader("vshaderParticle_v150.glsl", "fshaderParticle_v150.glsl");
	glUseProgram(program);

	GLuint time_loc = glGetUniformLocation(program, "time");
	glUniform1f(time_loc, elapsed_time);
}

void Shape::idle()
{
	updateParticles();
	glutPostRedisplay();
}


void Shape::drawParticles()
{
}

//unused
float Shape::applyForces(int i, int j)
{
	if (j == 1) //only affect y direction
		return -1;
	else
		return 0;
}

//unused
void Shape::testCollision(int n)
{
	for (int i = 0; i<3; i++) {
		if (particles[n].position[i] >= 1.0) {
			particles[n].velocity[i] *= -coef;
			particles[n].position[i] = 1.0 - coef*(particles[n].position[i] - 1.0);
		}
		if (particles[n].position[i] <= -1.0) {
			particles[n].velocity[i] *= -coef;
			particles[n].position[i] = -1.0 - coef*(particles[n].position[i] + 1.0);
		}
	}
}





