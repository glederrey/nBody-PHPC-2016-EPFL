CC = g++
MPI = mpic++
CFLAGS+=-Wall -g -DDEBUG -DWRITE_OUTPUT -DWRITE_TIME -DWRITE_QUADTREE
OPT+=$(CFLAGS) -Ofast -ftree-vectorize
LIBS+=-lm
OBJ=Configuration.o Configuration-opt.o brute-force.o brute-force-opt.o brute-force-mpi.o brute-force-mpi-opt.o Body.o Node.o Quadtree.o Body-opt.o Node-opt.o Quadtree-opt.o barnes-hut.o barnes-hut-opt.o
EXEC=brute-force brute-force-opt brute-force-mpi brute-force-mpi-opt barnes-hut barnes-hut-opt
OUT=../exec/

all: $(OBJ) $(EXEC)

# Creation of Object files for class Configuration
Configuration.o: Configuration.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

Configuration-opt.o: Configuration.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

# Compile brute-force unoptimized
brute-force.o: brute-force.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

brute-force: brute-force.o Configuration.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Compile brute-force optimized
brute-force-opt.o: brute-force.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

brute-force-opt: brute-force-opt.o Configuration-opt.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Compile brute-force with mpi
brute-force-mpi.o: brute-force-mpi.cpp
	$(MPI) $(CFLAGS) -o $(OUT)$@ -c $<

brute-force-mpi: brute-force-mpi.o Configuration.o
	$(MPI) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Compile brute-force with mpi and optimization
brute-force-mpi-opt.o: brute-force-mpi.cpp
	$(MPI) $(OPT) -o $(OUT)$@ -c $<

brute-force-mpi-opt: brute-force-mpi-opt.o Configuration-opt.o
	$(MPI) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Creation of Object files for class Body
Body.o: Body.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

Body-opt.o: Body.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

# Creation of Object files for class Node
Node.o: Node.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

Node-opt.o: Node.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

# Creation of Object files for class Quadtree
Quadtree.o: Quadtree.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

Quadtree-opt.o: Quadtree.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

# Compile barnes-hut unoptimized
barnes-hut.o: barnes-hut.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

barnes-hut: barnes-hut.o Configuration.o Body.o Node.o Quadtree.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Compile barnes-hut optimized
barnes-hut-opt.o: barnes-hut.cpp
	$(CC) $(OPT) -o $(OUT)$@ -c $<

barnes-hut-opt: barnes-hut-opt.o Configuration-opt.o Body-opt.o Node-opt.o Quadtree-opt.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)


clean :
	cd $(OUT) && rm -f $(OBJ) $(EXEC)
