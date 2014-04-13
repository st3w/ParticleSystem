#include <GL/gl.h>
#include <GL/glut.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include "particle.h"
using namespace std;

vector<Particle> particles;
const unsigned numParticles = 20;
//float t = 0;
const float delta_t = 1.0/60.0;
const float gravity = 1.0;

void createParticles()
{
	for (unsigned i = 0; i < numParticles; i++) {
		Vec3f position(0.0f, 0.0f, -5.0f);
		
		Vec3f velocity( 1.0f * ( (float) rand() / (float) RAND_MAX - 0.5f), // x velocity
			 3.0f * ( (float) rand() / (float) RAND_MAX), // y velocity
			 1.0f * ( (float) rand() / (float) RAND_MAX - 0.5f) ); // z velocity

		Color color((float) rand() / (float) RAND_MAX, // red color
			(float) rand() / (float) RAND_MAX, // green color
			(float) rand() / (float) RAND_MAX); // blue color

		particles.emplace_back(position, velocity, color);	
	}	
}

void updateParticles()
{
	
	for (unsigned i = 0; i < numParticles; i++) {
		//update position
		
		// newPosition = oldPosition + vel*delta_t + 1/2*a*dt^2
		
		// Updates the y velocity (gravity)
		particles[i].velocity.y -= gravity*delta_t;

		// Updates the position
		particles[i].position.x += particles[i].velocity.x * delta_t;
		particles[i].position.y += particles[i].velocity.y * delta_t;
		particles[i].position.z += particles[i].velocity.z * delta_t;
	}

	glutPostRedisplay();
	// t+=delta_t;
}

void init() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	createParticles();
}

void reshape(int w, int h) {
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 100.0);
        glMatrixMode (GL_MODELVIEW);
}

void drawSquare() {
	glBegin(GL_QUADS);
		glVertex2d(-1.0, -1.0);
 		glVertex2d(1.0, -1.0);
		glVertex2d(1.0, 1.0);
		glVertex2d(-1.0, 1.0);
	glEnd();
}

void drawParticles()
{
	for (unsigned i = 0; i < numParticles; i++) {
		glPushMatrix();

		glTranslatef(particles[i].position.x, particles[i].position.y, particles[i].position.z);

                glDisable (GL_DEPTH_TEST);
                //glEnable (GL_BLEND);

                //glBlendFunc(GL_DST_COLOR, GL_ZERO);
                //glBindTexture(GL_TEXTURE_2D, texture[0]);
		
		glColor3f(particles[i].color.r, particles[i].color.g, particles[i].color.b);
		drawSquare();
		glEnable(GL_DEPTH_TEST);

		glPopMatrix();
	}
}
void display() {
	glClearDepth(1);
	glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -10);

	drawParticles();
	glutSwapBuffers();
}
 
int main(int argc, char** argv) { 
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(640, 480);
        glutInitWindowPosition(100, 100);
        glutCreateWindow ("CAP4730 Final Project Demo");
        init();
        glutDisplayFunc(display);
       	glutIdleFunc(updateParticles);
        glutReshapeFunc(reshape);
        glutMainLoop();
        return 0;
}
