#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define MAX 30         //max size of snake

#define UP 1           //Contants that define the direction of the snake
#define DOWN 2
#define RIGHT 3
#define LEFT 4

void initGrid(int,int);
void draw_grid();
void draw_food();
void draw_snake();

#endif // GAME_H_INCLUDED
