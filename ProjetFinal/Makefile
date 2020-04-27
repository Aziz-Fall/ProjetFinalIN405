run: main 
	valgrind ./main data

main: list_pages.c main.c communication.c memory_manager.c
	gcc *.c -o main -Wall -g -O1 -lpthread

clean: 
	rm -fr main 
	clear



#-pthread -fstack-protector-all -Wstack-protector
