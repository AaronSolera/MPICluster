#ifndef ALIENLOGIC_H
#define ALIENLOGIC_H

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<json-c/json.h>
#include<math.h>
#include<stdbool.h>
#include"linked_list.h"
//include "bridge.h"


#define ALIEN_CONFIG_PAHT  "./files/Alientxt.json"
#define ALIEN_AUTO_CREATE_PAHT  "./files/AlienCreate.json"
#define MAXSTAGESIZE  10
typedef struct Alien{
    float speed; 
    int type;
    float pos_x;
    float pos_y;
    int dir;
    int priority;
    int weight;
    double max_exec_time;
    int route;
    int cond;
    int stage;
    int route_x[MAXSTAGESIZE];  
	int route_y[MAXSTAGESIZE];
	double find_x;
    double find_y;
    int queue;
    int isSelected;

} Alien;
void liberarMemoria(Alien *alien,struct List *listaAliens);
bool rectAlienCol(float x1,float y1, float x2, float y2);
bool validateAlienCollision(float x1,float y1, Alien *alien, struct List *listaAliens);
void loadRoute(Alien *alien);
bool getNewPos(Alien *alien);
void AlienWhile(Alien *alien,pthread_mutex_t *lock,struct List *listaAliens);
void initAlien(Alien *alien,pthread_mutex_t  *lock,struct List *listaAliens);
void moveAlien(Alien *alien);
void * newAlien (void *arg);
void readAlienConfig(Alien *alien);


#endif