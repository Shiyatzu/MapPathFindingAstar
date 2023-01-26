// OFF Reader.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Grid.h";
#include "Cell.h"
#include <list>

//Hardcoded Map Name
//string filename = "arena.map";
string filename = "lak401d.map";
Grid map;

//CAMERA VARIABLES
float camera_x = 200.0f, camera_y = 300.0f, camera_z = 300.0f, camera_speed = 10.0f;


void initGL() { /* Initialize OpenGL Graphics */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	//OPEN MAP FROM FILE AND CREATE GRID
	map.gridFromMap(filename);

	//Set Camera positions based on map size
	camera_x = map.map.size() / 2;
	camera_y = map.map[0].size() / 2;
	camera_z = 2 * (camera_x > camera_y ? camera_x : camera_y); // camera height is double whaever is bigger
	cout << "\ncamera pos " << camera_x << " " << camera_y << " " << camera_z;
} 

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera_x, -camera_y, camera_z,   // eye position
		camera_x, -camera_y, 0,   // reference point
		0, 1, 0);
	glBegin(GL_QUADS);

	map.drawGrid();

	glEnd();
	glutSwapBuffers();
}
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1; // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);  // Set the viewport to cover the new window
	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100000.0f);
	glMatrixMode(GL_MODELVIEW);
}
void processNormalKeys(unsigned char key, int xx, int yy) {
	switch (key) {
		cout << camera_x, camera_y, camera_z;
	case 'x': //Start and End Position
		map.aStar(5, 169, 302, 5);
		break;
	case 'w':
		// Move camera forward
		camera_y -= camera_speed;
		break;
	case 's':
		// Move camera backward
		camera_y += camera_speed;
		break;
	case 'a':
		// Move camera left
		camera_x -= camera_speed;
		break;
	case 'd':
		// Move camera right
		camera_x += camera_speed;
		break;
	case 'q':
		// Zoom Out
		camera_z += camera_speed;
		break;
	case 'e':
		// Zoom In
		camera_z -= camera_speed;
		break;
	}
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(1000, 800);   // Set the window's initial width & height
	glutInitWindowPosition(1, 1); // Position the window's initial top-left corner
	glutCreateWindow("A* Pathfinding"); // Create window with the given title
	
	
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutKeyboardFunc(processNormalKeys);
	glutPostRedisplay();

	glEnable(GL_DEPTH_TEST);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
