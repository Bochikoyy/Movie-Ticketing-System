#ifndef UTILITIES_H
#define UTILITIES_H

// ---------------------------------------------------------
// STANDARD LIBRARIES
// ---------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

// ---------------------------------------------------------
// ANSI COLOR CODES
// ---------------------------------------------------------
// These are special escape sequences that tell the terminal to change text color.
// Example: printf(COLOR_RED "Error" COLOR_RESET);
#define COLOR_RESET   "\033[0m"   // Resets color back to default
#define COLOR_RED     "\033[31m"  // Red text (Used for Errors/Sold Out)
#define COLOR_GREEN   "\033[32m"  // Green text (Used for Success/Available)
#define COLOR_YELLOW  "\033[33m"  // Yellow text (Used for Headers/VIP)
#define COLOR_BLUE    "\033[34m"  // Blue text
#define COLOR_MAGENTA "\033[35m"  // Magenta text (Used for Borders)
#define COLOR_CYAN    "\033[36m"  // Cyan text (Used for Titles)
#define COLOR_WHITE   "\033[37m"  // White text (Standard)

// ---------------------------------------------------------
// SCREEN CONFIGURATION
// ---------------------------------------------------------
// Defined width of the terminal window used for centering calculations.
#define SCREEN_WIDTH 100 

// ---------------------------------------------------------
// FUNCTION PROTOTYPES
// ---------------------------------------------------------

// Clears all text from the terminal window (cls/clear).
void clearScreen();

// Pauses the program for a specific amount of time (used for animations).
// @param milliseconds: Time to wait (e.g., 1000 = 1 second).
void pauseExecution(int milliseconds);

// Clears the input buffer (stdin) to remove leftover newline characters.
// This prevents the "skipping" bug when switching between scanf and fgets.
void clearInputBuffer();

// Gets an integer input from the user with robust validation.
// @param x, y: Coordinates to lock the prompt position (prevents visual stacking errors).
// @param prompt: The text to display to the user.
// @param min, max: The allowed range for the number.
// Returns: A valid integer within the specified range.
int getIntInput(int x, int y, const char* prompt, int min, int max); 

// Gets a string input safely from the user.
// Automatically removes the trailing newline character from fgets.
void getStringInput(const char* prompt, char* buffer, int size); 

// Moves the terminal cursor to a specific coordinate.
// @param x: Horizontal position (Column).
// @param y: Vertical position (Row).
void gotoxy(int x, int y); 

#endif
