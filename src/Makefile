CC = g++
CFLAGS+=-Wall -g #-Werror
EXEC_NAME = brute-force
OBJ_FILES=brute-force.o

all: $(EXEC_NAME)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)
