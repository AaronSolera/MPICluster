CC = mpicc
RUN = mpirun
MAKE_STATIC_LIB = ar rv
ALLEGRO_FLAGS = -lallegro -lallegro_image -lallegro_primitives
JSON_FLAGS = -ljson-c
LIB = cd ./lib &&
RM_O = cd ./lib && rm *.o

.PHONY: main

#all: allegro json main
#	$(RM_O)

main:
	$(CC) -o ./bin/main ./src/main.c ./src/windowLogic.c -I./include -L./lib $(ALLEGRO_FLAGS) $(JSON_FLAGS) -lpthread -lm
	$(RUN) -np 2 -hosts master,client1 ./bin/main

allegro:
	sudo apt-get install liballegro5-dev

json:
	sudo apt-get install libjson-c-dev
