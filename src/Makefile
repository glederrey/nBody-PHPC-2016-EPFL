CC = g++
CFLAGS+=-Wall -g
OPT+=$(CFLAGS) -O3 -ftree-vectorize
LIBS+=-lm
OBJ=Configuration.o Configuration-opt.o brute-force.o brute-force-opt.o
EXEC=brute-force brute-force-opt
OUT=../exec/

all: $(OBJ) $(EXEC)

Configuration.o: Configuration.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

Configuration-opt.o: Configuration.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

brute-force.o: brute-force.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

brute-force-opt.o: brute-force.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

brute-force: brute-force.o Configuration.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

brute-force-opt: brute-force-opt.o Configuration-opt.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

clean :
	cd $(OUT) && rm $(OBJ) $(EXEC)
