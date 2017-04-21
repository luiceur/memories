CC=cc
OPT=-hlist=m
CFLAGS=-I.
DEPS = 
OBJ = strided.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPT)

stride: $(OBJ)
	cc -o $@ $^ $(CFLAGS) $(OPT)

clean:
	rm $(OBJ) stride
