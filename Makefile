CC=gcc
CFLAGS=-Wextra -Wall -pedantic
INC=-I include/
SRC=./
EXEC=bf
 
all: $(EXEC)
 
bf: $(SRC)brainfuck.c
	$(CC) $(INC) -o $(SRC)$@ $^ $(CFLAGS) 
 
$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -o $@ -c $< $(CFLAGS) 
 
clean:
	rm -rf $(SRC)*.o $(EXEC)
