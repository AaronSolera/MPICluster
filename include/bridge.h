#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <json-c/json.h>
#include <queue.h>
#include "alienLogic.h"

#define WEST_BRIDGE_CONFIG_FILENAME 	"./files/west_bridge_config.json"
#define CENTRAL_BRIDGE_CONFIG_FILENAME  "./files/central_bridge_config.json"
#define EAST_BRIDGE_CONFIG_FILENAME  	"./files/east_bridge_config.json"

#define ROUND_ROBIN 						0
#define PRIORITY							1
#define SJF 								2
#define LOTTERY								3
#define FIFO 								4

#define Y 									0
#define SEMAPHORES							1
#define SURVIVAL							2

#define NORTH 								0
#define SOUTH 								1

struct Bridge
{
	int length;

	int capacity;

	int scheduling_algorithm;
	int ordered_aliens_per_side;

	int transit_control_algorithm;

	int Y_aliens;
	int Y_count;

	int M_sec;
	int N_sec;

	struct timespec start_time;
	struct timespec current_time;

	int open_side;

	struct List *north_aliens_queue;
	struct List *south_aliens_queue;
	struct List *aliens_crossing_list;
};

void initBridge(struct Bridge *bridge, const char *filename);

int priority(const void *a, const void *b);

int shortestJobFirst(const void *a, const void *b, const void *c);

void sortBridgeSide(struct Bridge *bridge, struct List **queue);

void insertAlienInNorth(struct Bridge *bridge, Alien *alien);

void insertAlienInSouth(struct Bridge *bridge, Alien *alien);

void getAlienFromQueue(struct List **queue, struct List **aliens_crossing_list, int scheduling_algorithm, int ordered_aliens_per_side);

void letAlienPass(struct Bridge *bridge, int side);

void YAlgorithm(struct Bridge *bridge);

void semaphoresAlgorithm(struct Bridge *bridge);

void survivalAlgorithm(struct Bridge *bridge);

void checkBridgeCrossed(struct Bridge *bridge);

void manageTraffic(struct Bridge *bridge);

#endif
