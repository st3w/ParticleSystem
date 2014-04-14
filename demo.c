#include <GL/gl.h>
#include <GL/glut.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include "particle.h"
using namespace std;

vector<Particle> particles;
const unsigned numParticles = 200;
//float t = 0;
const float delta_t = 1.0/60.0;
const float gravity = 15.0;

GLuint LoadTextureRAW(const char * filename, int width, int height);
void FreeTexture(GLuint texture);
GLuint texture[2];

void ResetParticle(Particle& particle)
{
    Vec3f position(0.0f, 0.0f, -5.0f);

    Vec3f velocity( 8.0f * ( (float) rand() / (float) RAND_MAX - 0.5f), // x velocity
            15.0f * ( (float) rand() / (float) RAND_MAX), // y velocity
            8.0f * ( (float) rand() / (float) RAND_MAX - 0.5f) ); // z velocity

    int randNum = rand() % 3;
    float r, g, b;
    if (randNum == 0) {
        r = g = b = 1.0f;   
    }
    if (randNum == 1) {
        r = 1.0f;
        g = 74.0f/255.0f;
        b = 0.0f;
        r = r * 1.1f;
        g = g * 1.1f;
    }
    if (randNum == 2) {
        r = 0.0f;
        g = 33.0f/255.0f;
        b = 165.0f/255.0f;
        g = g * 1.4f;
        b = b * 1.4f;
    }

    Color color(r, g, b);
    /*    
    Color color((float) rand() / (float) RAND_MAX, // red color
            (float) rand() / (float) RAND_MAX, // green color
            (float) rand() / (float) RAND_MAX); // blue color
    */
    particle.position = position;
    particle.velocity = velocity;
    particle.color = color;
}

void createParticles()
{
	for (unsigned i = 0; i < numParticles; i++) {
        Particle p;
        ResetParticle(p);
		particles.push_back(p);	
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
	    
        if (particles[i].position.y < -10.0f) {
            ResetParticle(particles[i]);
        }
    }
    
	glutPostRedisplay();
	// t+=delta_t;
}

void init() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	texture[0] = LoadTextureRAW( "gator.raw", 256, 256);
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
        glTexCoord2d(0, 1);
		glVertex2d(-1.0, -1.0);
        glTexCoord2d(1, 1);
 		glVertex2d(1.0, -1.0);
        glTexCoord2d(1, 0);
		glVertex2d(1.0, 1.0);
        glTexCoord2d(0, 0);
		glVertex2d(-1, 1);
	glEnd();
}

void drawParticles()
{
	for (unsigned i = 0; i < numParticles; i++) {
		glPushMatrix();

		glTranslatef(particles[i].position.x, particles[i].position.y, particles[i].position.z);

        glDisable (GL_DEPTH_TEST);
        glEnable (GL_BLEND);

        //glBlendFunc(GL_ONE, GL_ONE);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glColor3f(particles[i].color.r, particles[i].color.g, particles[i].color.b);
        drawSquare();
        glEnable(GL_DEPTH_TEST);

		glPopMatrix();
	}
}

void display() 
{
	glClearDepth(1);
	glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -10);

	drawParticles();
	glutSwapBuffers();
}
 
int main(int argc, char** argv) 
{ 
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

GLuint LoadTextureRAW(const char * filename, int width, int height) {
    GLuint texture;
    unsigned char * data;
    FILE * file;

    file = fopen(filename, "rb");
    if (file == NULL)
        return 0;

    data = (unsigned char *) malloc(width * height * 3); 

    fread(data, width * height * 3, 1, file);
    fclose(file);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE,
            data);

    free(data);

    return texture;
}

void FreeTexture(GLuint texture) {
        glDeleteTextures(1, &texture);
}

