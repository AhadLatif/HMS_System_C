# Makefile at HMS_System_C/Makefile

CC     = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC    = src/main.c \
         src/patient.c \
         src/common.c \
         src/doctor.c \
         src/billing.c \
         src/id_manager.c
OBJ    = $(SRC:.c=.o)
TARGET = hms.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET) 2>nul || rm -f src/*.o $(TARGET)
