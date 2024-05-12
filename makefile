all: mutex semaphore monitor

mutex: mutex.c
	gcc -o mutex mutex.c -lpthread
	
semaphore: semaphore.c
	gcc -o semaphore semaphore.c -lpthread
	
monitor: mon.c
	gcc -o monitor mon.c -lpthread
