CC = mpicc
RUN = mpirun
MAKE_STATIC_LIB = ar rv
ALLEGRO_FLAGS = -lallegro -lallegro_image -lallegro_primitives
LIB = cd ./lib &&
RM_O = cd ./lib && rm *.o

.PHONY: all

all: allegro main
#	$(RM_O)

main:
	$(CC) -o ./bin/main ./src/main.c ./src/windowLogic.c $(ALLEGRO_FLAGS) -lm
	$(RUN) -np 2 -hosts master,client1 ./bin/main

allegro:
	sudo apt-get install liballegro5-dev
