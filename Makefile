
#Dependcy Tree
simpledu: LogFunction.o utils.o simpledu.o flags.o signal.o
	gcc -Wall LogFunction.o utils.o simpledu.o flags.o signal.o -o simpledu

LogFunction.o: LogFunction.c LogFunction.h
	gcc -Wall LogFunction.c -c

utils.o: utils.c utils.h
	gcc -Wall utils.c -c

simpledu.o: simpledu.c utils.h LogFunction.h flags.h signal.h
	gcc -Wall simpledu.c -c

flags.o: flags.c flags.h
	gcc -Wall flags.c -c

signal.o: signal.c signal.h 
	gcc -Wall signal.c -c
