/*
 * Final Project V3
 */
#include <windows.h>  								// for MS Windows
#include <GL/glut.h>  								// GLUT, include glu.h and gl.h
#include <Math.h>	  								// Math Library
#include <stdbool.h>  								//Boolean Statements
#include <stdlib.h>									//Three Libraries for lighting and shading
#include <stdarg.h>
#include <stdio.h>
 
/* Global variables */
char title[] = "Shape Course Final Project";		//Project Title
int refreshMills = 30;        						// refresh interval in 30 milliseconds
GLfloat angleCube = 0.0f;     						// Rotational angle for cube [NEW]
GLfloat angleReverse = 3.0f;						// Reverse the angle
float angle = 0.0f;									// angle of rotation for the camera direction	
float lx=0.0f,lz=0.0f, ly=0.0f;						// actual vector representing the camera's direction
float x=0.0f,z=-43.0f, y=0.0f; 						// XZ position of the camera

//Materials (Shading and Lighting)
enum {
    BRASS, RED_PLASTIC, EMERALD, SLATE
} MaterialType;
enum {
    CUBE1_MATERIAL = 1, CUBE2_MATERIAL = 2, CUBE3_MATERIAL = 3
} MaterialDisplayList;
enum {
    LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
} LightValues;

GLfloat red_light[] =
{1.0, 0.0, 0.0, 1.0}, green_light[] =
{0.0, 1.0, 0.0, 1.0}, white_light[] =
{1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] =
{-1.0, 0.0, 1.0, 0.0}, right_light_position[] =
{1.0, 0.0, 1.0, 0.0};
GLfloat brass_ambient[] =
{0.33, 0.22, 0.03, 1.0}, brass_diffuse[] =
{0.78, 0.57, 0.11, 1.0}, brass_specular[] =
{0.99, 0.91, 0.81, 1.0}, brass_shininess = 30.0;
GLfloat red_plastic_ambient[] =
{0.0, 0.0, 0.0}, red_plastic_diffuse[] =
{0.5, 0.0, 0.0}, red_plastic_specular[] =
{0.7, 0.6, 0.6}, red_plastic_shininess = 33.0;
GLfloat emerald_ambient[] =
{0.0215, 0.1745, 0.0215}, emerald_diffuse[] =
{0.07568, 0.61424, 0.07568}, emerald_specular[] =
{0.633, 0.727811, 0.633}, emerald_shininess = 79.8;
GLfloat slate_ambient[] =
{0.02, 0.02, 0.02}, slate_diffuse[] =
{0.02, 0.01, 0.01}, slate_specular[] =
{0.4, 0.4, 0.4}, slate_shininess = .90;

int shade_model = GL_SMOOTH;
char *left_light;
char *cube1_material, *cube2_material, *cube3_material;


bool fullScreenMode = true; 						// Full-screen or windowed mode
int windowWidth  = 640;     						// Windowed mode's width
int windowHeight = 480;     						// Windowed mode's height
int windowPosX   = 50;      						// Windowed mode's top-left corner x
int windowPosY   = 50;      						// Windowed mode's top-left corner y

//variables for speed and position of cubes
GLfloat cube1X = 0.0f, cube1Y = 0.0f;				
GLfloat cube2X = 0.0f, cube2Y = 0.0f;
GLfloat cube1Xspeed = 0.1f, cube1Yspeed = 0.1f;
GLfloat cube2Xspeed = 0.3f, cube2Yspeed = 0.1f;

void increaseCubeSpeed(){
   
   cube1X += cube1Xspeed;         						// Change the first shape speed in the x axis after each display()
   cube1Y += cube1Yspeed;     							// Change the first shape speed in the y axis after each display()
   cube2X += cube2Xspeed;         						// Change the first shape speed in the x axis after each display()
   cube2Y += cube2Yspeed;     							// Change the first shape speed in the y axis after each display()

}

void moveCube1(){
	
	// Animation Control - compute the location for the next refresh
   if (cube1X > 4.0) {
      cube1X = 4.0;
      cube1Xspeed= -cube1Xspeed; 
   } else if(cube1X < -4.0){
   	  cube1X = -4.0;
      cube1Xspeed = -cube1Xspeed; 
   	
   }
   
   if(cube1Y > 1.0) {
      cube1Y = 0.0;
      cube1Yspeed = -cube1Yspeed;
   } else if (cube1Y< 1.0) {
      cube1Y = 0.0;
      cube1Yspeed = -cube1Yspeed;
   }
	

	
}

void moveCube2(){
	
	// Animation Control - compute the location for the next refresh
   if (cube2X > 5.0) {
      cube2X = 5.0;
      cube2Xspeed= -cube2Xspeed; 
   } else if(cube2X < -5.0){
   	  cube2X = -5.0;
      cube2Xspeed = -cube2Xspeed; 
   	
   }
   
   if(cube2Y > 1.0) {
      cube2Y = 0.0;
      cube2Yspeed = -cube2Yspeed;
   } else if (cube2Y< 1.0) {
      cube2Y = 0.0;
      cube2Yspeed = -cube2Yspeed;
   }
   }




/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   			// Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    			// Set the type of depth-test
   glShadeModel(GL_SMOOTH);   			// Enable smooth shading
   glEnable(GL_LIGHTING);				//Enable Lighting
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
} 

 void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	// Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     						// To operate on model-view matrix
   glLoadIdentity();               						// Load the model-view matrix								  								  // Save model-view matrix setting
 	   	gluLookAt(	x, y, z,							// Camera ViewPoint
			x+lx, y+ly,  z+lz,
			0.0f, 1.0f,  0.0f);

	
   glPushMatrix();                     					// Save model-view matrix setting
   glTranslatef(cube1X, cube1Y, -5.0f);    				// Translate the shape
  glRotatef(angleCube, angleCube + 2.0, angleCube - 2.0, angleCube + 1.0); 				// Rotate by angle in degrees
 	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	  glCallList(CUBE3_MATERIAL);						//Calling the material of the cube
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
 
      // Bottom face (y = -1.0f)
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 

      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)

      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)

      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)
  
      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   	glEnd();  											// End of drawing color-cube
   	
    glPopMatrix();  									// Restore the model-view matrix
    
 	glPushMatrix();                     				// Save model-view matrix setting
   glTranslatef(cube2X, cube2Y, -15.0f);    			// Translate the shape
   glRotatef(angleCube, angleCube + 3.0, angleCube - 2.0, angleCube + 3.0);  				// Rotate by angle in degrees
 	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	  glCallList(CUBE2_MATERIAL);						//Calling the material of the cube
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      // Bottom face (y = -1.0f)
     
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      // Front face  (z = 1.0f)
    
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)
     
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)
  
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)

      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   	glEnd();  												// End of drawing color-cube
    glPopMatrix();  
    
    glPushMatrix();          
	           												// Save model-view matrix setting
   glTranslatef(cube1X, cube1Y, -25.0f);    				// Translate the shape
  glRotatef(angleCube, angleCube + 1.0, angleCube - 1.0, angleCube + 2.0);  					// Rotate by angle in degrees
 glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
 	  glCallList(CUBE3_MATERIAL);							//Calling the material of the cube
     
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      // Bottom face (y = -1.0f)
  
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      // Front face  (z = 1.0f)
   
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)
   
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)
      
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)
  
      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   	glEnd();  											// End of drawing color-cube
    glPopMatrix();   									// Restore the model-view matrix
    
    glPushMatrix();                     				// Save model-view matrix setting
    
   glTranslatef(cube2X, cube2Y, -35.0f);    			// Translate the shape
   glRotatef(angleCube, angleCube + 4.0, angleCube - 3.0, angleCube + 1.0); 				// Rotate by angle in degrees
 glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	  glCallList(CUBE1_MATERIAL);						//Calling the material of the cube
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      // Bottom face (y = -1.0f)

      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      // Front face  (z = 1.0f)

      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)
 
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)

      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)

      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   	glEnd();  													// End of drawing color-cube
   	  
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(-6.0f, 0.0f, -5.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(-6.0f, 0.0f, -15.0f); 							// Push eveything 5 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(-6.0f, 0.0f, -25.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    	
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(-6.0f, 0.0f, -35.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(-6.0f, 0.0f, -45.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(0.0f, 0.0f, -45.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, -45.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								//WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, -35.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
 	glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, -25.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, -15.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, -5.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, 5.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(6.0f, 0.0f, 15.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(0.0f, 0.0f, 15.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting

    glTranslatef(-6.0f, 0.0f, 15.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting  

    glTranslatef(-6.0f, 0.0f, 5.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting   
	glCallList(CUBE3_MATERIAL);
    glTranslatef(0.0f, 2.0f, 5.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
    
    glPushMatrix();   											// Save model-view matrix setting 
    glCallList(CUBE3_MATERIAL);
    glTranslatef(0.0f, 2.0f, -40.0f); 							// Push eveything 10 units back into the scene
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f); 							// Rotate our object around a vector or an axis
 	glutWireSphere(0.5, 20, 50);								// WireSphere
    
    glPopMatrix(); 												// Restore the model-view matrix
		         
    increaseCubeSpeed();
    moveCube1();												// Call the function to move the cube
    moveCube2();												// Call the function to move the cube

    angleCube -= angleReverse;
   	glutSwapBuffers();  
    
}

//Different Types of Light
void light_select(GLenum which, int value, char **label)
{
    glEnable(which);
    switch (value) {
        case LIGHT_OFF:
            *label = "off";
            glDisable(which);
            break;
        case LIGHT_RED:
            *label = "red";
            glLightfv(which, GL_DIFFUSE, red_light);
            break;
        case LIGHT_WHITE:
            *label = "white";
            glLightfv(which, GL_DIFFUSE, white_light);
            break;
        case LIGHT_GREEN:
            *label = "green";
            glLightfv(which, GL_DIFFUSE, green_light);
            break;
    }
    glutPostRedisplay();
}

//Light hitting from the right
void left_light_select(int value)
{
    light_select(GL_LIGHT0, value, &left_light);
}


//Material begin hit from the light
void material(int dlist, GLfloat * ambient, GLfloat * diffuse,
         GLfloat * specular, GLfloat shininess)
{
    glNewList(dlist, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glEndList();
}


//Different Types of material
char * material_select(int object, int value)
{
    glutPostRedisplay();
    switch (value) {
        case BRASS:
            material(object, brass_ambient,
                     brass_diffuse, brass_specular, brass_shininess);
            return "brass";
        case RED_PLASTIC:
            material(object, red_plastic_ambient, red_plastic_diffuse,
                     red_plastic_specular, red_plastic_shininess);
            return "red plastic";
        case EMERALD:
            material(object, emerald_ambient, emerald_diffuse,
                     emerald_specular, emerald_shininess);
            return "emerald";
        case SLATE:
            material(object, slate_ambient, slate_diffuse,
                     slate_specular, slate_shininess);
            return "slate";
    }
    return NULL; /* avoid bogus warning! */
}


//Cube 1 Material
 void
cube1_select(int value)
{
    cube1_material = material_select(CUBE1_MATERIAL, value);
}

//Cube 2 Material
void
cube2_select(int value)
{
    cube2_material = material_select(CUBE2_MATERIAL, value);
}

//Cube 3 Material
void
cube3_select(int value)
{
    cube3_material = material_select(CUBE3_MATERIAL, value);
}
 
/* Called back when timer expired  */
void timer(int value) {
   glutPostRedisplay();     				 	// Post re-paint request to activate display()
   glutTimerFunc(refreshMills, timer, 0); 		// next timer call milliseconds later
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

//Special Keys for the movement and full screen
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.2f;

	switch (key) {
		case GLUT_KEY_LEFT :
			angle -= 0.08f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.08f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
		      case GLUT_KEY_F1:    									// F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         			// Toggle state
         if (fullScreenMode) {                     		   // Full-screen mode
            windowPosX   = glutGet(GLUT_WINDOW_X); 		  // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      		  // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
         }
         break;

	}
}

//Hold down left mouse button to speed up and right mouse button to stop movement
void mouseButton(int button, int state, int x, int y) {

	// only start motion if the right button is pressed
	if (button == GLUT_RIGHT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {	
			angleReverse = -3.0f;
			cube1Xspeed  = 0.1f;
			cube2Xspeed  = 0.1f;
		}
		else  {// state = GLUT_DOWN
			cube1Xspeed  = 0.0f;
			angleReverse = 0.0f;
			cube2Xspeed  = 0.0f;
		}
	}
	
	
		// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {	
			angleReverse = -3.0f;
			cube1Xspeed  = 0.1f;
			cube2Xspeed  = 0.1f;
		}
		else  {// state = GLUT_DOWN
			cube1Xspeed  = 0.2f;
			angleReverse = 6.0f;
			cube2Xspeed  = 0.2f;
		}
	}
	
}

 
 /* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE  | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
   glutInitWindowSize(windowWidth, windowHeight);  				// Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); 				// Initial window top-left corner (x, y)
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glLightfv(GL_LIGHT1, GL_POSITION, left_light_position);		//For Light
   left_light_select(LIGHT_RED);								//Red Light is used
   cube1_select(BRASS);											//Cube 1 is brass material
   cube2_select(RED_PLASTIC);									//Cube 1 is red plastic material
   cube3_select(SLATE);											//Cube 1 is slate material
   glutSpecialFunc(processSpecialKeys); 						//Use the keyboard arrow keys
   glutMouseFunc(mouseButton);									//Use the mouse buttons
   initGL();                       								// Our own OpenGL initialization
   glutTimerFunc(0, timer, 0);     								// First timer call immediately
   glutMainLoop();                 								// Enter the infinite event-processing loop
   return 0;
}
