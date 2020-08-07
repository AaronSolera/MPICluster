/*
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#include "../include/windowLogic.h"

#define WIDTH 1080
#define HEIGHT 720

#define imgCastle      "images/imgmode.png"
#define CASTLE0_X 30
#define CASTLE0_Y 30
#define BUFFERSIZE 128

const float FPS = 60;

//define bitmaps
ALLEGRO_BITMAP * ImageCastle = NULL;
ALLEGRO_BITMAP * buffer = NULL;

int main(int argc, char *argv[])
{
    //
	int mouse_timer     = 0;

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

	//Read path()
	//al_install_keyboard();

	//Read the bitmap from the image .png
	ImageCastle    = al_load_bitmap(imgCastle);
	

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
				mouse_timer = 30;
				medianFilter(ImageCastle);
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(150, 120, 170));


			al_draw_bitmap(ImageCastle, CASTLE0_X, CASTLE0_Y, 0);
			
			

			if(mouse_timer != 0){
				mouse_timer --;
			}
			
			al_flip_display();
			redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(ImageCastle);
	al_destroy_bitmap(buffer);
	

	return 0;
}