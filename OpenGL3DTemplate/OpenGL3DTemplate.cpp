#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <ctime>
#include <string>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 3.0f, float eyeY = 3.0f, float eyeZ = 3.0f, float centerX = 2.0f, float centerY = 0.0f, float centerZ = 2.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

	void setFrontView() {
		eye = Vector3f(1.0f, 1.0f, 2.0f);
		center = Vector3f(1.0f, 1.0f, 0.0f);
		up = Vector3f(0.0f, 1.0f, 0.0f);


		// Rotate the view to the left
		float angle = 90.0f; // Adjust the angle as needed
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(angle)) + right * sin(DEG2RAD(angle));
		up = view.cross(right);
		center = eye + view;
	}


	void setTopView() {
		eye = Vector3f(1.0f, 3.0f, 1.0f);
		center = Vector3f(1.0f, 0.0f, 1.0f);
		up = Vector3f(0.0f, 0.0f, -1.0f);
	}

	void setSideView() {
		eye = Vector3f(1.0f, 1.0f, 2.0f);
		center = Vector3f(1.0f, 1.0f, 0.0f);
		up = Vector3f(0.0f, 1.0f, 0.0f);

		// Rotate the view to the left
		float angle = 30.0f; // Adjust the angle as needed
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(angle)) + right * sin(DEG2RAD(angle));
		up = view.cross(right);
		center = eye + view;
	}
};

class Player {
public:
	Vector3f position;
	float scale;
	float rotation;

	Player(float x = 2.0f, float y = 0.0f, float z = 2.0f, float s = 0.5f) {
		position = Vector3f(x, y, z);
		scale = s;
		rotation = 0.0f;
	}

	void draw() {
		
		// Draw head
		
		glPushMatrix();
		glColor3f(1.0f, 0.8f, 0.6f);
		glTranslated(position.x, position.y + 1.0, position.z);
		glutSolidSphere(scale * 0.2, 15, 15);
		glPopMatrix();

		
		// Draw body
		glPushMatrix();
		glColor3f(0.8f, 0.4f, 0.9f);
		glTranslated(position.x, position.y + 0.6, position.z);
		glScaled(scale * 0.75, scale * 1.2, scale * 0.5);
		
		glutSolidCube(1);
		glPopMatrix();

		// Draw limbs
		glPushMatrix();
		glColor3f(1.0f, 0.8f, 0.6f);
		glTranslated(position.x - scale * 0.3, position.y + 0.65, position.z);
		glScaled(scale * 0.75, scale * 0.1, scale * 0.1);
		
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 0.8f, 0.6f);
		glTranslated(position.x + scale * 0.3, position.y + 0.65, position.z);
		glScaled(scale * 0.75, scale * 0.1, scale * 0.1);
		
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 0.8f, 0.6f);
		glTranslated(position.x - 0.1, position.y + 0.3, position.z + scale * 0.3);
		glScaled(scale * 0.1, scale * 0.1, scale * 0.5);
		
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 0.8f, 0.6f);
		glTranslated(position.x + 0.1, position.y + 0.3, position.z + scale * 0.3);
		glScaled(scale * 0.1, scale * 0.1, scale * 0.5);
		
		glutSolidCube(1);
		glPopMatrix();

	}

	void moveX(float d) {
		position.x += d;
		updateRotation();
	}

	void moveY(float d) {
		position.y += d;
		updateRotation();
	}

	void moveZ(float d) {
		position.z += d;
		updateRotation();
	}

private:
	void updateRotation() {
		// Calculate the new rotation angle based on the movement direction
		rotation = atan2(position.x, position.z) * (180.0 / M_PI);
	}
};

Player player;
Camera camera;
float WALL_MIN_X = 0.3f; //0.3
float WALL_MAX_X = 7.2f; //7.2
float WALL_MIN_Y = -0.25f; //-0.25
float WALL_MAX_Y = 3.0f; //3.0
float WALL_MIN_Z = 0.1f; //0.1
float WALL_MAX_Z = 4.8f; //4.8
bool goal = false;
int timeLimitSeconds = 60;  // Set the time limit in seconds
time_t startTime;
bool timeDone = false;
const int colorChangeInterval = 2; // Change color every 5 seconds
int elapsedTime = 0;
bool working = false;
float carouselAngle = 0.0f;
float ferrisWheelAngle = 0.0f;
float seesawAngle = 0.0f;
float bendAngle = 20.0f;
float balloonVerticalPosition = 1.7f;
float orangeVerticalPosition = 2.5f;
float hauntedHouseVerticalPosition = 0.0f;
bool moveUp = true;  // Flag to control the direction of movement



void timer(int value) {
	time_t currentTime = time(nullptr);
	elapsedTime = static_cast<int>(currentTime - startTime);

	// Check if the time limit is reached
	if (elapsedTime >= timeLimitSeconds) {
		std::string resultMessage = "Time's up! You ";

		// Check the goal to determine the result
		if (goal) {
			std::cout << "You win!" << std::endl;
			resultMessage += "win!";
		}
		else {
			std::cout << "You lose!" << std::endl;
			resultMessage += "lose!";
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glColor3f(0.0, 1.0, 0.0);  // Set text color to white
		glRasterPos2f(-0.5, 0);   // Set the position for the text

		// Display the result message character by character
		for (const char& c : resultMessage) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
		timeDone = true;
	}
	else {
		glutTimerFunc(1000, timer, 0);  // Call the timer function after 1000 milliseconds (1 second)
	}

	glutPostRedisplay();  // Request a redraw to update the displayed message
}

void initialize() {
	// Other initialization code goes here
	startTime = time(nullptr);  // Record the start time
	glutTimerFunc(1000, timer, 0);  // Start the timer
}



void drawPlayer() {
	player.draw();
}


void drawCylinder(float radius, float height, int sides) {
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; ++i) {
		float theta = i * 2.0f * M_PI / sides;
		float x = radius * cos(theta);
		float y = radius * sin(theta);

		glVertex3f(x, y, 0.0f);
		glVertex3f(x, y, height);
	}
	glEnd();
}

void drawWheelSupport(float radius, float height, int sides) {
	glPushMatrix();
	drawCylinder(radius, height, sides);
	glPopMatrix();
}

void drawFerrisWheel() {
	int sides = 35;
	float wheelRadius = 1.3f;
	float cabinRadius = 0.4f;
	float cabinHeight = 0.6f;
	float supportHeight = 3.0f;


	glPushMatrix();
	glTranslated(0.3, 0, -1.0);
	// Draw Ferris wheel support structure
	glPushMatrix();
	glColor3f(0.9f, 0.8f, 1.0f);
	glTranslated(-1.0, 0.0, 1.4);
	drawWheelSupport(0.1f, supportHeight * 0.1, sides);
	glPopMatrix();


	// Draw Ferris wheel wheel
	glColor3f(0.6f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(-1.0f, 1.4f, supportHeight - 1.55);
	glRotatef(ferrisWheelAngle, 0.0, 0.0, 1.0);
	drawCylinder(wheelRadius, 0.2f, sides);
	glPopMatrix();

	glPushMatrix();
	// Draw lines from top of the cylinder to the bottom
	glBegin(GL_LINES);
	for (int i = 0; i <= sides; ++i) {
		float theta = i * 2.0f * M_PI / sides;
		float x = wheelRadius * cos(theta);
		float y = wheelRadius * sin(theta);
		if (i % 2 == 0) {
			glColor3f(0.6f, 0.8f, 1.0f);
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex3f(-1.0f + x, 1.4f + y, supportHeight - 1.35);

		glVertex3f(-1.0f, 1.0f, supportHeight - 1.35);
	}
	glEnd();
	glPopMatrix();

	glPopMatrix();
}



void drawWall(double thickness) {


	// Draw the first cube

	glPushMatrix();
	if (elapsedTime % 2 == 0) {
		glColor3f(1.0f, 0.5f, 0.5f);
	}
	else {
		glColor3f(0.8f, 0.4f, 0.9f);
	}
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	if (elapsedTime % 2 == 0) {
		glColor3f(0.8f, 0.4f, 0.9f);
	}
	else {
		glColor3f(1.0f, 0.5f, 0.5f);
	}
	glTranslated(1.0, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	/*glRotated(45, 0, 0, 0);*/
	glutSolidCube(1);
	glPopMatrix();


}



void drawSeesaw() {
	glPushMatrix();
	glTranslated(0.2, 0, 0);
	// Draw the main beam (cylinder)
	glColor3f(0.9f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, -1.6f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate the cylinder to stand upright
	drawCylinder(0.1, 0.4, 50);
	glPopMatrix();

	// Draw the seesaw plank (cuboid)
	glColor3f(0.6f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, -1.6f, 0.0f); // Position the plank above the center
	glRotatef(bendAngle, 0.0f, 0.0f, 1.0f);
	glScalef(2.0, 0.2, 0.5); // Scale the cuboid to create the plank
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw the left support (cuboid)
	glColor3f(0.9f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(-0.6f, -1.4f - 0.5f * sin(seesawAngle), 0.0f); // Move the left seat up and down using sine function
	glScalef(0.1, 1.0, 0.1); // Scale the cuboid to create the support
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw the right support (cuboid)
	glColor3f(0.9f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(0.6f, -1.4f + 0.5f * sin(seesawAngle), 0.0f); // Move the right seat up and down using sine function
	glScalef(0.1, 1.0, 0.1); // Scale the cuboid to create the support
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f); // Rotate the cylinder to stand upright
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw the left handle (cylinder)
	glColor3f(0.6f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(-0.6f, -1.0f - 0.5f * sin(seesawAngle), -0.25f); // Move the left seat up and down using sine function
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f); // Rotate the cylinder to stand upright
	drawCylinder(0.05, 0.5, 50);
	glPopMatrix();

	// Draw the left handle (cylinder)
	glColor3f(0.6f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslatef(0.6f, -1.0f + 0.5f * sin(seesawAngle), -0.25f); // Move the right seat up and down using sine function
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f); // Rotate the cylinder to stand upright
	drawCylinder(0.05, 0.5, 50);
	glPopMatrix();
	glPopMatrix();
}

void drawCylinder1(float radius, float height, int sides) {
	glBegin(GL_QUADS);
	for (int i = 0; i < sides; ++i) {
		float theta = i * 2.0f * M_PI / sides;
		float x = radius * cos(theta);
		float y = radius * sin(theta);

		float nextTheta = (i + 1) * 2.0f * M_PI / sides;
		float nextX = radius * cos(nextTheta);
		float nextY = radius * sin(nextTheta);

		// Draw the side of the cylinder
		glVertex3f(x, y, 0.0f);
		glVertex3f(nextX, nextY, 0.0f);
		glVertex3f(nextX, nextY, height);
		glVertex3f(x, y, height);
	}
	glEnd();

	// Draw the top and bottom circles
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);  // Center of the top circle
	for (int i = 0; i <= sides; ++i) {
		float theta = i * 2.0f * M_PI / sides;
		float x = radius * cos(theta);
		float y = radius * sin(theta);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, height);  // Center of the bottom circle
	for (int i = 0; i <= sides; ++i) {
		float theta = i * 2.0f * M_PI / sides;
		float x = radius * cos(theta);
		float y = radius * sin(theta);
		glVertex3f(x, y, height);
	}
	glEnd();
}


void drawCarousel() {
	int sides = 20;
	float wheelRadius = 0.9f;
	float cabinRadius = 0.15f;
	float cabinHeight = 0.4f;
	float supportHeight = 3.0f;

	glPushMatrix();
	glTranslated(2.0, 4.0, 4.0);

	// Draw Carousel support structure
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glColor3f(0.9f, 0.8f, 1.0f);
	glTranslated(-1.0, 0.0, 1.7);
	drawWheelSupport(0.1f, supportHeight * 0.75, sides);
	glPopMatrix();

	// Draw Carousel wheel
	glColor3f(0.6f, 0.8f, 1.0f);
	glPushMatrix();
	glTranslated(-1.0, -3.855, 0);
	glRotated(90, 1, 0, 0);
	drawCylinder1(wheelRadius, 0.1f, sides);
	glPopMatrix();

	glPushMatrix();

	// Draw Carousel wheel
	glColor3f(0.6f, 0.8f, 1.0f);
	glPushMatrix();

	glTranslated(-1.0, -1.65, 0);
	glRotated(90, 1, 0, 0);
	glRotatef(carouselAngle, 0.0, 0.0, 1.0);
	drawCylinder1(wheelRadius, 0.1f, sides);
	glPopMatrix();

	// Draw lines from the top of the cylinder to some point below
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(0, 1.65, 3.2);
	glBegin(GL_LINES);
	for (int i = 0; i <= 10; ++i) {
		float theta = i * 2.0f * M_PI / 10;
		float x = wheelRadius * cos(theta);
		float y = wheelRadius * sin(theta);

		glVertex3f(-1.0 + x, -1.65 + y, 0.0);  // Top of the cylinder
		glVertex3f(-1.0 + x, -1.65 + y, -1.5); // Point below the cylinder
	}


	glEnd();
	//glColor3f(1.0f, 0.5f, 0.5f);
	for (int i = 0; i < 10; ++i) {
		glPushMatrix();
		glTranslated(-1.0, -1.6, -1.7);
		float theta = i * 2.0f * M_PI / 10;
		float x = wheelRadius * cos(theta);
		float y = wheelRadius * sin(theta);
		glTranslatef(x, y, supportHeight - 1.55);
		glColor3f((i % 2 == 0) ? 1.0f : 1.0f, (i % 2 == 0) ? 1.0f : 0.5f, 1.0f);
		glRotatef(carouselAngle, 0.0, 0.0, 1.0);
		drawCylinder1(cabinRadius, cabinHeight, sides);
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();



	glPopMatrix();
}










void setupLights() {
	// Material properties
	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Increased ambient intensity
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Increased diffuse intensity
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	// Light properties
	GLfloat lightIntensity[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Increased intensity
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightIntensity);

	// Enable lighting and light 0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);  // Enable normalization of normals for proper lighting
}


void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1200 / 750, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void drawBalloonCar() {
	// Draw the car body
	glPushMatrix();
	glColor3f(0.9f, 0.8f, 1.0f);
	glTranslated(0.7, 0.5, 2);
	glScaled(0.5, 0.9, 1.0);  // Adjust the scale as needed
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw front left wheel
	glPushMatrix();
	glColor3f(0.6f, 0.8f, 1.0f);
	glTranslated(0.5, 0.065, 2.25);
	drawCylinder1(0.05, 0.02, 50);
	glPopMatrix();

	// Draw front left wheel
	glPushMatrix();
	glColor3f(0.6f, 0.8f, 1.0f);
	glTranslated(0.5, 0.065, 1.7);
	drawCylinder1(0.05, 0.02, 50);
	glPopMatrix();

	// Draw lines from the car to balloons
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(0.7, 0.75, 2);  // Top center of the car body
	glVertex3f(0.7, 1.5, 2);   // Adjust the height of the line endpoint as needed

	// Draw three balloons connected to the line
	glVertex3f(0.7, 1.5, 2);
	glVertex3f(0.8, 1.7, 2);// Balloon 1



	glVertex3f(0.7, 1.5, 2);
	glVertex3f(0.7, 1.8, 2);  // Balloon 2

	glVertex3f(0.7, 1.5, 2);
	glVertex3f(0.6, 1.7, 2);  // Balloon 3

	glEnd();
	glPopMatrix();

	// Draw three balloons as spheres
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);  // Red color for balloons
	glTranslated(0.8, balloonVerticalPosition, 2);
	glutSolidSphere(0.1, 20, 20);  // Adjust the radius as needed
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);  // Green color for balloons
	glTranslated(0.7, balloonVerticalPosition + 0.1, 2);
	glutSolidSphere(0.1, 20, 20);  // Adjust the radius as needed
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);  // Blue color for balloons
	glTranslated(0.6, balloonVerticalPosition, 2);
	glutSolidSphere(0.1, 20, 20);  // Adjust the radius as needed
	glPopMatrix();



}



void drawHauntedHouseEntrance() {
	glPushMatrix();
	glScaled(0.8, 0.8, 0.8);
	glTranslated(3, 1.25 + hauntedHouseVerticalPosition, 3);  // Adjust the position using the variable
	glRotated(180, 0, 1, 0);

	// Draw the haunted house structure
	glColor3f(0.9f, 0.8f, 1.0f);  // Dark red-brown color for the haunted house
	glPushMatrix();
	glTranslated(-1.5, 0.0, -2.5);  // Adjust the position as needed

	// Draw the main building
	glPushMatrix();
	glScaled(2.0, 2.5, 1.5);  // Adjust the scale as needed
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw the roof
	glPushMatrix();
	glColor3f(0.6f, 0.8f, 1.0f);  // Darker color for the roof
	glTranslated(0.0, 1.3, 0.0);  // Adjust the position as needed
	glRotated(45, 0, 1, 0);  // Rotate the roof for a slanted appearance
	glutSolidCube(1.4);
	glPopMatrix();

	// Draw the door
	glColor3f(1.0f, 0.5f, 1.0f);  // Brown color for the door
	glPushMatrix();
	glTranslated(0.0, -0.5, 0.75);  // Adjust the position as needed
	glScaled(0.6, 1.0, 0.1);  // Adjust the scale as needed
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw windows
	glColor3f(1.0f, 0.5f, 1.0f);  // Dark blue-gray color for the windows

	// Window 1
	glPushMatrix();
	glTranslated(-0.8, 0.7, 0.75);  // Adjust the position as needed
	glScaled(0.3, 0.3, 0.1);  // Adjust the scale as needed
	glutSolidCube(1.0);
	glPopMatrix();

	// Window 2
	glPushMatrix();
	glTranslated(0.8, 0.7, 0.75);  // Adjust the position as needed
	glScaled(0.3, 0.3, 0.1);  // Adjust the scale as needed
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

void drawTree() {
	// Draw tree trunk
	glPushMatrix();
	glColor3f(0.5f, 0.35f, 0.2f);  // Brown color for trunk
	glTranslated(2.3, 2.0, 4.2);  // Adjust the position as needed
	glRotated(90, 1, 0, 0);
	glScaled(0.1, 0.1, 0.5);  // Adjust the scale as needed
	drawCylinder1(1, 4, 50);
	glPopMatrix();

	// Draw tree leaves
	glPushMatrix();
	glColor3f(0.0f, 0.5f, 0.0f);  // Green color for leaves
	glTranslated(2.3, 2.0, 4.2); // Adjust the position as needed
	glutSolidSphere(0.5, 15, 15);  // Adjust the radius as needed
	glPopMatrix();

	// Draw first orange
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);  // Orange color for oranges
	glTranslated(2.3, orangeVerticalPosition, 4.2);  // Adjust the position as needed
	glutSolidSphere(0.1, 15, 15);  // Adjust the radius as needed
	glPopMatrix();

	// Draw second orange
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);  // Orange color for oranges
	glTranslated(2.0, orangeVerticalPosition, 4.5);  // Adjust the position as needed
	glutSolidSphere(0.1, 15, 15);  // Adjust the radius as needed
	glPopMatrix();

	// Draw third orange
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);  // Orange color for oranges
	glTranslated(2.6, orangeVerticalPosition, 4.5);  // Adjust the position as needed
	glutSolidSphere(0.1, 15, 15);  // Adjust the radius as needed
	glPopMatrix();
}


void animateTimer(int value) {
	// Update rotation angles
	carouselAngle += 2.0f;  // Adjust the rotation speed as needed
	ferrisWheelAngle += 1.0f;  // Adjust the rotation speed as needed
	seesawAngle += 0.1f;
	bendAngle += 1.0f; // Adjust the rotation speed as needed
	if (bendAngle > 20.0f || bendAngle < -20.0f) {
		bendAngle = -bendAngle; // Reverse the rotation direction when reaching the limits
	}
	// Ensure angles stay within a reasonable range
	if (carouselAngle >= 360.0f) {
		carouselAngle -= 360.0f;
	}

	if (ferrisWheelAngle >= 360.0f) {
		ferrisWheelAngle -= 360.0f;
	}
	balloonVerticalPosition += 0.01f;  // Adjust the speed of animation as needed

	// Check if the balloons reach a certain height, then reset their position
	if (balloonVerticalPosition > 2.0f) {
		balloonVerticalPosition = 1.7f;
	}

	orangeVerticalPosition -= 0.1f;  // Adjust the speed of animation as needed

	// Check if the balloons reach a certain height, then reset their position
	if (orangeVerticalPosition < 1.0f) {
		orangeVerticalPosition = 2.5f;
	}


	// Redisplay the scene
	glutPostRedisplay();

	// Continue the animation if working is true
	if (working) {
		glutTimerFunc(16, animateTimer, 0);  // 60 frames per second (1000ms / 60fps)
	}
}

void animate() {
	if (!working) {
		working = true;
		glutTimerFunc(0, animateTimer, 0);  // Start the animation
	}
}

void stopAnimate() {
	working = false;
}

void animateHauntedHouse(int value) {
	const float speed = 0.01f;  // Adjust the movement speed as needed

	// Update the vertical position based on the direction of movement
	if (moveUp) {
		hauntedHouseVerticalPosition += speed;
	}
	else {
		hauntedHouseVerticalPosition -= speed;
	}

	// Reverse the direction when reaching certain limits
	if (hauntedHouseVerticalPosition >= 0.5f || hauntedHouseVerticalPosition <= -0.5f) {
		moveUp = !moveUp;
	}

	// Redisplay the scene
	glutPostRedisplay();

	// Continue the animation
	glutTimerFunc(16, animateHauntedHouse, 0);  // 60 frames per second (1000ms / 60fps)
}



void Display() {

	if (!timeDone) {
		setupCamera();
		setupLights();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glPushMatrix();
		glTranslated(0.4, 0.4, 0.6);
		glRotated(45, 0, 0, 1);
		glScaled(0.08, 0.08, 0.08);
		drawJack();
		glPopMatrix();


		glPushMatrix();
		glTranslated(0.6, 0.38, 0.5);
		glRotated(30, 0, 1, 0);
		glutSolidTeapot(0.08);
		glPopMatrix();


		glPushMatrix();
		glTranslated(0.25, 0.42, 0.35);
		glutSolidSphere(0.1, 15, 15);
		glPopMatrix();


		glPushMatrix();
		glTranslated(0.4, 0.0, 0.4);
		drawTable(0.6, 0.02, 0.02, 0.3);
		glPopMatrix();*/

		drawPlayer();  // Draw the player

		glPushMatrix();
		glTranslated(3.0, 0.0, 0.0);
		drawFerrisWheel();
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.0, 0.0, 0.0);
		drawTree();
		glPopMatrix();

		if (!goal) {
			glPushMatrix();
			glTranslated(2.0, 0.0, 0.0);
			drawHauntedHouseEntrance();
			glPopMatrix();
			
		}
		animateHauntedHouse(0);
		glPushMatrix();
		drawCarousel();
		glPopMatrix();

		glPushMatrix();
		drawBalloonCar();
		glPopMatrix();

		glPushMatrix();
		glTranslated(5, 2.0, 0.5);
		drawSeesaw();
		glPopMatrix();

		glPushMatrix();
		glScaled(5.0, 1.0, 5.0);
		drawWall(0.02);
		glPopMatrix();

		glPushMatrix();
		glScaled(1.0, 3.0, 5.0);
		glRotated(90, 0, 0, 1.0);
		drawWall(0.02);
		glPopMatrix();


		glPushMatrix();
		glScaled(5.0, 3.0, 1.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		drawWall(0.02);
		glPopMatrix();


		glPushMatrix();
		glScaled(5.0, 3.0, 1.0);
		glTranslated(0.0, 0.0, 5.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		drawWall(0.02);
		glPopMatrix();

		
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		std::string resultMessage = "Time's up! You ";

		// Check the goal to determine the result
		if (goal) {
			std::cout << "You win!" << std::endl;
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			resultMessage += "win!";
		}
		else {
			std::cout << "You lose!" << std::endl;
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			resultMessage += "lose!";
		}

		glColor3d(1.0, 1.0, 1.0);  // Set text color to white
		glRasterPos2f(0, 0);   // Set the position for the text

		// Display the result message character by character
		for (const char& c : resultMessage) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
	}


	glFlush();
}



void Keyboard(unsigned char key, int x, int y) {
	float d = 0.05;
	float newX = 0;
	float newY = 0;
	float newZ = 0;
	float myX;
	float myY;
	float myZ;

	switch (key) {
	case 't': //top
		camera.setTopView();
		break;
	case 's': //side
		camera.setSideView();
		break;
	case 'f': //front
		camera.setFrontView();
		break;
	case 'u':
		camera.moveY(d);
		break;
	case 'd':
		camera.moveY(-d);
		break;
	case 'r':
		camera.moveX(d);
		break;
	case 'l':
		camera.moveX(-d);
		break;
	case 'c':
		camera.moveZ(d);
		break;
	case 'b':
		camera.moveZ(-d);
		break;
	case 'x':
		// Calculate the new position
		newX = player.position.x + d;
		myZ = player.position.z;

		// Check if the new position is within the boundaries
		if (newX >= WALL_MIN_X && newX <= WALL_MAX_X) {
			player.moveX(d);
		}
		if (newX >= 5 && newX <= 6 && myZ >= 4 && myZ <= 4.8) {
			goal = true;
		}

		break;
	case 'w':
		// Calculate the new position
		newX = player.position.x - d;
		myZ = player.position.z;

		// Check if the new position is within the boundaries
		if (newX >= WALL_MIN_X && newX <= WALL_MAX_X) {
			player.moveX(-d);
		}

		if (newX >= 5 && newX <= 6 && myZ >= 4 && myZ <= 4.8) {
			goal = true;
		}
		break;
	case 'y':
		// Calculate the new position
		newY = player.position.y + d;

		// Check if the new position is within the boundaries
		if (newY >= WALL_MIN_Y && newY <= WALL_MAX_Y) {
			player.moveY(d);
		}

		break;
	case 'a':
		// Calculate the new position
		newY = player.position.y - d;

		// Check if the new position is within the boundaries
		if (newY >= WALL_MIN_Y && newY <= WALL_MAX_Y) {
			player.moveY(-d);
		}
		break;
	case 'z':
		// Calculate the new position
		newZ = player.position.z + d;
		myX = player.position.x;
		// Check if the new position is within the boundaries
		if (newZ >= WALL_MIN_Z && newZ <= WALL_MAX_Z) {
			player.moveZ(d);
		}

		if (myX >= 5 && myX <= 6 && newZ >= 4 && newZ <= 4.8) {
			goal = true;
		}
		break;
	case 'p':
		// Calculate the new position
		newZ = player.position.z - d;
		myX = player.position.x;
		// Check if the new position is within the boundaries
		if (newZ >= WALL_MIN_Z && newZ <= WALL_MAX_Z) {
			player.moveZ(-d);
		}

		if (myX >= 5 && myX <= 6 && newZ >= 4 && newZ <= 4.8) {
			goal = true;
		}
		break;
	case 'o':
		if (working) {
			stopAnimate();

		}
		else {
			animate();

		}
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}
	setupCamera();

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 2.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1200, 750);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Amusement Park");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	initialize();

	glutMainLoop();
}