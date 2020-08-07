#ifndef WINDOWLOGIC_H
#define WINDOWLOGIC_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define WINDOW_FILTER_SIZE 3
#define R_CHANNEL 0
#define G_CHANNEL 1
#define B_CHANNEL 2
//functions
unsigned char *** createMatriz(ALLEGRO_BITMAP *image);
unsigned char *** allocateMemorySpaceForImage(const int * ptr_height, const  int * ptr_width);
void medianFilter(ALLEGRO_BITMAP *image);
int calculateMedian(unsigned char * arr, int length);

#endif