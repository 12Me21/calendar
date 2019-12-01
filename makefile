pcal: cal.o phase.o
	cc cal.o phase.o -lm -o pcal

cal.o: cal.c
	cc -c cal.c -Os

phase.o: phase.c
	cc -c phase.c -Os
