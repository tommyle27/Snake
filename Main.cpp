
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>

#include "pixmap/RGBpixmap.h"
#include "game.h"
#include "snake.h"
#include "food.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ROWS 16.0
#define COLUMNS 16.0    //creates dimensions for grid for the dimensions of the screen

GLint winWidth = 600, winHeight = 600;

bool lose = false;   //boolean expression to check if the player loses

extern int snakeDir;  //making the direction variable accessible from the game file

int score = 0;

int FPS = 8;      //adjusts the speed of the snake (larger = faster)

GLuint texture[5];     //from "TexureSound.cpp"

struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};

int ImageLoad(char *filename, Image *image) {
	FILE *file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)

	char temp; // temporary color storage for bgr-rgb conversion.

	// make sure the file is there.
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}

	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);

	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}

	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}

	size = image->sizeX * image->sizeY * 3;

	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}

	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}

	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}

	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}

	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);

	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}

	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}

	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	return 1;
}

Image* loadTexture(char *filename) {
	Image *image = (Image *) malloc(sizeof(Image));
	if (image == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad(filename, image)) {
		exit(1);
	}
	return image;
}

void init()     //need to add textures here
{
    glClearColor(0.0,0.0,0.0,0.0);    //create colour of background
    initGrid(COLUMNS,ROWS);

	glGenTextures(4, texture);

	Image* image = loadTexture("cake.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

	image = loadTexture("creep.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

	image = loadTexture("creep2.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

	image = loadTexture("lava.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

	image = loadTexture("brick.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture


	glEnable(GL_TEXTURE_2D);

    //initGrid(COLUMNS,ROWS);
}


void display(void)  //display the screen
{
    if(lose) {    //display game over window with score if player has lost

    	char scores[10];
    	itoa(score, scores,10);
        char text[50]= "Your score: ";
        strcat(text, scores);
        PlaySound(NULL, NULL, NULL);
        MessageBox(NULL,text,"YOU LOSE!",0);
        exit(0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2d(0,0);
        glTexCoord2f(26,0);
        glVertex2d(26,0);
        glTexCoord2f(26,26);
        glVertex2d(26,26);
        glTexCoord2f(0,26);
        glVertex2d(0,26);
    glEnd();

    draw_grid();
    draw_food();
    draw_snake();
    glutSwapBuffers();           //swap buffers to display the next screen
}
void winReshapeFcn(GLint w, GLint h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,COLUMNS,0.0,ROWS,-1.0,1.0);   //create co-ordinate system with origin at 0/bottom left, and 40 units Up and Right. -1, and 1 balance out the Z axis.
    											//-1 = into the screen, 1 = popping out of the screen, therefore the z points lies between -1 and 1.
    glMatrixMode(GL_MODELVIEW);               //must switch back to the model view matrix as attributes take place during then.
    glLoadIdentity();
    winWidth = w;
    winHeight = h;
}
void timer(int)
{
    glutPostRedisplay();                     //calls the display function next
    glutTimerFunc(1000/FPS,timer,0);     //form a continuous loop, adjusting FPS for milliseconds (1000 milliseconds in 1 second/FPS value)
}
void keyboard(int key,int,int)   //takes constant int value of key that's pressed, and x and y of mouse (not needed)
{
    switch(key)                        //checking which key is pressed
    {
    case GLUT_KEY_UP:
        if(snakeDir!=DOWN)           //if it's moving upwards, it can't move downwards. So if the direction isn't already down, change to up
            snakeDir=UP;
        break;
    case GLUT_KEY_DOWN:
        if(snakeDir!=UP)             //if the direction isn't already up, change to down
            snakeDir=DOWN;
        break;
    case GLUT_KEY_RIGHT:
        if(snakeDir!=LEFT)          //if the direction isn't already left, change to right
            snakeDir=RIGHT;
        break;
    case GLUT_KEY_LEFT:
        if(snakeDir!=RIGHT)         //if the direction isn't already right, change to left
            snakeDir=LEFT;
        break;
    }
}

int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //single or double buffer windows, better for games
    glutInitWindowPosition(10,10);
    glutInitWindowSize(winWidth,winHeight);

    glutCreateWindow("SNAKE:");
    glutDisplayFunc(display);
    glutReshapeFunc(winReshapeFcn);
    glutSpecialFunc(keyboard);    //using direction arrows, takes first argument as integer to find with arrow is pressed
    glutTimerFunc(0,timer,0);    //call timer function, 0 to start with no delay, call timer function, and pass no values

    PlaySound((LPCSTR) "background.wav", NULL, SND_FILENAME | SND_NOSTOP | SND_ASYNC);

    init();
    glutMainLoop();

    return 0;
}
