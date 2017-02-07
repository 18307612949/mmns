COBJS=main.c funclib.c
CC=gcc
OUT=mmns
DEBUG=-gdwarf-2 -g3

all:
	$(CC) -o $(OUT) $(COBJS)

debug:
	$(CC) $(DEBUG) -o $(OUT) $(COBJS)

clean:
	rm $(OUT)
