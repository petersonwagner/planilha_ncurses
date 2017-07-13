CFLAGS = -Wall

myxcel: myxcel.o formulas.o xcelfuncs.o csvfile.o commline.o
	gcc $(CFLAGS) myxcel.o formulas.o xcelfuncs.o csvfile.o commline.o -lncurses -o myxcel

myxcel.o: myxcel.c formulas.c xcelfuncs.c csvfile.c
	gcc $(CFLAGS) -c myxcel.c

xcelfuncs.o: xcelfuncs.c datastruct.h
	gcc $(CFLAGS) -c xcelfuncs.c

formulas.o: formulas.c datastruct.h
	gcc $(CFLAGS) -c formulas.c

csvfile.o: csvfile.c commline.c datastruct.h
	gcc $(CFLAGS) -c csvfile.c

commline.o: commline.c datastruct.h
	gcc $(CFLAGS) -c commline.c

clean:
	rm -f csvfile.o xcelfuncs.o commline.o datastruct.o myxcel.o formulas.o *~

purge: clean
	rm -f myxcel
