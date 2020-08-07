#include "../include/alienLogic_lthread.h"

//this 
bool rectAlienCol(float x1,float y1, float x2, float y2){
    if((y1 <= y2 + 19)&&(y2 <= y1 +19)&&(x1 <= x2+19)&&(x2<= x1 +19)){
        return 1;
    }
    return 0;
}

bool validateAlienCollision(float x1,float y1,Alien *alien, struct List *listaAliens){
    int p = 0;
    Alien *alien_to_validate;
    while(p < listaAliens->length){

        getAt(listaAliens,p,(void *) &alien_to_validate);
         if(alien != alien_to_validate){
            if(rectAlienCol(x1,y1,alien_to_validate->pos_x,alien_to_validate->pos_y)){
                return 0;
            }
         }
        
        p++;
    }
    return 1;
}


void loadRoute(Alien *alien){
    if(alien->route == 0){
        alien->route_x[0]    = 130;
        alien->route_x[1]    = 570;
        alien->route_x[2]    = 570;
        alien->route_x[3]    = 850;
        alien->route_x[4]    = 850;
        alien->route_x[5]    = 850;
        alien->route_x[6]    = 850;
        alien->route_x[7]    = 500;
        alien->route_x[8]    = 500;
        alien->route_x[9]    = 930;

        alien->route_y[0]    = 50;
        alien->route_y[1]    = 50;
        alien->route_y[2]    = 200;
        alien->route_y[3]    = 200;
        alien->route_y[4]    = 270;//depende
        alien->route_y[5]    = 480;//depende
        alien->route_y[6]    = 520;
        alien->route_y[7]    = 520;
        alien->route_y[8]    = 650;
        alien->route_y[9]    = 650;
    }
    if(alien->route == 1){
        alien->route_x[0]    = 130;
        alien->route_x[1]    = 570;
        alien->route_x[2]    = 570;
        alien->route_x[3]    = 570;
        alien->route_x[4]    = 570;
        alien->route_x[5]    = 570;
        alien->route_x[6]    = 570;
        alien->route_x[7]    = 500;
        alien->route_x[8]    = 500;
        alien->route_x[9]    = 930;

        alien->route_y[0]    = 50;
        alien->route_y[1]    = 50;
        alien->route_y[2]    = 200;
        alien->route_y[3]    = 240;
        alien->route_y[4]    = 270;//
        alien->route_y[5]    = 480;//
        alien->route_y[6]    = 520;
        alien->route_y[7]    = 520;
        alien->route_y[8]    = 650;
        alien->route_y[9]    = 650;
    }
    if(alien->route == 2){
        alien->route_x[0]    = 130;
        alien->route_x[1]    = 570;
        alien->route_x[2]    = 570;
        alien->route_x[3]    = 230;
        alien->route_x[4]    = 230;
        alien->route_x[5]    = 230;
        alien->route_x[6]    = 230;
        alien->route_x[7]    = 500;
        alien->route_x[8]    = 500;
        alien->route_x[9]    = 930;

        alien->route_y[0]    = 50;
        alien->route_y[1]    = 50;
        alien->route_y[2]    = 200;
        alien->route_y[3]    = 200;
        alien->route_y[4]    = 270;//
        alien->route_y[5]    = 480;//
        alien->route_y[6]    = 520;
        alien->route_y[7]    = 520;
        alien->route_y[8]    = 650;
        alien->route_y[9]    = 650;
    }
    if(alien->route == 3){
        alien->route_x[0]    = 930;
        alien->route_x[1]    = 550;
        alien->route_x[2]    = 550;
        alien->route_x[3]    = 830;
        alien->route_x[4]    = 830;
        alien->route_x[5]    = 830;
        alien->route_x[6]    = 830;
        alien->route_x[7]    = 550;
        alien->route_x[8]    = 550;
        alien->route_x[9]    = 130;

        alien->route_y[0]    = 600;
        alien->route_y[1]    = 600;
        alien->route_y[2]    = 540;
        alien->route_y[3]    = 540;
        alien->route_y[4]    = 480;
        alien->route_y[5]    = 270;
        alien->route_y[6]    = 220;
        alien->route_y[7]    = 220;
        alien->route_y[8]    = 100;
        alien->route_y[9]    = 100;
    }
    if(alien->route == 4){
        alien->route_x[0]    = 930;
        alien->route_x[1]    = 550;
        alien->route_x[2]    = 550;
        alien->route_x[3]    = 550;
        alien->route_x[4]    = 550;
        alien->route_x[5]    = 550;
        alien->route_x[6]    = 550;
        alien->route_x[7]    = 550;
        alien->route_x[8]    = 550;
        alien->route_x[9]    = 130;

        alien->route_y[0]    = 600;
        alien->route_y[1]    = 600;
        alien->route_y[2]    = 540;
        alien->route_y[3]    = 500;
        alien->route_y[4]    = 480;
        alien->route_y[5]    = 270;
        alien->route_y[6]    = 220;
        alien->route_y[7]    = 220;
        alien->route_y[8]    = 100;
        alien->route_y[9]    = 100;
    }
    if(alien->route == 5){
        alien->route_x[0]    = 930;
        alien->route_x[1]    = 550;
        alien->route_x[2]    = 550;
        alien->route_x[3]    = 210;
        alien->route_x[4]    = 210;
        alien->route_x[5]    = 210;
        alien->route_x[6]    = 210;
        alien->route_x[7]    = 550;
        alien->route_x[8]    = 550;
        alien->route_x[9]    = 130;

        alien->route_y[0]    = 600;
        alien->route_y[1]    = 600;
        alien->route_y[2]    = 540;
        alien->route_y[3]    = 540;
        alien->route_y[4]    = 480;
        alien->route_y[5]    = 270;
        alien->route_y[6]    = 220;
        alien->route_y[7]    = 220;
        alien->route_y[8]    = 100;
        alien->route_y[9]    = 100;
    }
}

bool getNewPos(Alien *alien){
    alien->stage += 1;
    if(alien->stage == MAXSTAGESIZE){
        return 0;
    }
    alien->find_x = alien->route_x[alien->stage];
    alien->find_y = alien->route_y[alien->stage];
    return 1;
}

void liberarMemoria(Alien *alien,struct List *listaAliens){
    printf("Entra a la func\n");
    int cantidad = 0;
    Alien *alien_to_remove;
    while(cantidad < listaAliens->length){
        getAt(listaAliens,cantidad,(void *) &alien_to_remove);
        
        printf("eliminamos alien de la lista...\n");
        if(alien_to_remove == alien){
            removeAt(listaAliens,cantidad);
            printf("eliminamos alien de la lista...\n");
        }
        cantidad ++;
    }
}

void AlienWhile(Alien *alien,lthread_mutex_t *lock,struct List *listaAliens){
    int x = 0;
    int new_pos = 0;

    bool moveAlien = 1;
    
    while(moveAlien){
        lthread_mutex_trylock(lock);
        if(alien->stage == MAXSTAGESIZE){
            moveAlien = 0;
        }
        //lthread_mutex_trylock(lthread_mutex_t * mutex);
        if(alien->cond==1){
            if(fabs(alien->pos_x -alien->find_x)<alien->speed){
                if(validateAlienCollision(alien->find_x,alien->pos_y,alien,listaAliens)){
                    alien->pos_x = alien->find_x;
                    new_pos += 1;
                }
                
            }else if(alien->pos_x < alien->find_x){
                if(validateAlienCollision(alien->pos_x + alien->speed,alien->pos_y,alien,listaAliens)){
                    alien->pos_x = alien->pos_x + alien->speed;
                }
                
            }else{
                if(validateAlienCollision(alien->pos_x - alien->speed,alien->pos_y,alien,listaAliens)){
                    alien->pos_x = alien->pos_x - alien->speed;
                }
            }
            if(fabs(alien->pos_y -alien->find_y)<alien->speed){
                if(validateAlienCollision(alien->pos_x,alien->find_y,alien,listaAliens)){
                alien->pos_y = alien->find_y;
                new_pos += 1;
                }
            }else if(alien->pos_y < alien->find_y){
                if(validateAlienCollision(alien->pos_x,alien->pos_y + alien->speed,alien,listaAliens)){
                    alien->pos_y = alien->pos_y + alien->speed;
                }
            }else{
                if(validateAlienCollision(alien->pos_x,alien->pos_y - alien->speed,alien,listaAliens)){
                    alien->pos_y = alien->pos_y - alien->speed;
                }
            }
            if(new_pos ==2){
                moveAlien = getNewPos(alien);
            }
            x++;
            new_pos = 0;
            alien->cond = 0;
        }
        lthread_mutex_unlock(lock);
        sleep(0.5);
    }
    liberarMemoria(alien,listaAliens);
    free(alien);
    printf("Hilo eliminado.....");
    
}


void initAlien(Alien *alien,lthread_mutex_t *lock,struct List *listaAliens){
    readAlienConfig(alien);
    alien->dir           = 1;
    alien->priority      = 3;
    alien->weight        = 3;
    alien->cond          = 1;
    alien->stage         = 0;
    alien->queue         = 0;
    alien->isSelected    = 0;
    loadRoute(alien);
    alien->pos_x         = alien->route_x[0];
    alien->pos_y         = alien->route_y[0];
    alien->find_x        = alien->route_x[1];
    alien->find_y        = alien->route_y[1];
    AlienWhile(alien, lock,listaAliens);
    
}

void readAlienConfig(Alien *alien){
    double a_speed;
    double a_time;
    FILE *fp;
    char buffer[1024];

    struct json_object *parsed_json;

    struct json_object *speed;
    struct json_object *max_exec_time;

    fp =fopen(ALIEN_CONFIG_PAHT,"r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);
    
    json_object_object_get_ex(parsed_json, "speed", &speed);
    json_object_object_get_ex(parsed_json, "max_exec_time", &max_exec_time);

    a_speed = json_object_get_double(speed);
    a_time = json_object_get_double(max_exec_time);

    if(alien->type == 0)
        alien->speed = a_speed;
    if(alien->type == 1)
        alien->speed = a_speed*1.2;
    if(alien->type == 2)
        alien->speed = a_speed * (1 +(rand() % 150 + 50) / 100);
    alien->max_exec_time = a_time;
}

void moveAlien(Alien *alien){
    if(alien->dir==1){
        alien->pos_x++;
    }
    printf("%f",alien->pos_x);
}


void* newAlien(void *arg){
    /*char* alien_type = (char *) arg;
    
    int alien_speed = 0;
    double alien_max_excec_time = 0;
    readAlienConfig(&alien_speed,&alien_max_excec_time);
    Alien alien;
    if(alien_type == "n"){
        Alien alien2 = {alien_speed,1,100,100,1,10,5,10.12,1};//speed,posx,posy,dir,priority,weight,max_exec_time,route
        alien = alien2;
        printf("I'm a normal alien \n");
    }
    if(alien_type == "a"){
        Alien alien2 = {alien_speed*1.20,1,200,200,1,10,5,10.12,1};//speed,posx,posy,dir,priority,weight,max_exec_time,route
        alien = alien2;
        printf("I'm a alpha alien \n");
    }
    if(alien_type == "b"){
        Alien alien2 = {alien_speed,1,300,300,1,10,5,alien_max_excec_time,1};//speed,posx,posy,dir,priority,weight,max_exec_time,route
        alien = alien2;
        printf("I'm a beta alien \n");
    }


    int x = 0;
    while(x<10){
        printf("thread active\n");
        moveAlien(&alien);
        x++;
    }
    return NULL;*/
};
