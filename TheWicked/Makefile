CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC_DIR = src
OBJ = $(SRC_DIR)/main.o $(SRC_DIR)/ui.o $(SRC_DIR)/tickets.o $(SRC_DIR)/payments.o $(SRC_DIR)/utilities.o
EXEC = WickedTicketingSystem

# Main target
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Rule to compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)