#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>

#include "game.h"
#include "food.h"

#include <fstream>
#include <string>
#include <strstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


using namespace std;
void unit(int,int);
int random(int,int);


extern GLuint texture[5];

bool length_inc = false;   //boolean value to see if snake can increase

extern int score;

extern bool lose;        //making the lose variable accessible from the main file

extern int FPS;

bool food = false;      //initialize it to false, as we draw food when food isn't present on the screen

int rows=0,columns=0;

int snakeDir = RIGHT;   //initializing default direction

int foodx,foody;       //variables to store position of food

int posx[MAX+1]={4,3};    //storing position of length of snake
int posy[MAX+1]={10,10};

int length=2;      //initial length of snake

void initGrid(int x,int y)  //initialize grid
{
    columns=x;
    rows=y;
}

void draw_grid()  //draws grind, with squares from values between columns and rows
{
    for(int i =0;i<columns;i++){
        for(int j=0;j<rows;j++){
        	unit(i,j);
    	}
	}
}

void draw_snake()       //function that draws the snake on screen
{
    for(int i = length-1; i>0; i--)    //when moving, the position of the previous snake block becomes the position of the current snake block
    {
        posx[i]=posx[i-1];
        posy[i]=posy[i-1];
    }
    for(int i=0;i<length;i++)  //loop to draw all snake elements
    {
    	glBindTexture(GL_TEXTURE_2D, texture[2]); //set snake body to green
        if(i==0)
        {
        	glBindTexture(GL_TEXTURE_2D, texture[1]);  //if it's the head, set snake to blue
            switch(snakeDir)
            {
            case UP:
                posy[i]++;
                break;
            case DOWN:
                posy[i]--;
                break;
            case RIGHT:
                posx[i]++;
                break;
            case LEFT:
                posx[i]--;
                break;
            }
            if(posx[i]==0||posx[i]==columns-1||posy[i]==0||posy[i]==rows-1){    //if the position is outside of the boundary, game over
            																	//blocks go from 0 - 39, so it's columns or rows - 1.
                lose=true;
            }
            else if(posx[0]== foodx && posy[0]==foody)                          //if the snake head touches the food, it turn food to false and adds to score
            {
                food=false;
                score++;
                if (score == 10){
                	FPS = 8;
                }
                if (score == 25){
					FPS = 8;
				}
                if(length<=MAX)
                    length_inc = true;           //boolean value to check if length can increase
                if(length==MAX) {                 //if snake length = max, display win and stop size increase
                	PlaySound(NULL, NULL, NULL);
                    MessageBox(NULL,"You Win: \nCONGRATULATIONS","GAME OVER!",0);
                    exit(0);
                }
            }


            for(int j=1;j<length;j++)
            {
                if(posx[j]==posx[0] && posy[j]==posy[0])           //if any of the parts of the snake touch the head, game over
                    lose=true;
            }
        }

        glBegin(GL_QUADS);
            glTexCoord2f(posx[i],posy[i]);
            glVertex2d(posx[i],posy[i]);

            glTexCoord2f(posx[i]+1,posy[i]);
            glVertex2d(posx[i]+1,posy[i]);

            glTexCoord2f(posx[i]+1,posy[i]+1);
            glVertex2d(posx[i]+1,posy[i]+1);

            glTexCoord2f(posx[i],posy[i]+1);
            glVertex2d(posx[i],posy[i]+1);
        glEnd();
    }
    if(length_inc)   //increase snake length
    {
        length++;
        length_inc=false;
    }
}

void draw_food()
{
    if(!food)
    {

        for(int i = 0; i <= length; i++) {
            foodx=random(1,columns-2);  //calls random for random x and y values
            foody=random(1,rows-2);

        	if(posx[i] == foodx && posy[i] == foody) {
        		food = false;
        		continue;
        	} else {
        		food = true;
        	}

        }
                //set food to true as it's on the screen
    }

    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    	glTexCoord2f(foodx, foody);
    	glVertex2d(foodx,foody);
    	glTexCoord2f(foodx+1,foody);
    	glVertex2d(foodx+1,foody);
    	glTexCoord2f(foodx+1,foody+1);
        glVertex2d(foodx+1,foody+1);
    	glTexCoord2f(foodx,foody+1); //draw the food texture
        glVertex2d(foodx,foody+1); //draw the food
    glEnd();
}

void unit(int x,int y)  //initializes unit of where square is drawn in grid
{
    glLoadIdentity();
    if(x==0||x==columns-1||y==0||y==rows-1)  //if in the barrier, turn red
    {
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glBegin(GL_QUADS);                  //creates each square in the grid
        	glTexCoord2f(x,y);
        	glVertex2d(x,y);
        	glTexCoord2f(x+1,y);
            glVertex2d(x+1,y);
            glTexCoord2f(x+1,y+1);
            glVertex2d(x+1,y+1);
            glTexCoord2f(x,y+1);
            glVertex2d(x,y+1);
        glEnd();
    }

}

int random(int _min,int _max)  //min and max for either x or y
{
	srand(time(NULL));   //seeding the random number every time to ensure the random value is not the same


    return _min + rand() % (_max - _min);  //algorithm to get random x and y values within range 1-38
}

