/*
 * food.cpp
 *
 *  Created on: Nov 29, 2022
 *      Author: ellig

#include <GL/glut.h>
#include <iostream>
#include <ctime>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "food.h"
#include "game.h"
#include "game.cpp"




int random(int _min,int _max)  //min and max for either x or y
{
	srand(time(NULL));   //seeding the random number every time to ensure the random value is not the same


    return _min + rand() % (_max - _min);  //algorithm to get random x and y values within range 1-38
}

void init_food(int foodx, int foody, bool fod){
    if(!fod)
    {
        foodx=random(1,columns-2);  //calls random for random x and y values
        foody=random(1,rows-2);
        fod = true;                //set food to true as it's on the screen
    }
	fod = false;
}

void draw_food(int foodx, int foody, bool fod)
{
    glColor3f(1.0,0.0,1.0);
    glBegin(GL_QUADS);
        glVertex2d(foodx,foody); glVertex2d(foodx+1,foody); glVertex2d(foodx+1,foody+1); glVertex2d(foodx,foody+1); //draw the food
    glEnd();
}
*/
