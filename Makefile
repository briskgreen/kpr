kprd:main.o kprd.o config.o mysock.o
	gcc -o kprd main.o kprd.o config.o mysock.o -lssl

main.o:main.c
	gcc -c main.c

kprd.o:kprd.c
	gcc -c kprd.c

config.o:config.c
	gcc -c config.c

clean:
	rm kprd *.o
