CC     = gcc
CFLAGS = -I. -Isrc -Isrc/patient -Isrc/doctor -Isrc/billing -Isrc/id_manager -Isrc/common -Isrc/VisitLog -Isrc/Admin -Wall -Wextra
SRC    = main.c \
         src/patient/patient.c \
         src/doctor/doctor.c \
         src/billing/billing.c \
         src/id_manager/id_manager.c \
         src/common/common.c \
         src/VisitLog/visitlog.c \
         src/Admin/Admin.c
OBJ    = $(SRC:.c=.o)
TARGET = hms.exe

.PHONY: all clean run directories

all: directories $(TARGET)



$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

run: all
	@./$(TARGET)

clean:
	@taskkill /F /IM hms.exe >nul 2>&1 || exit 0
	@del /Q $(OBJ) $(TARGET) 2>nul || exit 0
