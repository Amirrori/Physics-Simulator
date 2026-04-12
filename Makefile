all:
	gcc raylibSim.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
