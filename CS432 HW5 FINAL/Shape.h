#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "Angel.h"
#include "Drawable.h"

class Shape: public Drawable{  //derived from drawable class


public:	

	//constructors
	Shape(int tempshape, char *texture, float x, float y, float z);
	
	//destructor
	~Shape();


	//func declarations
	void build();
	void draw(Camera, vector<Light>);
	void setMaterial(vec4, vec4, vec4, float);
	void makeQuad(int a, int b, int c, int d); \
	void buildCube();
	void Shape::makePlaneQuad(int a, int b, int c, int d);
	void buildGroundPlane();
	void makeSkyBoxQuad(int a, int b, int c, int d);
	void buildSkyBox();
	void changeTextureToCrate();
	void changeTextureToBricks();
	void idle();
	void buildDog();
	void dynamicQuad(int a, int b, int c, int d);
	void buildDynamicCube();



	//sphere
	const int NumTimesToSubdivide = 5;
	const int NumTriangles = 4096;
	const int NumVertices = 3 * NumTriangles;
	typedef Angel::vec4 point4;
	typedef Angel::vec4 color4;
	point4 points[3*4096];

	vec3 normals[3 * 4096];
	color4 colors[3 * 4096];
	GLuint ModelView, Projection;
	int Index = 0;
	int shapeType;
	char *textureName;
	GLuint buffers[2];
	GLuint VAOs[2];
	vec2 vertexTextureLocations[6 * 2 * 3];

	//cube
	vec4 verticesCube[8];
	vec4 potentialColorsCube[20];
	const int NumCubeVertices = 6 * 2 * 3;
	int currentCubeIndex = 0;
	vec4 cubeVertexLocations[6 * 2 * 3];
	vec4 cubeVertexColors[6 * 2 * 3];
	int imgsize = 512;	
	const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)
	point4 pointsCube[36];
	vec3 normalsCube[36];


	//particle system
	struct particle {
		vec4 color;
		vec4 position;
		vec4 velocity;
		float mass;
	};
	int NUM_PARTICLES = 1000;
	particle particles[1000];
	vec4 particlePoints[1000];
	vec4 particleColors[1000];
	vec4 particleVelocities[1000];
	float last_time, present_time;
	float coef;

	float x2;
	float y2;
	float z2;

	//function 
	void initializeParticles();
	void updateParticles();
	void drawParticles();
	float applyForces(int i, int j);
	void testCollision(int n);

	
	//vertices set
	point4 vertices2[8];
	point4 vertices3[8];
	point4 vertices4[8];
	point4 vertices5[4];
	point4 vertices6[8];

	//point4 vertices5[20];
	point4 pointsDog[36];
	vec3 normalsDog[36];
		

private:
	vec4 vertexLocations[3];
	vec3 vertexNormals[3];
	GLuint textures[2];
		
	void buildTriangle();
	void tetrahedron(int count);
	void divide_triangle(const point4& a, const point4& b, const point4& c, int count);
	point4 unit(const point4& p);
	void triangle(const point4& a, const point4& b, const point4& c, vec4 color1);
	GLubyte *data;

	//materials
	vec4 matDiffuse, matSpecular, matAmbient;
	float shininess;
	

};
#endif
