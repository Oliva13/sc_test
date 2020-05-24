init_sc: main.o sconly.o
	gcc -o init_sc main.o sconly.o -L/usr/local/lib -lpcsclite
	
main.o: main.c
	gcc -DTEST_2 -c main.c

sconly.o: sconly.c
	gcc -c sconly.c 
	
clean:
	rm *.o init_sc
