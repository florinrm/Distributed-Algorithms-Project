build:
	mpicc heartbeat.c -o heartbeat -lm

run:
	mpirun --oversubscribe -np 12 heartbeat

clean:
	rm heartbeat