/* #include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "snake.h"
#include "game.h"
#include "game.cpp"
#include "food.h"
#include "food.cpp"


int snakeDir = RIGHT;   //initializing default direction
bool length_inc = false;   //boolean value to see if snake can increase
int posx[MAX+1]={4,3};    //storing position of length of snake
int posy[MAX+1]={10,10};
int length=2;      //initial length of snake

void init_snake(int posx, int posy, int length, bool length_inc, int snakeDir){
	 snakeDir = RIGHT;   //initializing default direction
	 length_inc = false;   //boolean value to see if snake can increase
	 posx[MAX+1]={4,3};    //storing position of length of snake
	 posy[MAX+1]={10,10};
	 length=2;      //initial length of snake
}
void draw_snake(int posx, int posy, int length, bool length_inc, int snakeDir)       //function that draws the snake on screen
{
    for(int i = length-1; i>0; i--)    //when moving, the position of the previous snake block becomes the position of the current snake block
    {
        posx[i]=posx[i-1];
        posy[i]=posy[i-1];
    }
    for(int i=0;i<length;i++)  //loop to draw all snake elements
    {
        glColor3f(0.0,1.0,0.0); //set snake body to green
        if(i==0)
        {
            glColor3f(0.0,0.0,1.0);  //if it's the head, set snake to blue
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
            else if(posx[i]==foodx && posy[i]==foody)                          //if the snake head touches the food, it turn food to false and adds to score
            {
                food=false;
                score++;
                if(length<=MAX)
                    length_inc = true;           //boolean value to check if length can increase
                if(length==MAX)                  //if snake length = max, display win and stop size increase
                    MessageBox(NULL,"You Win\nYou can still keep playing but the snake will not grow.","Awesome",0);
            }
            for(int j=1;j<length;j++)
            {
                if(posx[j]==posx[0] && posy[j]==posy[0])           //if any of the parts of the snake touch the head, game over
                    lose=true;
            }
        }
        glBegin(GL_LINE_LOOP);
            glVertex2d(posx[i],posy[i]); glVertex2d(posx[i]+1,posy[i]); glVertex2d(posx[i]+1,posy[i]+1); glVertex2d(posx[i],posy[i]+1);
        glEnd();
    }
    if(length_inc)   //increase snake length
    {
        length++;
        length_inc=false;
    }
}
*/
