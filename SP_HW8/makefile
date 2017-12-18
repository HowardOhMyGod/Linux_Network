server: shm_server.o lookup5.o dict.h
	gcc -o shm_server lookup5.o shm_server.c -l pthread

client: main.o lookup6.o dict.h
	gcc -o shm_client lookup6.o main.c -l pthread
