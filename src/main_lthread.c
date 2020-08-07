/*
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../include/alienLogic_lthread.h"
#include <lpthread.h>
#include <stdbool.h>
#include "linked_list.h"
#include "../include/bridge.h"
#include <allegro5/allegro_primitives.h>
//#include <json-c/json.h>

#define WIDTH 1080
#define HEIGHT 720
#define imgAlienNormal "imgAlien.png"
#define imgAlienBeta   "imgAlien1.png"
#define imgAlienAlfa   "imgAlien2.png"
#define imgCastle      "imgCastle.png"
#define imgMode        "imgmode.png"
#define imgsi_no       "si_no.png"
#define imgContinuar   "imgContinuar.png"

#define CASTLE0_X 30
#define CASTLE0_Y 30
#define CASTLE1_X 950
#define CASTLE1_Y 590
#define BTN0_X 30
#define BTN0_Y 150
#define BTN1_X 60
#define BTN1_Y 150
#define BTN2_X 90
#define BTN2_Y 150
#define BTN3_X 950
#define BTN3_Y 550
#define BTN4_X 980
#define BTN4_Y 550
#define BTN5_X 1010
#define BTN5_Y 550
#define BTNMODE_X 520
#define BTNMODE_Y 10
#define BTNSI_X 500
#define BTNSI_Y 350
#define BTNNO_X 560
#define BTNNO_Y 350
#define BTNCONTINUAR_X 510
#define BTNCONTINUAR_Y 320
const float FPS = 60;


lthread_mutex_t lock; 

struct List *listaAliens;
	//define bitmaps
	ALLEGRO_BITMAP *Image = NULL;
	ALLEGRO_BITMAP *Image2 = NULL;
	ALLEGRO_BITMAP *Image3 = NULL;
	ALLEGRO_BITMAP *ImageCastle = NULL;
	ALLEGRO_BITMAP *ImageMode = NULL;
	ALLEGRO_BITMAP *ImageSiNo = NULL;
	ALLEGRO_BITMAP *ImageContinuar = NULL;


void continuar(){
	al_clear_to_color(al_map_rgb(100, 100, 100));
	al_draw_bitmap(ImageContinuar,BTNCONTINUAR_X,BTNCONTINUAR_Y,0);
	al_draw_bitmap_region(ImageSiNo,0,0,20,20,BTNSI_X,BTNSI_Y,0);
	al_draw_bitmap_region(ImageSiNo,20,0,20,20,BTNNO_X,BTNNO_Y,0);	
}


bool reducirSegundo(){
	Alien *alien_to_timer;
	int cant = 0;
	while(cant < listaAliens->length){
		getAt(listaAliens,cant,(void *) &alien_to_timer);
		if(alien_to_timer->type == 2){
			if(alien_to_timer->max_exec_time != 0){
				alien_to_timer->max_exec_time -= 1;
			}else{
				alien_to_timer->max_exec_time = 10000;
				return 1;
			}
		}
		cant++;
	}
	return 0;

}



void* thread_alien(void *arg){
	Alien *alien = (Alien *) arg;
    initAlien(alien,&lock,listaAliens);
}


Alien* create_alien(int type){
	Alien *alien = (Alien *) malloc(sizeof(Alien));
	alien->type          = type % 3;
	int numero = rand() % 3; 
	if(type < 3){
		alien->route         = numero;
	}else if(type < 6){
		alien->route         = numero + 3;
	}
	lthread_t hilo;
	lthread_create(&hilo,NULL,&thread_alien,(void *) alien);
	addLast(listaAliens,&alien);

	return alien;
}

bool detectColition(float x1,float y1, int ancho1, int alto1, float x2, float y2,int ancho2,int alto2){
	return ((y1 <= y2 + alto2)&&(y2<= y1 +alto1)&&(x1 <= x2+ancho2)&&(x2<= x1 +ancho1));
}

void detectButtonPresed(int mouse_x,int mouse_y, int *game_mode){
	if(!(*game_mode)){
		if(detectColition(BTN0_X,BTN0_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(0);
		if(detectColition(BTN1_X,BTN1_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(1);
		if(detectColition(BTN2_X,BTN2_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(2);
		if(detectColition(BTN3_X,BTN3_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(3);
		if(detectColition(BTN4_X,BTN4_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(4);
		if(detectColition(BTN5_X,BTN5_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(5);
	}
	if(detectColition(BTNMODE_X,BTNMODE_Y,40,40,mouse_x,mouse_y,0,0))
		(*game_mode) = ((* game_mode) + 1) % 2;
}

void detectContinuar(int mouse_x,int mouse_y,bool *running, bool *rtos_fin){
	if(detectColition(BTNSI_X,BTNSI_Y,20,20,mouse_x,mouse_y,0,0))   
		(* rtos_fin) = 0;
	if(detectColition(BTNNO_X,BTNNO_Y,20,20,mouse_x,mouse_y,0,0))
		(* running) =  0;
}
void eliminarAlien(int mouse_x,int mouse_y){
	int cantidad = 0;
	Alien *alien_to_delete;
	while(cantidad < listaAliens->length){
		getAt(listaAliens,cantidad,(void *) &alien_to_delete);
		if(detectColition(alien_to_delete->pos_x,alien_to_delete->pos_y, 20, 20, mouse_x, mouse_y,0,0)){
			alien_to_delete->stage = MAXSTAGESIZE;
			removeAt(listaAliens,cantidad);
		}
		cantidad ++;
	}
	//removeAt
}

void colocarMapa(){
	//ruta 1,2,3 común
	al_draw_filled_rectangle(130, 50, 570, 70,   al_map_rgb(20, 20, 20));
	al_draw_filled_rectangle(570, 50, 590, 220,  al_map_rgb(20, 20, 20));

	al_draw_filled_rectangle(500, 520, 520, 650, al_map_rgb(20, 20, 20));
	al_draw_filled_rectangle(500, 650, 950, 670, al_map_rgb(20, 20, 20));

	//ruta 0
	al_draw_filled_rectangle(570, 200, 850, 220, al_map_rgb(20, 20, 20));
	al_draw_filled_rectangle(850, 200, 870, 290, al_map_rgb(20, 20, 20));

	al_draw_filled_rectangle(500, 520, 870, 540, al_map_rgb(20, 20, 20));
	al_draw_filled_rectangle(850, 520, 870, 480, al_map_rgb(20, 20, 20)); //480

	//ruta 1
	al_draw_filled_rectangle(570, 200, 590, 290, al_map_rgb(20, 20, 20));

	al_draw_filled_rectangle(570, 480, 590, 520, al_map_rgb(20, 20, 20));

	//ruta 2
	al_draw_filled_rectangle(600, 200, 230, 220, al_map_rgb(20, 20, 20));
	al_draw_filled_rectangle(230, 200, 250, 290, al_map_rgb(20, 20, 20));

	al_draw_filled_rectangle(230, 480, 250, 520, al_map_rgb(20, 20, 20));
	al_draw_filled_rectangle(230, 520, 500, 540, al_map_rgb(20, 20, 20));


	//ruta 3,4,5
	al_draw_filled_rectangle(950, 600, 550, 620, al_map_rgb(220, 220, 220));
	al_draw_filled_rectangle(550, 620, 570, 540, al_map_rgb(220, 220, 220));

	al_draw_filled_rectangle(130, 100, 550, 120 , al_map_rgb(220, 220, 220));
	al_draw_filled_rectangle(550, 100, 570, 240, al_map_rgb(220, 220, 220));

	//ruta 3
	al_draw_filled_rectangle(550, 540, 850, 560, al_map_rgb(220, 220, 220));
	al_draw_filled_rectangle(830, 540, 850, 480, al_map_rgb(220, 220, 220));

	al_draw_filled_rectangle(830, 220, 850, 290, al_map_rgb(220, 220, 220));
	al_draw_filled_rectangle(830, 220, 550, 240, al_map_rgb(220, 220, 220));

	//ruta 4
	al_draw_filled_rectangle(550, 540, 570, 480, al_map_rgb(220, 220, 220));

	al_draw_filled_rectangle(550, 220, 570, 290, al_map_rgb(220, 220, 220));	

	//ruta 5
	al_draw_filled_rectangle(550, 540, 210, 560, al_map_rgb(220, 220, 220));
	al_draw_filled_rectangle(210, 540, 230, 480, al_map_rgb(220, 220, 220));

	al_draw_filled_rectangle(210, 290, 230, 220, al_map_rgb(220, 220, 220));
	al_draw_filled_rectangle(210, 220, 550, 240, al_map_rgb(220, 220, 220));

	//puente con  colorcitos diferentes
	al_draw_filled_rectangle(210, 290, 250, 480, al_map_rgb(250, 133, 66));
	al_draw_filled_rectangle(550, 290, 590, 480, al_map_rgb(250, 133, 66));
	al_draw_filled_rectangle(830, 290, 870, 480, al_map_rgb(250, 133, 66));


}

int main(int argc, char *argv[])
{

	struct Bridge east_bridge, center_bridge, west_bridge;

    //
	int movimiento      = 0;
	int contador        = 0;
	int mouse_timer     = 0;
	int cant_alien      = 0;
	int game_mode       = 0;
	bool rtos_fin       = 0;
	
	initBridge(&east_bridge,EAST_BRIDGE_CONFIG_FILENAME);
	initBridge(&center_bridge,CENTRAL_BRIDGE_CONFIG_FILENAME);
	initBridge(&west_bridge,WEST_BRIDGE_CONFIG_FILENAME);

	//create new alien
	listaAliens = createList(sizeof(Alien *));
	Alien * alien_to_show;
	

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_MOUSE_STATE state;


	bool running = true;
	bool redraw = true;

	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}
	//init the mouse driver
	al_install_mouse();

	//Init image reader
	al_init_image_addon();
	al_init_primitives_addon();

	//Read the bitmap from the image .png
	Image          = al_load_bitmap(imgAlienNormal);
	Image2         = al_load_bitmap(imgAlienBeta);
	Image3         = al_load_bitmap(imgAlienAlfa);
	ImageCastle    = al_load_bitmap(imgCastle);
	ImageMode      = al_load_bitmap(imgMode);
	ImageSiNo      = al_load_bitmap(imgsi_no);
	ImageContinuar =al_load_bitmap(imgContinuar);
	//if image is null finish the program 
	assert(Image != NULL);

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	display = al_create_display(WIDTH, HEIGHT);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}
	//configure the window
	al_set_window_title(display,"Alien Community");

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	float x = 0;
	// Game loop *********************************************************************************************
	while (running) { 
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;
		ALLEGRO_BITMAP *bitmap;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event) {
			switch (event.type) {
				case ALLEGRO_EVENT_TIMER:
					redraw = true;
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				default:
					fprintf(stderr, "Unsupported event received: %d\n", event.type);
					break;
			}
		}

		if(mouse_timer == 0){
			al_get_mouse_state(&state);
			if (state.buttons & 1) {
				if(rtos_fin){
					detectContinuar(state.x,state.y,&running, &rtos_fin);
				}else{
				    detectButtonPresed(state.x,state.y,&game_mode);
				    mouse_timer = 30;
				    lthread_mutex_trylock(&lock);
				    eliminarAlien(state.x,state.y);
				    lthread_mutex_unlock(&lock);
					}
			}
		}
		

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			if(rtos_fin){
				continuar();
			}else{
			// Redraw
			if(contador == 60){
				movimiento = (movimiento+1)%4;
				contador = 0;
				lthread_mutex_trylock(&lock);
				rtos_fin = reducirSegundo();
				lthread_mutex_unlock(&lock);
			}
			contador += 1;
			
			al_clear_to_color(al_map_rgb(50, 50, 50));
			colocarMapa();
			
			if(game_mode == 0)
				al_draw_bitmap_region(ImageMode,0,40,40,40,BTNMODE_X,BTNMODE_Y,0);
			else
				al_draw_bitmap_region(ImageMode,0,0,40,40,BTNMODE_X,BTNMODE_Y,0);
			

			cant_alien = 0;
			lthread_mutex_trylock(&lock);
			while(cant_alien < listaAliens->length){
				getAt(listaAliens,cant_alien,(void *) &alien_to_show);
				if(alien_to_show->type == 0){
					al_draw_bitmap_region(Image,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);
				}
				if(alien_to_show->type == 1){
					al_draw_bitmap_region(Image2,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);
				}
				if(alien_to_show->type == 2){
					al_draw_bitmap_region(Image3,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);
				}

				if(alien_to_show->stage != 5){
					alien_to_show->cond = 1;
				}else{
					///add to the queue
					if(!alien_to_show->queue){
						alien_to_show->queue = 1;
						if(alien_to_show->route == 0){
							insertAlienInNorth(&east_bridge,alien_to_show);
						}
						if(alien_to_show->route == 1){
							insertAlienInNorth(&center_bridge,alien_to_show);
						}
						if(alien_to_show->route == 2){
							insertAlienInNorth(&west_bridge,alien_to_show);
						}
						if(alien_to_show->route == 3){
							insertAlienInSouth(&east_bridge,alien_to_show);

						}
						if(alien_to_show->route == 4){
							insertAlienInSouth(&center_bridge,alien_to_show);
						}
						if(alien_to_show->route == 5){
							insertAlienInSouth(&west_bridge,alien_to_show);
						}
					}
					if(alien_to_show->isSelected){
						alien_to_show->cond = 1;
					}
				
				}
				cant_alien++;
			}
			lthread_mutex_unlock(&lock);
			//Draw buttoms
			al_draw_bitmap_region(Image,60,40,20,20,BTN0_X, BTN0_Y, 0);
			al_draw_bitmap_region(Image2,60,40,20,20,BTN1_X, BTN1_Y, 0);
			al_draw_bitmap_region(Image3,60,40,20,20,BTN2_X, BTN2_Y, 0);

			al_draw_bitmap_region(Image,60,40,20,20,BTN3_X, BTN3_Y, 0);
			al_draw_bitmap_region(Image2,60,40,20,20,BTN4_X, BTN4_Y, 0);
			al_draw_bitmap_region(Image3,60,40,20,20,BTN5_X, BTN5_Y, 0);

			//draw castle
			al_draw_bitmap(ImageCastle, CASTLE0_X, CASTLE0_Y, 0);
			al_draw_bitmap(ImageCastle, CASTLE1_X, CASTLE1_Y, ALLEGRO_FLIP_HORIZONTAL);
			}
			
			if(mouse_timer != 0){
				mouse_timer --;
			}
			
			al_flip_display();
			redraw = false;
		}

		//Manage the traffic on bridges
		manageTraffic(&east_bridge);
		manageTraffic(&center_bridge);
		manageTraffic(&west_bridge);
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(Image);
	al_destroy_bitmap(Image2);
	al_destroy_bitmap(Image3);
	al_destroy_bitmap(ImageCastle);
	al_destroy_bitmap(ImageMode);
	al_destroy_bitmap(ImageSiNo);
	al_destroy_bitmap(ImageContinuar);

	return 0;
}