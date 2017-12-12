producer: producer.o ../packet.h
	gcc -o producer ../packet.h producer.c -lrt -pthread

consumer: consumer.o ../packet.h
	gcc -o consumer ../packet.h consumer.c -lrt -pthread
