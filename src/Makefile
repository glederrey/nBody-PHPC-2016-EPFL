CC = g++
MPI = mpic++
CFLAGS+=-Wall -g -Ofast -ftree-vectorize -DDEBUG -DWRITE_TIME #-DWRITE_OUTPUT #-DWRITE_QUADTREE
LIBS+=-lm
OBJ=Configuration.o brute-force.o brute-force-mpi.o Body.o Node.o Quadtree.o barnes-hut.o ParallelQuadtree.o barnes-hut-mpi.o
EXEC=brute-force brute-force-mpi barnes-hut barnes-hut-mpi
OUT=../exec/
RESULTS=../results/
create_exec := $(shell mkdir -p $(OUT))
create_results := $(shell mkdir -p $(RESULTS))

all: $(OBJ) $(EXEC)

# Creation of Object files for class Configuration
Configuration.o: Configuration.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

# Compile brute-force unoptimized
brute-force.o: brute-force.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

brute-force: brute-force.o Configuration.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Compile brute-force with mpi
brute-force-mpi.o: brute-force-mpi.cpp
	$(MPI) $(CFLAGS) -o $(OUT)$@ -c $<

brute-force-mpi: brute-force-mpi.o Configuration.o
	$(MPI) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Creation of Object files for class Body
Body.o: Body.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

# Creation of Object files for class Node
Node.o: Node.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

# Creation of Object files for class Quadtree
Quadtree.o: Quadtree.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

# Compile barnes-hut unoptimized
barnes-hut.o: barnes-hut.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

barnes-hut: barnes-hut.o Configuration.o Body.o Node.o Quadtree.o
	$(CC) -o $(OUT)$@ $(OUT)$< $(LIBS)

# Creation of Object files for class ParallelQuadtree
ParallelQuadtree.o: ParallelQuadtree.cpp
	$(CC) $(CFLAGS) -o $(OUT)$@ -c $<

# Compile barnes-hut unoptimized
barnes-hut-mpi.o: barnes-hut-mpi.cpp
	$(MPI) $(CFLAGS) -o $(OUT)$@ -c $<

barnes-hut-mpi: barnes-hut-mpi.o Configuration.o Body.o Node.o Quadtree.o ParallelQuadtree.o
	$(MPI) -o $(OUT)$@ $(OUT)$< $(LIBS)


clean :
	cd $(OUT) && rm -f $(OBJ) $(EXEC)
