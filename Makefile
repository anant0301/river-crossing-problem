
schedule: main.o schedule.o queue.o
	gcc -o schedule.out main.o schedule.o queue.o -lpthread -pthread

main.o: main.c
	gcc -c $< -o $@
queue.o: queue.c
	gcc -c $< -o $@
schedule.o: schedule.c
	gcc -c $< -o $@

clean:
	rm -f *.o *.out