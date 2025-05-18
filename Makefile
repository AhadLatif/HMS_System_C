# Makefile for HMS Project

CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = src/main.c src/patient.c src/doctor.c src/billing.c src/common.c src/id_manager.c src/doctor.c src/billing.c 
OBJ = $(SRC:.c=.o)
TARGET = HMS

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o *.exe 2> nul
