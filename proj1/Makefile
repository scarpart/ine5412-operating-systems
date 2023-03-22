PROJ_NAME=main
 
C_SOURCE=$(wildcard *.cc)
 
H_SOURCE=$(wildcard *.h)
 
OBJ=$(C_SOURCE:.cc=.o)
 
CC=g++
 
CC_FLAGS= -g -c -Wall
 
#Compilation
all: $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^
 
clean:
	rm -rf *.o $(PROJ_NAME) *~