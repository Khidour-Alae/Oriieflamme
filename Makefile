SRC=src
HEADERS=headers
BIN=bin
OBJ=obj

CC=gcc
FLAGS=-Wall -Wextra -std=c99
OBJECTS=$(OBJ)/main.o $(OBJ)/board.o $(OBJ)/cards.o $(OBJ)/faction.o $(OBJ)/interface.o 


all: oriieflamme

$(OBJ)/board.o : $(SRC)/board.c $(HEADERS)/board.h $(HEADERS)/faction.h $(HEADERS)/cards.h
	$(CC) $(FLAGS) -o $@ -c $<
$(OBJ)/cards.o : $(SRC)/cards.c $(HEADERS)/cards.h
	$(CC) $(FLAGS) -o $@ -c $<
$(OBJ)/faction.o : $(SRC)/faction.c $(HEADERS)/faction.h
	$(CC) $(FLAGS) -o $@ -c $<
$(OBJ)/interface.o : $(SRC)/interface.c $(HEADERS)/interface.h $(HEADERS)/board.h $(HEADERS)/cards.h $(HEADERS)/faction.h
	$(CC) $(FLAGS) -o $@ -c $<
$(OBJ)/main.o : $(SRC)/main.c $(HEADERS)/board.h $(HEADERS)/cards.h $(HEADERS)/faction.h $(HEADERS)/interface.h
	$(CC) $(FLAGS) -o $@ -c $<

bin/oriieflamme: $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^