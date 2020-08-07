#include "../include/bridge.h"

void initBridge(struct Bridge *bridge, const char *filename)
{
	FILE *fp;
	char buffer[1024];

	struct json_object *parsed_json;

	struct json_object *length;
	struct json_object *capacity;
	struct json_object *transit_control_algorithm;
	struct json_object *scheduling_algorithm;
	struct json_object *ordered_aliens_per_side;
	struct json_object *y;
	struct json_object *m;
	struct json_object *n;

	fp = fopen(filename, "r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "length", &length);
	json_object_object_get_ex(parsed_json, "capacity", &capacity);
	json_object_object_get_ex(parsed_json, "transit_control_algorithm", &transit_control_algorithm);
	json_object_object_get_ex(parsed_json, "scheduling_algorithm", &scheduling_algorithm);
	json_object_object_get_ex(parsed_json, "ordered_aliens_per_side", &ordered_aliens_per_side);
	json_object_object_get_ex(parsed_json, "Y", &y);
	json_object_object_get_ex(parsed_json, "M", &m);
	json_object_object_get_ex(parsed_json, "N", &n);

	bridge->length 						= json_object_get_int(length);
	bridge->capacity 					= json_object_get_int(capacity);
	bridge->transit_control_algorithm	= json_object_get_int(transit_control_algorithm);
	bridge->scheduling_algorithm 		= json_object_get_int(scheduling_algorithm);
	bridge->ordered_aliens_per_side 	= json_object_get_int(ordered_aliens_per_side);
	bridge->Y_aliens 					= json_object_get_int(y);
	bridge->M_sec 						= json_object_get_int(m);
	bridge->N_sec 						= json_object_get_int(n);

	bridge->north_aliens_queue		= createList(sizeof(Alien *));
	bridge->south_aliens_queue		= createList(sizeof(Alien *));
	bridge->aliens_crossing_list	= createList(sizeof(Alien *));

	bridge->open_side	= NORTH;
	bridge->Y_count 	= 0;

	if (bridge->transit_control_algorithm == SEMAPHORES)
		clock_gettime(CLOCK_REALTIME, &bridge->start_time);
}


int priority(const void *a, const void *b)
{
	int alien1_priority = ((Alien *)a)->priority;
	int alien2_priority = ((Alien *)b)->priority;

	return alien1_priority > alien2_priority;
}


int shortestJobFirst(const void *a, const void *b, const void *c)
{
	int bridge_length = ((struct Bridge *)c)->length;
	int alien1_speed = ((Alien *)a)->speed;
	int alien2_speed = ((Alien *)b)->speed;

	return !(bridge_length / alien1_speed > bridge_length / alien2_speed);
}


void sortBridgeSide(struct Bridge *bridge, struct List **queue)
{
	switch (bridge->scheduling_algorithm)
	{
		case PRIORITY:
			bubbleSort2(*queue, bridge->ordered_aliens_per_side, priority);
			break;

		case SJF:
			bubbleSort3(*queue, bridge->ordered_aliens_per_side, shortestJobFirst, (const void *)bridge);
			break;

		default:
			break;
	}
}


void insertAlienInNorth(struct Bridge *bridge, Alien *alien)
{
	alien->pos_x += 30;

	addLast(bridge->north_aliens_queue, &alien);

	if ((bridge->scheduling_algorithm == PRIORITY || bridge->scheduling_algorithm == SJF) && 
		 bridge->north_aliens_queue->length <= bridge->ordered_aliens_per_side)
		sortBridgeSide(bridge, &bridge->north_aliens_queue);
}


void insertAlienInSouth(struct Bridge *bridge, Alien *alien)
{
	alien->pos_x -= 30;

	addLast(bridge->south_aliens_queue, &alien);

	if ((bridge->scheduling_algorithm == PRIORITY || bridge->scheduling_algorithm == SJF) && 
		bridge->south_aliens_queue->length <= bridge->ordered_aliens_per_side)
		sortBridgeSide(bridge, &bridge->south_aliens_queue);
}


void getAlienFromQueue(struct List **queue, struct List **aliens_crossing_list, int scheduling_algorithm, int ordered_aliens_per_side)
{
	if (!isListEmpty(*queue))
	{
		Alien *alien_tmp;

		if (scheduling_algorithm == ROUND_ROBIN || scheduling_algorithm == PRIORITY || 
			scheduling_algorithm == SJF         || scheduling_algorithm == FIFO)
		{
			getFirst(*queue, (void *)&alien_tmp);
			removeFirst(*queue);
		}

		else if (scheduling_algorithm == LOTTERY)
		{
			int index;

			if ((*queue)->length > ordered_aliens_per_side)
				index = rand() % ordered_aliens_per_side;

			else
				index = rand() % (*queue)->length;

			getAt(*queue, index, (void *)&alien_tmp);
			removeAt(*queue, index);
		}

		if (alien_tmp->route < 3)
			alien_tmp->pos_y += 20; 

		else if (alien_tmp->route < 6)
			alien_tmp->pos_y -= 20;

		addLast(*aliens_crossing_list, &alien_tmp);

		alien_tmp->isSelected = 1;
	}
}


void letAlienPass(struct Bridge *bridge, int side)
{
	switch (side)
	{
		case NORTH:
			getAlienFromQueue(&bridge->north_aliens_queue, &bridge->aliens_crossing_list, bridge->scheduling_algorithm, bridge->ordered_aliens_per_side);
			break;

		case SOUTH:
			getAlienFromQueue(&bridge->south_aliens_queue, &bridge->aliens_crossing_list, bridge->scheduling_algorithm, bridge->ordered_aliens_per_side);
			break;

		default:
			break;
	}
}


void YAlgorithm(struct Bridge *bridge)
{
	//If there are aliens waiting to cross the bridge
	if (!isListEmpty(bridge->north_aliens_queue) || !isListEmpty(bridge->south_aliens_queue))
	{
		//If there are no aliens crossing
		if (isListEmpty(bridge->aliens_crossing_list))
		{
			//If there are no aliens waiting in the north
			if (isListEmpty(bridge->north_aliens_queue))
				bridge->open_side = SOUTH;

			//If there are no aliens waiting in the south
			else if (isListEmpty(bridge->south_aliens_queue))
				bridge->open_side = NORTH;

			//There are aliens waiting on both sides
			else 
			{
				//If the number of aliens that have passed is zero and on both sides the number of aliens waiting is less than Y
				if (bridge->Y_count == 0 && 
					bridge->north_aliens_queue->length < bridge->Y_aliens && bridge->south_aliens_queue->length < bridge->Y_aliens)
						//Select the side where there are more aliens waiting
						bridge->open_side = (bridge->north_aliens_queue->length >= bridge->south_aliens_queue->length) ? NORTH : SOUTH;

				else if (bridge->Y_count == bridge->Y_aliens)
				{
					bridge->open_side = !bridge->open_side;
					bridge->Y_count = 0;
				}

			}
		}

		//If another alien can still pass and the bridge supports it
		if ((bridge->Y_count < bridge->Y_aliens) && (bridge->aliens_crossing_list->length < bridge->capacity))
		{
			letAlienPass(bridge, bridge->open_side);

			//If somewhere there are no aliens waiting
			if (isListEmpty(bridge->north_aliens_queue) || isListEmpty(bridge->south_aliens_queue))
				bridge->Y_count = 0;

			else
				bridge->Y_count++;
		}
	}
}


void semaphoresAlgorithm(struct Bridge *bridge)
{
	clock_gettime(CLOCK_REALTIME, &(bridge->current_time));

	//If there are no aliens crossing
	if (isListEmpty(bridge->aliens_crossing_list))
	{
		//If time's up on the north side
		if (bridge->open_side == NORTH && (bridge->current_time.tv_sec - bridge->start_time.tv_sec) >= bridge->M_sec)
		{
			bridge->open_side = SOUTH;
			clock_gettime(CLOCK_REALTIME, &bridge->start_time);
		}

		//If time's up on the south side
		else if (bridge->open_side == SOUTH && (bridge->current_time.tv_sec - bridge->start_time.tv_sec) >= bridge->N_sec)
		{
			bridge->open_side = NORTH;
			clock_gettime(CLOCK_REALTIME, &bridge->start_time);
		}
	}

	//If the bridge supports another alien
	if (bridge->aliens_crossing_list->length < bridge->capacity)
		letAlienPass(bridge, bridge->open_side);
}


void survivalAlgorithm(struct Bridge *bridge)
{
	//If there are aliens waiting to cross the bridge
	if (!isListEmpty(bridge->north_aliens_queue) || !isListEmpty(bridge->south_aliens_queue))
	{
		//If there are no aliens crossing
		if (isListEmpty(bridge->aliens_crossing_list))
		{	
			if (bridge->open_side == NORTH && isListEmpty(bridge->north_aliens_queue))
				bridge->open_side = SOUTH;

			else if (bridge->open_side == SOUTH && isListEmpty(bridge->south_aliens_queue))
				bridge->open_side = NORTH;
		}

		//If the bridge supports another alien
		if (bridge->aliens_crossing_list->length < bridge->capacity)
			letAlienPass(bridge, bridge->open_side);
	}
}


void checkBridgeCrossed(struct Bridge *bridge)
{
	Alien *alien_tmp;

	int i = 0;

	//Go through the list of aliens that are crossing the bridge
	for (struct lNode *node = bridge->aliens_crossing_list->head; node != NULL; node = node->next)
	{
		getAt(bridge->aliens_crossing_list, i, (void *)&alien_tmp);

		//If the alien has already crossed the bridge
		if (alien_tmp->stage >= 6)
		{
			removeAt(bridge->aliens_crossing_list, i);
			i--;

			/*printf("--------------------------------\n");
			printf("Y aliens        %i\n", bridge->Y_aliens);
			printf("Y count         %i\n", bridge->Y_count);
			printf("Aliens cruzando %i\n", bridge->aliens_crossing_list->length);
			printf("Bridge side     %i\n", bridge->open_side);
			printf("Aliens en norte %i\n", bridge->north_aliens_queue->length);	
			printf("Aliens en sur   %i\n", bridge->south_aliens_queue->length);
			printf("--------------------------------\n");*/
		}

		i++;
	}
}


void manageTraffic(struct Bridge *bridge)
{
	switch (bridge->transit_control_algorithm)
	{
		case Y:
			YAlgorithm(bridge);
			break;

		case SEMAPHORES:
			semaphoresAlgorithm(bridge);
			break;

		case SURVIVAL:
			survivalAlgorithm(bridge);
			break;

		default:
			break;
	}		

	//If there are aliens on the bridge
	if (!isListEmpty(bridge->aliens_crossing_list))
		checkBridgeCrossed(bridge);
}
