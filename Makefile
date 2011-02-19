CC 	= msp430-gcc
CFLAGS 	= -Os -Wall -g

OBJS 	= main.o


all: $(OBJS)
	$(CC) $(CFLAGS) -o lambda.elf $(OBJS)

install: all
	mspdebug rf2500 "erase"
	mspdebug rf2500 "load lambda.elf"

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr lambda.elf $(OBJS)
	mspdebug rf2500 "erase"

