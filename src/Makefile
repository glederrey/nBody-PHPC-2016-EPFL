CC = g++
MPI = mpic++
CFLAGS+=-Wall -g -DDEBUG -DWRITE_OUTPUT -DWRITE_TIME
OPT+=$(CFLAGS) -Ofast -ftree-vectorize
LIBS+=-lm
OBJ=Configuration.o Configuration-opt.o brute-force.o brute-force-opt.o brute-force-mpi.o brute-force-mpi-opt.o
EXEC=brute-force brute-force-opt brute-force-mpi brute-force-mpi-opt
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

clean :
	cd $(OUT) && rm $(OBJ) $(EXEC)
