#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#include <mpi.h>
#include "../include/windowLogic.h"

#define WIDTH 1080
#define HEIGHT 720

#define imgCastle      "images/imgmode.png"
#define CASTLE0_X 30
#define CASTLE0_Y 30
#define BUFFERSIZE 128

const float FPS = 60;

//define bitmaps
ALLEGRO_BITMAP *image;

int main(int argc, char *argv[])
{
	int rank, size, name_len, ptr_width, ptr_height, total_ranks = 2;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &name_len);

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
	image = al_load_bitmap(imgCastle);

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

	ptr_height = al_get_bitmap_height(image);
    ptr_width  = al_get_bitmap_width(image);

	ALLEGRO_BITMAP *sub_bitmap;

	if(rank == 0){
        sub_bitmap = al_create_sub_bitmap(image, 0, 0, *ptr_width, *ptr_height/total_ranks + 1);
    }else if(rank == total_ranks){
        sub_bitmap = al_create_sub_bitmap(image, 0, (*ptr_height/total_ranks) * rank - 1, *ptr_width, *ptr_height/total_ranks + 1);
    }else{
        sub_bitmap = al_create_sub_bitmap(image, 0, (*ptr_height/total_ranks) * rank - 1, *ptr_width, *ptr_height/total_ranks + 2);
    }

	medianFilter(sub_bitmap);

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

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(150, 120, 170));


			al_draw_bitmap(image, CASTLE0_X, CASTLE0_Y, 0);

			
			al_flip_display();
			redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(image);
	
	MPI_Finalize();

	return 0;
}