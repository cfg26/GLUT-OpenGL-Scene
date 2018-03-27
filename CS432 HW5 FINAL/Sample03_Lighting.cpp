//SAMPLE CODE
//Lighting
//1 Triangle
//1 Distance Light Source

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Shape.h"
#include "Light.h"
#include "Camera.h"

//camera variables
Camera camera;
Camera skyCamera;
Camera camera2;


//for rotation
mat4 m;
mat4 k;


//used for conversion from window to camera coordinates
float newX;
float newY;
float newZ;

//used to hold shapes
vector<Shape*> objs;
vector<Shape*> tempShapes;


//used for animation
float tempX = 0.1;
float g = -4.0;
float tempg = -0.1;
float h = -7.0;
float angle = 0;
int rotateX = 90;
int rotateY = 0;

//Foward declarations of functions
void init();
void display();
void resize(int width, int height);
void keyboard(unsigned char, int, int);
void close();
void mykey(unsigned char, int, int);
void ArrowInput(int key, int x, int y);
void timerCallback(int value);
void idle();
void mymouse(GLint button, GLint
	state, GLint x, GLint y);

//objects
vector<Light> lights;

//All of the shapes used
Shape *mySphere;
Shape *myCube;
Shape *myGroundPlane;
Shape *mySkyBox;
Shape *myParticles;
Shape *myStore;
Shape *myStore1;
Shape *myStore2;
Shape *myStore3;
Shape *myStore4;
Shape *myStreet1;
Shape *myStreet2;
Shape *myStreet3;
Shape *mySidewalk1;
Shape *mySidewalk2;
Shape *mySidewalk3;
Shape *myParkingLot;
Shape *myParkingLot2;
Shape *myParkingLot3;
Shape *myParkingLot4;
Shape *myParkingLot5;
Shape *myParkingLot6;
Shape *myParkingLot7;
Shape *myParkingLot8;
Shape *myParkingLot9;
Shape *myParkingLot10;
Shape *streetPart1;
Shape *streetPart2;

//person 1
Shape *armLeft;
Shape *armRight;
Shape *body;
Shape *legLeft;
Shape *legRight;
Shape *head;

//person 2
Shape *armLeft2;
Shape *armRight2;
Shape *body2;
Shape *legLeft2;
Shape *legRight2;
Shape *head2;

//person 3
Shape *armLeft3;
Shape *armRight3;
Shape *body3;
Shape *legLeft3;
Shape *legRight3;
Shape *head3;

//Houses
Shape *myHouse1;
Shape *myHouse2;
Shape *myHouse3;
Shape *myHouse4;

//car
Shape *carTop;
Shape *carBottom;
Shape *wheel1;
Shape *wheel2;
Shape *wheel3;
Shape *wheel4;

//trees
Shape *leaves1;
Shape *tree1;

Shape *leaves2;
Shape *tree2;

Shape *leaves3;
Shape *tree3;

Shape *leaves4;
Shape *tree4;

Shape *leaves5;
Shape *tree5;

Shape *leaves6;
Shape *tree6;

//used to fix face of people
Shape *faceFix;
Shape *faceFix1;
Shape *faceFix2;

//windows
Shape *window1;
Shape *window2;

//our dynamic crate
Shape *dynamicCrate;

//for small calculations
int counter;
float tempt = 4.0;
int temp = 1;

//camera coordinates
GLfloat eyeX =0;
GLfloat eyeY = 20;
GLfloat eyeZ=-1;

GLfloat atX = 0;
GLfloat atY = 1;
GLfloat atZ = 0;

GLfloat upX = 0;
GLfloat upY = 0;
GLfloat upZ = -1;

//light coordinates
GLfloat light1X = 0;
GLfloat light1Y = 10;
GLfloat light1Z = -1;
GLfloat light1W = 0;

GLfloat light2X = 1;
GLfloat light2Y = 1;
GLfloat light2Z = 1;
GLfloat light2W = 1;

GLfloat light4X = 1;
GLfloat light4Y = 1;
GLfloat light4Z = 1;
GLfloat light4W = 1;

GLfloat eye2X = 10;
GLfloat eye2Y = 10;
GLfloat eye2Z = -10;

GLfloat at2X = 1;
GLfloat at2Y = 1;
GLfloat at2Z = -2;

GLfloat up2X = 0;
GLfloat up2Y = 0;
GLfloat up2Z = -1;

float u = 3.0;





//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    glutInit( &argc, argv );	//initialize glut
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_SINGLE|GLUT_DEPTH);
#else
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE|GLUT_DEPTH);
#endif

    glutInitWindowSize( 512, 512 );	//set window size to be 512x512 pixels

    glutCreateWindow( "Lighting" );  //open a window with the title "2D Graphics"

	//initialize glew if necessary (don't need to on Macs)
	#ifndef __APPLE__
	GLenum err = glewInit();
	#endif

    init();  //do some initialize for our program

	//set up the callback functions
    glutDisplayFunc( display );  //REQUIRED.  What to do when it's time to draw
    glutKeyboardFunc( mykey );  //What to do if a keyboard event is detected
	glutSpecialFunc(ArrowInput);
	glutWMCloseFunc(close);
	glutMouseFunc(mymouse);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);  //use for recomputing projection matrix on reshape
    glutMainLoop();  //start infinite loop, listening for events
    return 0;
}


//Initialization
void init()
{
	//here all objects are initialized, with both material and model matrices. They are all pushed back into the objs vector.
	


	//skybox
	mySkyBox = new Shape(4, "crate_texture.ppm", 0, 0, 0);
	objs.push_back(mySkyBox);
	
	
	myCube = new Shape(2, "crate_texture.ppm", 0, 0, 0);
	myCube->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myCube->setModelMatrix(RotateX(90)*RotateY(0)*Translate(5, 1.2, 6)*Scale(0.5, 0.5, 0.5));  //scale it
	objs.push_back(myCube);


	//person 1
	body = new Shape(2, "body.ppm", 0, 0, 0);
	body->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	body->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1, -0.1)*Scale(0.3, 0.3, 0.3));  //scale it
	objs.push_back(body);

	armLeft = new Shape(2, "skin.ppm", 0, 0, 0);
	armLeft->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	armLeft->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1, 0.1)*Scale(0.1, 0.5, 0.1));  //scale it
	objs.push_back(armLeft);

	armRight = new Shape(2, "skin.ppm", 0, 0, 0);
	armRight->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	armRight->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1, -0.3)*Scale(0.1, 0.5, 0.1));  //scale it
	objs.push_back(armRight);


	legLeft = new Shape(2, "jeans.ppm", 0, 0, 0);
	legLeft->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	legLeft->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1.3, 0)*Scale(0.1, 0.3, 0.1));  //scale it
	objs.push_back(legLeft);

	legRight = new Shape(2, "jeans.ppm", 0, 0, 0);
	legRight->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	legRight->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1.3, -0.2)*Scale(0.1, 0.3, 0.1));  //scale it
	objs.push_back(legRight);


	head = new Shape(2, "face3.ppm", 0, 0, 0);
	head->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	head->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 0.79, -0.1)*Scale(0.2, 0.2, 0.2));  //scale it
	objs.push_back(head);

	
	faceFix2 = new Shape(2, "skin.ppm", 0, 0, 0);
	faceFix2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	faceFix2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3.01, 0.79, -0.1)*Scale(0.2, 0.21, 0.21));  //scale it
	objs.push_back(faceFix2);






    //person 2
	body2 = new Shape(2, "body.ppm", 0, 0, 0);
	body2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	body2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1, -0.1+tempt)*Scale(0.3, 0.3, 0.3));  //scale it
	objs.push_back(body2);

	armLeft2 = new Shape(2, "skin.ppm", 0, 0, 0);
	armLeft2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	armLeft2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1, 0.1 + tempt)*Scale(0.1, 0.5, 0.1));  //scale it
	objs.push_back(armLeft2);

	armRight2 = new Shape(2, "skin.ppm", 0, 0, 0);
	armRight2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	armRight2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1, -0.3 + tempt)*Scale(0.1, 0.5, 0.1));  //scale it
	objs.push_back(armRight2);

	legLeft2 = new Shape(2, "jeans.ppm", 0, 0, 0);
	legLeft2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	legLeft2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1.3, 0 + tempt)*Scale(0.1, 0.3, 0.1));  //scale it
	objs.push_back(legLeft2);

	legRight2 = new Shape(2, "jeans.ppm", 0, 0, 0);
	legRight2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	legRight2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 1.3, -0.2 + tempt)*Scale(0.1, 0.3, 0.1));  //scale it
	objs.push_back(legRight2);

	head2 = new Shape(2, "face3.ppm", 0, 0, 0);
	head2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	head2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3, 0.79, -0.1 + tempt)*Scale(0.2, 0.2, 0.2));  //scale it
	objs.push_back(head2);


	faceFix1 = new Shape(2, "skin.ppm", 0, 0, 0);
	faceFix1->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	faceFix1->setModelMatrix(RotateX(90)*RotateY(0)*Translate(3.01, 0.79, -0.1 + tempt)*Scale(0.2, 0.21, 0.21));  //scale it
	objs.push_back(faceFix1);



	//person 3
	body3 = new Shape(2, "body.ppm", 0, 0, 0);
	body3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	body3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 1, -0.1 - tempt + 0.9)*Scale(0.3, 0.3, 0.3));  //scale it
	objs.push_back(body3);

	armLeft3 = new Shape(2, "skin.ppm", 0, 0, 0);
	armLeft3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	armLeft3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 1, 0.1 - tempt + 0.9)*Scale(0.1, 0.5, 0.1));  //scale it
	objs.push_back(armLeft3);

	armRight3 = new Shape(2, "skin.ppm", 0, 0, 0);
	armRight3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	armRight3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 1, -0.3 - tempt + 0.9)*Scale(0.1, 0.5, 0.1));  //scale it
	objs.push_back(armRight3);

	legLeft3 = new Shape(2, "jeans.ppm", 0, 0, 0);
	legLeft3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	legLeft3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 1.3, 0 - tempt + 0.9)*Scale(0.1, 0.3, 0.1));  //scale it
	objs.push_back(legLeft3);

	legRight3 = new Shape(2, "jeans.ppm", 0, 0, 0);
	legRight3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	legRight3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 1.3, -0.2 - tempt + 0.9)*Scale(0.1, 0.3, 0.1));  //scale it
	objs.push_back(legRight3);

	head3 = new Shape(2, "face3.ppm", 0, 0, 0);
	head3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	head3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 0.79, -0.1 - tempt + 0.9)*Scale(0.2, 0.2, 0.2));  //scale it
	objs.push_back(head3);

	faceFix = new Shape(2, "skin.ppm", 0, 0, 0);
	faceFix->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	faceFix->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4.01, 0.79, -0.1 - tempt + 0.9)*Scale(0.2, 0.21, 0.21));  //scale it
	objs.push_back(faceFix);





	//buildings/////////////////////////////
	myStore = new Shape(2, "store4.ppm", 0, 0, 0);
	myStore->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStore->setModelMatrix(RotateX(90)*RotateY(180)*Translate(-2, 1, -8.8)*Scale(3,20,3));  //scale it
	objs.push_back(myStore);

	myStore1 = new Shape(2, "store4.ppm", 0, 0, 0);
	myStore1->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStore1->setModelMatrix(RotateX(90)*RotateY(180)*Translate(0, 1, -8.8)*Scale(3, 20, 3));  //scale it
	objs.push_back(myStore1);

	myStore2 = new Shape(2, "store4.ppm", 0, 0, 0);
	myStore2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStore2->setModelMatrix(RotateX(90)*RotateY(180)*Translate(2, 1, -8.8)*Scale(3, 20, 3));  //scale it
	objs.push_back(myStore2);

	myStore3 = new Shape(2, "store4.ppm", 0, 0, 0);
	myStore3->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStore3->setModelMatrix(RotateX(90)*RotateY(180)*Translate(5.6, 1, -4.7)*Scale(3, 20, 3));  //scale it
	objs.push_back(myStore3);

	myStore4 = new Shape(2, "store4.ppm", 0, 0, 0);
	myStore4->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStore4->setModelMatrix(RotateX(90)*RotateY(180)*Translate(-5.6, 1, -4.7)*Scale(3, 20, 3));  //scale it
	objs.push_back(myStore4);

	///houses///////////////////////////////////////////////////////////////////////////////////////////////

	myHouse1 = new Shape(2, "window.ppm", 0, 0, 0);
	myHouse1->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myHouse1->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(-1.2, 1, 7.75)*Scale(1, 1, 1));  //scale it
	objs.push_back(myHouse1);

	myHouse4 = new Shape(2, "window.ppm", 0, 0, 0);
	myHouse4->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myHouse4->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(-2.7, 1, 7.75)*Scale(1, 1, 1));  //scale it
	objs.push_back(myHouse4);

	myHouse2 = new Shape(2, "brks.ppm", 0, 0, 0);
	myHouse2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myHouse2->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(-2, 1, 8.8)*Scale(3, 5, 3));  //scale it
	objs.push_back(myHouse2);


	//house 2

	myHouse3 = new Shape(2, "brks.ppm", 0, 0, 0);
	myHouse3->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myHouse3->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(6, 1, 8.8)*Scale(3, 5, 3));  //scale it
	objs.push_back(myHouse3);

	//windows
	window1 = new Shape(2, "window.ppm", 0, 0, 0);
	window1->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	window1->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(5.3, 1, 7.75)*Scale(1, 1, 1));  //scale it
	objs.push_back(window1);


	window2 = new Shape(2, "window.ppm", 0, 0, 0);
	window2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	window2->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(6.8, 1, 7.75)*Scale(1, 1, 1));  //scale it
	objs.push_back(window2);

	//street parts////////////////////////////
	myStreet1 = new Shape(2, "street3.ppm", 0, 0, 0);
	myStreet1->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStreet1->setModelMatrix(Translate(0, 1, 1.95)*Scale(21.93,1,1)*RotateX(90));  //scale it
	objs.push_back(myStreet1);

	myStreet2 = new Shape(2, "street3.ppm", 0, 0, 0);
	myStreet2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStreet2->setModelMatrix(Translate(0, 2, 1.95)*Scale(21.93, 1, 1)*RotateX(90));  //scale it
	objs.push_back(myStreet2);

	myStreet3 = new Shape(2, "street3.ppm", 0, 0, 0);
	myStreet3->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myStreet3->setModelMatrix(Translate(0, -0.99, 1.94)*Scale(2, 3, 1)*RotateX(180));  //scale it
	objs.push_back(myStreet3);

	//sidewalk parts
	mySidewalk1 = new Shape(2, "sidewalk.ppm", 0, 0, 0);
	mySidewalk1->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	mySidewalk1->setModelMatrix(Translate(0, 2.9, 1.95)*Scale(21.93, 0.8, 1)*RotateX(90));  //scale it
	objs.push_back(mySidewalk1);

	mySidewalk2 = new Shape(2, "sidewalk.ppm", 0, 0, 0);
	mySidewalk2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	mySidewalk2->setModelMatrix(Translate(0, 0.1, 1.95)*Scale(21.93, 0.8, 1)*RotateX(90));  //scale it
	objs.push_back(mySidewalk2);

	mySidewalk3 = new Shape(2, "sidewalk.ppm", 0, 0, 0);
	mySidewalk3->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	mySidewalk3->setModelMatrix(Translate(0, -6.9, 1.95)*Scale(7.6, 0.8, 1)*RotateX(90));  //scale it
	objs.push_back(mySidewalk3);


	//parking lot parts
	myParkingLot = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot->setModelMatrix(Translate(-1,-5.5, 1.95)*Scale(2, 2, 1)*RotateX(90));  //scale it
	objs.push_back(myParkingLot);

	myParkingLot2 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot2->setModelMatrix(Translate(-3, -5.5, 1.95)*Scale(2, 2, 1)*RotateX(90));  //scale it
	objs.push_back(myParkingLot2);

	myParkingLot3 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot3->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot3->setModelMatrix(Translate(1, -5.5, 1.95)*Scale(2, 2, 1)*RotateX(90));  //scale it
	objs.push_back(myParkingLot3);

	myParkingLot4 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot4->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot4->setModelMatrix(Translate(3, -5.5, 1.95)*Scale(2, 2, 1)*RotateX(90));  //scale it
	objs.push_back(myParkingLot4);

	myParkingLot5 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot5->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot5->setModelMatrix(Translate(-3, -3.5, 1.95)*Scale(2, 2, 1)*RotateX(-90));  //scale it
	objs.push_back(myParkingLot5);

	myParkingLot6 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot6->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot6->setModelMatrix(Translate(-1, -3.5, 1.95)*Scale(2, 2, 1)*RotateX(-90));  //scale it
	objs.push_back(myParkingLot6);

	myParkingLot7 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot7->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot7->setModelMatrix(Translate(1, -3.5, 1.95)*Scale(2, 2, 1)*RotateX(-90));  //scale it
	objs.push_back(myParkingLot7);

	myParkingLot8 = new Shape(2, "lot.ppm", 0, 0, 0);
	myParkingLot8->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParkingLot8->setModelMatrix(Translate(3, -3.5, 1.95)*Scale(2, 2, 1)*RotateX(-90));  //scale it
	objs.push_back(myParkingLot8);
	

	//ground plane
	myGroundPlane = new Shape(3, "crate_texture.ppm", 0, 0, 0);
	myGroundPlane->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 100);
	myGroundPlane->setModelMatrix(Translate(0, 0, 7)*Scale(2,2,1));  //scale it
	objs.push_back(myGroundPlane);
	
	//particle system
	myParticles = new Shape(5, "crate_texture.ppm", 0, 0, 0);
	myParticles->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	myParticles->setModelMatrix(Scale(3,1,3));
	objs.push_back(myParticles);

	

	//trees
	tree1 = new Shape(2, "bark.ppm", 0, 0, 0);
	tree1->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	tree1->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, 1, -0.3 - tempt + 0.4)*Scale(0.7, 2.0, 0.7));  //scale it
	objs.push_back(tree1);
	
	leaves1 = new Shape(2, "tree.ppm", 0, 0, 0);
	leaves1->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	leaves1->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-4, -1, -0.1 - tempt + 0.2)*Scale(2.5, 2, 2.5));  //scale it
	objs.push_back(leaves1);

	tree2 = new Shape(2, "bark.ppm", 0, 0, 0);
	tree2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	tree2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(4, 1, -0.3 - tempt + 0.4)*Scale(0.7, 2.0, 0.7));  //scale it
	objs.push_back(tree2);

	leaves2 = new Shape(2, "tree.ppm", 0, 0, 0);
	leaves2->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	leaves2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(4, -1, -0.1 - tempt + 0.2)*Scale(2.5, 2, 2.5));  //scale it
	objs.push_back(leaves2);

	tree3 = new Shape(2, "bark.ppm", 0, 0, 0);
	tree3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	tree3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(7, 1, 1.2)*Scale(0.7, 2.0, 0.7));  //scale it
	objs.push_back(tree3);

	leaves3 = new Shape(2, "tree.ppm", 0, 0, 0);
	leaves3->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	leaves3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(7, -1, 1.3)*Scale(2.5, 2, 2.5));  //scale it
	objs.push_back(leaves3);

	tree4 = new Shape(2, "bark.ppm", 0, 0, 0);
	tree4->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	tree4->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-7, 1, 1.2)*Scale(0.7, 2.0, 0.7));  //scale it
	objs.push_back(tree4);

	leaves4 = new Shape(2, "tree.ppm", 0, 0, 0);
	leaves4->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	leaves4->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-7, -1, 1.3)*Scale(2.5, 2, 2.5));  //scale it
	objs.push_back(leaves4);

	tree5 = new Shape(2, "bark.ppm", 0, 0, 0);
	tree5->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	tree5->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-7, 1, -10)*Scale(0.7, 2.0, 0.7));  //scale it
	objs.push_back(tree5);

	leaves5 = new Shape(2, "tree.ppm", 0, 0, 0);
	leaves5->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	leaves5->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-7, -1, -10.1)*Scale(2.5, 2, 2.5));  //scale it
	objs.push_back(leaves5);

	tree6 = new Shape(2, "bark.ppm", 0, 0, 0);
	tree6->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	tree6->setModelMatrix(RotateX(90)*RotateY(0)*Translate(2, 1, -10)*Scale(0.7, 2.0, 0.7));  //scale it
	objs.push_back(tree6);

	leaves6 = new Shape(2, "tree.ppm", 0, 0, 0);
	leaves6->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	leaves6->setModelMatrix(RotateX(90)*RotateY(0)*Translate(2, -1, -10.1)*Scale(2.5, 2, 2.5));  //scale it
	objs.push_back(leaves6);




	//car

	carTop = new Shape(2, "car2.ppm", 0, 0, 0);
	carTop->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	carTop->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(-7, -1, 2)*Scale(1, 0.55, 0.55));  //scale it
	objs.push_back(carTop);

	carBottom = new Shape(2, "car2.ppm", 0, 0, 0);
	carBottom->setMaterial(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
	carBottom->setModelMatrix(RotateX(90)*RotateY(0)*Translate(-7, 0.6, -2)*Scale(0.7, 0.5, 0.5));  //scale it
	objs.push_back(carBottom);


	mySphere = new Shape(1, "crate_texture.ppm", 0, 0, 0);
	mySphere->setMaterial(vec4(0, 0, 0, 1), vec4(1, 0, 1, 1), vec4(1, 1.8f, 0, 1), 100);
	mySphere->setModelMatrix(Translate(-7.4, 1.7, 1.2)*Scale(0.15, 0.15, 0.15));  //scale it
	objs.push_back(mySphere);

	wheel1 = new Shape(1, "crate_texture.ppm", 0, 0, 0);
	wheel1->setMaterial(vec4(0, 0, 0, 1), vec4(1, 0, 1, 1), vec4(1, 1.8f, 0, 1), 100);
	wheel1->setModelMatrix(Translate(-6.75, 1.7, 1.2)*Scale(0.15, 0.15, 0.15));  //scale it
	objs.push_back(wheel1);

	wheel2 = new Shape(1, "crate_texture.ppm", 0, 0, 0);
	wheel2->setMaterial(vec4(0, 0, 0, 1), vec4(1, 0, 1, 1), vec4(1, 1.8f, 0, 1), 100);
	wheel2->setModelMatrix(Translate(-7.4, 2.3, 1.2)*Scale(0.15, 0.15, 0.15));  //scale it
	objs.push_back(wheel2);

	wheel3 = new Shape(1, "crate_texture.ppm", 0, 0, 0);
	wheel3->setMaterial(vec4(0, 0, 0, 1), vec4(1, 0, 1, 1), vec4(1, 1.8f, 0, 1), 100);
	wheel3->setModelMatrix(Translate(-6.75, 2.3, 1.2)*Scale(0.15, 0.15, 0.15));  //scale it
	objs.push_back(wheel3);


	
	//camera and lights init
	camera.positionCamera(vec4(eyeX, eyeY, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));
	camera.setToggle(2);
	
	lights.push_back(Light(vec4(light1X, light1Y , light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.0f, 1.0f, 1.0f, 1), vec4(light4X, light4Y, light4Z, light4W)));
	
	glEnable(GL_DEPTH_TEST); 

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void display( void )
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  //clear out the color of the framebuffer and the depth info from the depth buffer

	//draws all objects in draw and in tempShapes
	for (int i = 0; i < objs.size(); i++) {
		objs[i]->draw(camera, lights);
	}

	for (int i = 0; i < tempShapes.size(); i++) {
		tempShapes[i]->draw(camera, lights);
	}


	
	
	//glFlush();
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void resize(int w, int h){
	glViewport(0,0,(GLsizei) w, (GLsizei) h);  //make the viewport the entire window
	camera.changeProjection(w, h);		//update the projection matrix so it has the aspect ratio of the window
}

//move in and out
void keyboard( unsigned char key, int x, int y )
{
   if(key == 'q') { camera.positionCamera(vec4(eyeX, eyeY+=1, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0)); }
   if (key == 'Q') { camera.positionCamera(vec4(eyeX, eyeY -= 1, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0)); }
}

void ArrowInput(int key, int x, int y)
{

	//moves camera up/down y direction
	if (key == GLUT_KEY_DOWN) { camera.positionCamera(vec4(eyeX, eyeY, eyeZ+=0.5, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0)); lights.clear();
	lights.push_back(Light(vec4(light1X, light1Y, light1Z+=0.5, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(0.2f, 0.2f, 0.2f, 1), vec4(light4X, light4Y, light4Z, light4W)));
	glutPostRedisplay();
	}

	if (key == GLUT_KEY_UP) { camera.positionCamera(vec4(eyeX, eyeY, eyeZ-=0.5, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0)); lights.clear();
	lights.push_back(Light(vec4(light1X, light1Y, light1Z-=0.5, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(0.2f, 0.2f, 0.2f, 1), vec4(light4X, light4Y, light4Z, light4W)));
	glutPostRedisplay();
	}

	if (key == GLUT_KEY_RIGHT) { camera.positionCamera(vec4(eyeX += 0.5, eyeY, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0)); lights.clear();
	lights.push_back(Light(vec4(light1X+=0.5, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(0.2f, 0.2f, 0.2f, 1), vec4(light4X, light4Y, light4Z, light4W)));
	glutPostRedisplay();
	}
	if (key == GLUT_KEY_LEFT) { camera.positionCamera(vec4(eyeX -= 0.5, eyeY, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0)); lights.clear();
	lights.push_back(Light(vec4(light1X-=0.5, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(0.2f, 0.2f, 0.2f, 1), vec4(light4X, light4Y, light4Z, light4W)));
	glutPostRedisplay();
	}



	glutPostRedisplay();
}

void close(){
	delete mySphere; 
	delete myCube;//call destructor on close
}


//wasd camera controller to change look value
void mykey(unsigned char key, int x, int y) {
	if (key == 'a') {
		camera.positionCamera(vec4(eyeX, eyeY, eyeZ, 0), vec4(atX+=0.2, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));


		lights.clear();
		lights.push_back(Light(vec4(light1X, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.9f, 1.9f, 1.9f, 1), vec4(light4X, light4Y, light4Z, light4W)));
		glutPostRedisplay();
		


	}

	if (key == 'd') {
		camera.positionCamera(vec4(eyeX, eyeY, eyeZ, 0), vec4(atX -= 0.2, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));

		lights.clear();
		lights.push_back(Light(vec4(light1X, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.9f, 1.9f, 1.9f, 1), vec4(light4X, light4Y, light4Z, light4W)));
		glutPostRedisplay();


	}

	if (key == 'w') {
		camera.positionCamera(vec4(eyeX, eyeY, eyeZ, 0), vec4(atX, atY , atZ+=0.2, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));


		lights.clear();
		lights.push_back(Light(vec4(light1X, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.9f, 1.9f, 1.9f, 1), vec4(light4X, light4Y, light4Z, light4W)));
		glutPostRedisplay();


	}

	if (key == 's') {
		camera.positionCamera(vec4(eyeX, eyeY, eyeZ, 0), vec4(atX, atY, atZ-=0.2, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));
		lights.clear();
		lights.push_back(Light(vec4(light1X, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.9f, 1.9f, 1.9f, 1), vec4(light4X, light4Y, light4Z, light4W)));
		glutPostRedisplay();
	}

	if (key == 'Q') {
		camera.positionCamera(vec4(eyeX, eyeY+=0.2, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));

		lights.clear();
		lights.push_back(Light(vec4(light1X, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.9f, 1.9f, 1.9f, 1), vec4(light4X, light4Y, light4Z, light4W)));
		glutPostRedisplay();


	}

	if (key == 'q') {
		camera.positionCamera(vec4(eyeX, eyeY -=0.2 , eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));

		lights.clear();
		lights.push_back(Light(vec4(light1X, light1Y, light1Z, light1W), vec4(light2X, light2Y, light2Z, light2W), vec4(1.9f, 1.9f, 1.9f, 1), vec4(light4X, light4Y, light4Z, light4W)));
		glutPostRedisplay();


	}


	//toggles the spotlight if enabled
	if (key == 32) {


		camera.setToggle(0);
		glutPostRedisplay();
		display();
	}

	if (key == 'm') {
		camera.setToggle(1);
		glutPostRedisplay();
		display();

	}


	//roll pitch yaw
	if (key == 'z') {camera.posRoll();}
	if (key == 'Z') {camera.negRoll();}

	if (key == 'x') { camera.posPitch();}
	if (key == 'X') { camera.negPitch();}

	if (key == 'c') { camera.posYaw();}
	if (key == 'C') { camera.negYaw();}

	if (key == 't') {
		myCube = new Shape(2, "brks.ppm", 0, 0, 0);
		myCube->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
		myCube->setModelMatrix(RotateX(0)*RotateY(0)*Translate(1, 1, 0));
		glutPostRedisplay();
	}
	if (key == 'T') { 
	
		myCube = new Shape(2, "crate_texture.ppm", 0, 0, 0);
		myCube->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
		myCube->setModelMatrix(RotateX(0)*RotateY(0)*Translate(1, 1, 0));
		glutPostRedisplay();
	
	
	}


	if (key == 'u') {
		timerCallback(0);
		//camera.setToggle(2);

	}


	//second camera
	if (key == 'o') {
		camera.positionCamera(vec4(eye2X, eye2Y, eye2Z, 0), vec4(at2X, at2Y, at2Z, 0), vec4(up2X, up2Y, up2Z, 0), vec4(1, 0, 0, 0));

	}

	//resets camera to default
	if (key == 'O') {
		eyeX = 0;
		eyeY = 20;
		eyeZ = -1;

		atX = 0;
		atY = 1;
		atZ = 0;

		upX = 0;
		upY = 0;
		upZ = -1;

		camera.positionCamera(vec4(eyeX, eyeY, eyeZ, 0), vec4(atX, atY, atZ, 0), vec4(upX, upY, upZ, 0), vec4(1, 0, 0, 0));
	}

	//not used
	if (key == 'p') {
		glutIdleFunc(idle);

	}
	




	glutPostRedisplay();
}


//moves the people and car back and forth
void timerCallback(int value)
{
		
	
		body->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u, 1, -0.1)*Scale(0.3, 0.3, 0.3));  //scale it

		armLeft->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u, 1, 0.1)*Scale(0.1, 0.5, 0.1));  //scale it

		armRight->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u, 1, -0.3)*Scale(0.1, 0.5, 0.1));  //scale it

		legLeft->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u, 1.3, 0)*Scale(0.1, 0.3, 0.1));  //scale it

		legRight->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u, 1.3, -0.2)*Scale(0.1, 0.3, 0.1));  //scale it
		
		head->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u, 0.79, -0.1)*Scale(0.2, 0.2, 0.2));  //scale it
	
		faceFix2->setModelMatrix(RotateX(90)*RotateY(0)*Translate(u - 0.01, 0.79, -0.1)*Scale(0.2, 0.21, 0.21));  //scale it

		

		
		
		body3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g, 1, -0.1 - tempt + 0.9)*Scale(0.3, 0.3, 0.3));  //scale it
		
		armLeft3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g, 1, 0.1 - tempt + 0.9)*Scale(0.1, 0.5, 0.1));  //scale it
	
		armRight3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g, 1, -0.3 - tempt + 0.9)*Scale(0.1, 0.5, 0.1));  //scale it
		
		legLeft3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g, 1.3, 0 - tempt + 0.9)*Scale(0.1, 0.3, 0.1));  //scale it
	
		legRight3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g, 1.3, -0.2 - tempt + 0.9)*Scale(0.1, 0.3, 0.1));  //scale it
	
		head3->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g, 0.79, -0.1 - tempt + 0.9)*Scale(0.2, 0.2, 0.2));  //scale it

		faceFix->setModelMatrix(RotateX(90)*RotateY(0)*Translate(g - 0.01, 0.79, -0.1 - tempt + 0.9)*Scale(0.2, 0.21, 0.21));


	
		carTop->setModelMatrix(RotateX(-90)*RotateY(0)*Translate(h, -1, 2)*Scale(1, 0.55, 0.55));  //scale it

		carBottom->setModelMatrix(RotateX(90)*RotateY(0)*Translate(h, 0.6, -2)*Scale(0.7, 0.5, 0.5));  //scale it

		mySphere->setModelMatrix(Translate(h - 0.4, 1.7, 1.2)*Scale(0.15, 0.15, 0.15)*RotateX(rotateX)*RotateY(rotateY));  //scale it

		wheel1->setModelMatrix(Translate(h+0.25, 1.7, 1.2)*Scale(0.15, 0.15, 0.15)*RotateX(rotateX)*RotateY(rotateY));  //scale it

		wheel2->setModelMatrix(Translate(h-0.4, 2.3, 1.2)*Scale(0.15, 0.15, 0.15)*RotateX(rotateX)*RotateY(rotateY));  //scale it

		wheel3->setModelMatrix(Translate(h+0.25, 2.3, 1.2)*Scale(0.15, 0.15, 0.15)*RotateX(rotateX)*RotateY(rotateY));  //scale it


		rotateX++;
		rotateY++;



		u += tempX;
		g += tempg;
		h += 0.1;

		if (u > 7) {
			tempX = -0.1;
			//rotateX = 270;

		}

		else if (u < 0) {
			tempX = 0.1;
			//rotateX = 90;
		}

		if (g < -8) {
			tempg = 0.1;
			//rotateX = 270;

		}

		else if (g > 9) {
			tempg = -0.1;
			//rotateX = 90;
		}

		if (g > -2) {
			h = -7;
		}
		
		//display();
		counter++;
		angle++;
		glutTimerFunc(10, timerCallback, value);
		glutPostRedisplay();
	
}


//particle idle func
void idle() {

	myParticles->updateParticles();
	glutPostRedisplay();
}


//detects click and places cube based on camera coordinates
//also removes all placed objects
void mymouse(GLint button, GLint state, GLint x, GLint y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		for (int i = 0; i < tempShapes.size(); i++) {
			tempShapes[i]->setModelMatrix(Scale(0, 0, 0));
		}
	}

	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		//newX = (float)eyeX / (512 / 2);
		//newY = (float)(512 - eyeY) / (512 / 2);

		
		float newEyeZ = eyeZ + 4;
		Shape *tempShape = new Shape(7, "crate_texture.ppm", eyeX, eyeY-3, eyeZ);
		tempShape->setMaterial(vec4(1, 1.1f, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1.4f, 0, 1), 200);
		tempShape->setModelMatrix(Translate(0, 0,0)*Scale(1,1,1));
		//display();
		cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
		cout << eyeX;
		cout << eyeY;
		cout << eyeZ;
		
		cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

		tempShapes.push_back(tempShape);
		
	}
	glutPostRedisplay();
	
}


