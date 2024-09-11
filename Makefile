CC = cc
CFLAGS = -Wall -O2

OBJS = main.o json.o

all: json
.c.o:
	$(CC) -c $(CFLAGS) $<
json: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
clean:
	rm -f *.o json
